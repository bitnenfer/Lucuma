#pragma once

#include "utils/allocator.h"

namespace lu
{
	namespace file
	{
		bool ReadFileData(IAllocator& allocator, const char* pFileName, byte_t** ppReadData, uint64_t* pReadSize);
		void FreeFileData(IAllocator& allocator, byte_t* pReadData);
	}
}