#pragma once

#include "allocator.h"

namespace lu
{
	struct LinearAllocator : public IAllocator
	{
		LinearAllocator() : pBufferHead(NULL), pBufferTail(NULL), capacity(0) {}

		void initialize(byte_t* pBuffer, uint64_t bufferSize);
		void finalize();

		virtual void* allocate(uint64_t size) override;
		virtual void* allocateAligned(uint64_t size, uint32_t alignment) override;
		virtual void deallocate(void* p) override;
		virtual void reset() override;
		virtual uint64_t getCapacityInBytes() override;
		virtual uint64_t getUsedSpaceInBytes() override;
		virtual uint64_t getAvailableSpaceInBytes() override;

		byte_t* pBufferHead;
		byte_t* pBufferTail;
		uint64_t capacity;
	};
}
