#include "donut_pch.h"
#include "job.h"

namespace donut {

	void FunctionJob::Execute()
	{
		function();
	}

	Workload::Workload(Job::Priority priority)
		: priority(priority)
	{
		
	}

	void Workload::Add(Job* job)
	{
		queue.enqueue(job);
		job_counter += 1;
	}

	void Workload::JobCompleted(Job* job)
	{
		job_counter -= 1;
	}

	Job* Workload::GetNextJob()
	{
		Job* job;
		if (!queue.try_dequeue(job))
			return nullptr;
		return job;
	}

	WorkloadSet::WorkloadSet(PoolAllocator& allocator)
		: allocator(allocator)
	{
		for (size_t i = 0; i < workloads.size(); i++)
			workloads[i] = std::make_unique<Workload>(static_cast<Job::Priority>(i));
	}

	void WorkloadSet::JobCompleted(Job* job)
	{
		workloads[static_cast<size_t>(job->priority)]->JobCompleted(job);
		job->~Job();
		allocator.Deallocate(job);

		all_job_count -= 1;
	}

	bool WorkloadSet::HasJobs(Job::Priority priority)
	{
		return workloads[static_cast<size_t>(priority)]->HasJobs();
	}

	void WorkloadSet::Wait()
	{
		std::unique_lock lock(wait_mutex);
		condition.wait(lock);
	}

	void WorkloadSet::NotifyAll()
	{
		condition.notify_all();
	}

	void WorkloadSet::Add(Job* job, Job::Priority priority)
	{
		all_job_count += 1;
		job->priority = priority;
		workloads[static_cast<size_t>(priority)]->Add(job);
		condition.notify_one();
	}

	void WorkloadSet::AddException(std::exception_ptr exception)
	{
		std::scoped_lock lock(exception_mutex);
		exceptions.push_back(exception);
	}

	void WorkloadSet::RethrowExceptions()
	{
		std::scoped_lock lock(exception_mutex);
		for (auto& exception : exceptions)
			std::rethrow_exception(exception);
	}

	Job* WorkloadSet::GetNextJob()
	{
		Job* job = nullptr;
		for (int i = 0; i < static_cast<int>(Job::Priority::Count); i++)
		{
			job = workloads[i]->GetNextJob();
			if (job) break;
		}

		return job;
	}

	Job* WorkerThread::GetNextJob()
	{
		return workload_set.GetNextJob();
	}

	WorkerThread::WorkerThread(int index, WorkloadSet& workload_set)
		: workload_set(workload_set)
		, index(index)
		, enabled(true)
	{

		thread = std::thread([&]() 
		{
			std::string name = std::string("Worker ") + std::to_string(this->index);
			while (enabled)
			{
				auto* job = GetNextJob();
				if (job)
				{
					try
					{
						job->Execute();
						workload_set.JobCompleted(job);
					}
					catch (...)
					{
						workload_set.AddException(std::current_exception());
						workload_set.JobCompleted(job);
					}
				}
				else
					workload_set.Wait();
			}
		});
	}

	void WorkerThread::Join()
	{
		if (thread.joinable())
			thread.join();
	}

	void WorkerThread::Stop()
	{
		enabled = false;
	}

	Scheduler::Scheduler(uint32_t thread_number)
		: pool_allocator(JOB_POOL_ITEM_SIZE, JOB_POOL_ITEM_COUNT)
		, workload_set(pool_allocator)
	{
		for (size_t i = 0; i < thread_number; i++)
			threads.push_back(std::make_unique<WorkerThread>(i, workload_set));
	}

	Scheduler::~Scheduler()
	{
		for (auto& thread : threads)
			thread->Stop();

		workload_set.NotifyAll();

		for (auto& thread : threads)
			thread->Join();
	}

	namespace
	{
		std::unique_ptr<Scheduler> scheduler;
	}

	void Scheduler::Initialize()
	{
		assert(!scheduler);
		scheduler = std::make_unique<Scheduler>(std::thread::hardware_concurrency());
	}
	
	void Scheduler::Shutdown()
	{
		assert(scheduler);
		scheduler = nullptr;
	}
	
	void Scheduler::Wait(Job::Priority priority)
	{
		while (workload_set.HasJobs(priority))
			std::this_thread::yield();
	}

	void Scheduler::WaitAll()
	{
		while (workload_set.GetTotalJobCount() > 0)
		{
			std::this_thread::yield();
		}
	}

	void Scheduler::RethrowExceptions()
	{
		workload_set.RethrowExceptions();
	}

	Scheduler& Scheduler::Get()
	{
		assert(scheduler);
		return *scheduler;
	}

}