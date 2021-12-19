#pragma once

#include "allocator.h"
#include "concurrentqueue.h"

namespace donut {

	constexpr size_t JOB_POOL_ITEM_SIZE = 256;
	constexpr size_t JOB_POOL_ITEM_COUNT = 1000;

	class Job
	{
	public:
		enum class Priority : int
		{
			Frame = 0,
			High,
			Low,
			Count
		};

		Job() = default;
		virtual ~Job() = default;
		virtual void Execute() {}
	private:
		friend class WorkloadSet;
		Priority priority = Priority::High;
	};

	class FunctionJob : public Job
	{
	public:
		FunctionJob(std::function<void()> function) 
			: Job()
			, function(std::move(function))
		{}

		virtual void Execute() override;
	
	private:
		std::function<void()> function;
	};

	class Workload
	{
	public:
		Workload(Job::Priority priority);
		void Add(Job* job);
		bool HasJobs() const { return job_counter.load() > 0; };
		void JobCompleted(Job* job);

		Job::Priority GetPriority() const { return priority; }
		Job* GetNextJob();

	private:
		std::atomic_uint32_t job_counter = 0;
		moodycamel::ConcurrentQueue<Job*> queue;
		Job::Priority priority;
	};

	class WorkloadSet
	{
	public:
		WorkloadSet(PoolAllocator& allocator);
		void Wait();
		void Add(Job* job, Job::Priority priority);
		Job* GetNextJob();
		void JobCompleted(Job* job);
		void NotifyAll();
		bool HasJobs(Job::Priority priority);
		void AddException(std::exception_ptr exception);
		void RethrowExceptions();
		int GetTotalJobCount() const { return all_job_count.load(); }

	private:
		PoolAllocator& allocator;
		std::array<std::unique_ptr<Workload>, (size_t)Job::Priority::Count> workloads;
		std::condition_variable condition;
		std::mutex wait_mutex;

		std::mutex exception_mutex;
		std::list<std::exception_ptr> exceptions;
		std::atomic_int all_job_count = 0;
	};

	class WorkerThread
	{
	public:
		WorkerThread(int index, WorkloadSet& workload_set);
		void Join();
		void Stop();

	private:
		Job* GetNextJob();

	private:
		WorkloadSet& workload_set;
		std::atomic_bool enabled;
		std::thread thread;
		int index;
	};

	class Scheduler
	{
	public:
		static void Initialize();
		static void Shutdown();
		static Scheduler& Get();
		
		template <typename T, typename ...Args>
		T* SpawnJob(Job::Priority priority, Args&& ...args)
		{
			static_assert(sizeof(T) <= JOB_POOL_ITEM_SIZE);
			void* memory = pool_allocator.Allocate();

			if (!memory)
				throw std::runtime_error("Unable to allocate job");

			T* job = new (memory) T(std::forward<Args>(args)...);
			workload_set.Add(job, priority);
			return job;
		}

		void Wait(Job::Priority priority);
		void WaitAll();
		void RethrowExceptions();

		Scheduler(uint32_t thread_number);
		~Scheduler();

	private:
		PoolAllocator pool_allocator; // must be first to be constructed before workload_set
		WorkloadSet workload_set;
		std::vector<std::unique_ptr<WorkerThread>> threads;
	};

}