#include "linear_allocator.h"
#include "macros.h"
#include "../platform.h"
#include "../debug.h"

void lu::LinearAllocator::initialize(byte_t* pBuffer, uint64_t bufferSize)
{
	LU_ASSERT(pBufferHead == NULL);
	pBufferHead = pBuffer;
	pBufferTail = pBuffer;
	capacity = bufferSize;
}

void lu::LinearAllocator::finalize()
{
	pBufferHead = NULL;
	pBufferTail = NULL;
	capacity = 0;
}

void* lu::LinearAllocator::allocate(uint64_t size)
{
	return allocateAligned(size, LU_DEFAULT_ALIGNMENT);
}

void* lu::LinearAllocator::allocateAligned(uint64_t size, uint32_t alignment)
{
	LU_ASSERT(size < getAvailableSpaceInBytes());

	byte_t* p = (byte_t*)LU_ALIGN_ADDRESS(pBufferTail, alignment);
	if (LU_PTR_IN_RANGE(p, pBufferHead, pBufferHead + capacity))
	{
		pBufferTail = p + size;
		return (void*)p;
	}
	return NULL;
}

void lu::LinearAllocator::deallocate(void* p)
{
	LU_UNUSED(p);
	// linear allocator can't deallocate
}

void lu::LinearAllocator::reset()
{
	pBufferTail = pBufferHead;
}

uint64_t lu::LinearAllocator::getCapacityInBytes()
{
	return capacity;
}

uint64_t lu::LinearAllocator::getUsedSpaceInBytes()
{
	uintptr_t head = (uintptr_t)pBufferHead;
	uintptr_t tail = (uintptr_t)pBufferTail;
	return (uint64_t)(tail - head);
}

uint64_t lu::LinearAllocator::getAvailableSpaceInBytes()
{
	uintptr_t total = (uintptr_t)(pBufferHead + capacity);
	return total - getUsedSpaceInBytes();
}
