#pragma once

#include "../types.h"

namespace lu
{
	struct IAllocator
	{
		virtual void* allocate(uint64_t size) = 0;
		virtual void* allocateAligned(uint64_t size, uint32_t alignment) = 0;
		virtual void deallocate(void* p) = 0;
		virtual void reset() = 0;
		virtual uint64_t getCapacityInBytes() = 0;
		virtual uint64_t getUsedSpaceInBytes() = 0;
		virtual uint64_t getAvailableSpaceInBytes() = 0;
	};
}
