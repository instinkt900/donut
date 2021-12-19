#pragma once

#include <mutex>
#include "utils/math_utils.h"

namespace donut {
	template <typename T>
	class Allocator : public std::allocator<T>
	{
	public:
		T* allocate(size_t count)
		{
			auto size = sizeof(T) * count;
			T* result = (T*)_aligned_malloc(size, 16);
			return result;
		}

		T* allocate_aligned(size_t count, size_t alignment)
		{
			auto size = sizeof(T) * count;
			T* result = (T*)_aligned_malloc(size, (size_t)NextPowerOfTwo((uint32_t)alignment));
			return result;
		}

		void deallocate(T* ptr, size_t count)
		{
			_aligned_free(ptr);
		}
	};

	class PoolAllocatorBase
	{
	public:
		PoolAllocatorBase(size_t element_size, size_t count, void* memory);
		virtual ~PoolAllocatorBase() = default;

		void* Allocate();
		void Deallocate(void* item);
		size_t GetElementSize() const { return element_size; }
	
		bool CanAllocate() const { return allocated_count < element_count; }
		size_t GetAllocatedCount() const { return allocated_count; }
		const void* GetFreeListStart() const { return free_list_start; }

	protected:
		void SetupFreeList();

	protected:
		size_t allocated_count;
		size_t pool_item_size; // may be different from element size if alignment added
		size_t element_size; 
		size_t element_count; 
		void* pool_memory;
		void* free_list_start;
		std::mutex mutex;
	};

	class PoolAllocator : public PoolAllocatorBase
	{
	public:
		PoolAllocator(size_t element_size, size_t count)
			: PoolAllocatorBase(element_size, count, allocator.allocate(GetPoolMemorySize(element_size, count))) 
		{}

		~PoolAllocator()
		{
			allocator.deallocate((char*)pool_memory, GetPoolMemorySize(element_size, element_count));
		}
	
	private:
		size_t GetPoolMemorySize(size_t element_size, size_t count)
		{
			return element_size * count;
		}

	private:
		Allocator<char> allocator;
	};

}