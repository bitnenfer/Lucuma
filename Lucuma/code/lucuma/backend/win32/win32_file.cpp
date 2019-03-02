#include "../../file.h"
#include "../../debug.h"
#include <Windows.h>


bool lu::file::ReadFileData(IAllocator& allocator, const char* pFileName, byte_t** ppReadData, uint64_t* pReadSize)
{
	HANDLE file = CreateFileA(pFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD err = GetLastError();
	LU_ASSERT_MSG(err != ERROR_FILE_NOT_FOUND, "File %s not found", pFileName);
	LU_ASSERT_MSG(err != ERROR_PATH_NOT_FOUND, "Path %s not found", pFileName);
	if (file == INVALID_HANDLE_VALUE) return false;
	LARGE_INTEGER size;
	if (!GetFileSizeEx(file, &size)) return false;
	void* pReadBuffer = allocator.allocate((uint64_t)size.QuadPart);
	if (pReadBuffer == NULL) return false;
	DWORD bytesRead = 0;
	if (!ReadFile(file, pReadBuffer, (DWORD)size.QuadPart, &bytesRead, NULL)) return false;
	*ppReadData = (byte_t*)pReadBuffer;
	*pReadSize = (uint64_t)bytesRead;
	CloseHandle(file);
	return true;
}

void lu::file::FreeFileData(IAllocator& allocator, byte_t* pReadData)
{
	allocator.deallocate(pReadData);
}
