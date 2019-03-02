#pragma once

#include "allocator.h"

namespace lu
{
	struct PageAllocator : public IAllocator
	{
		virtual void* allocate(uint64_t size) override;
		virtual void* allocateAligned(uint64_t size, uint32_t alignment) override;
		virtual void deallocate(void* p) override;
		virtual void reset() override;
		virtual uint64_t getCapacityInBytes() override;
		virtual uint64_t getUsedSpaceInBytes() override;
		virtual uint64_t getAvailableSpaceInBytes() override;

		uint64_t usedBytes;
	};
}
