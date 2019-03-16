#include "../../page_allocator.h"
#include <Windows.h>

void* lu::PageAllocator::allocate(uint64_t size)
{
	void* p = (void*)VirtualAlloc(NULL, (SIZE_T)size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	return p;
}
void* lu::PageAllocator::allocateAligned(uint64_t size, uint32_t alignment)
{
	return allocate(size);
}
void lu::PageAllocator::deallocate(void* p)
{
	VirtualFree(p, 0, MEM_RELEASE);
}
void lu::PageAllocator::reset()
{
	// Nothing to reset
}
uint64_t lu::PageAllocator::getCapacityInBytes()
{
	return UINT64_MAX;
}
uint64_t lu::PageAllocator::getUsedSpaceInBytes()
{
	return usedBytes;
}
uint64_t lu::PageAllocator::getAvailableSpaceInBytes()
{
	return UINT64_MAX;
}
