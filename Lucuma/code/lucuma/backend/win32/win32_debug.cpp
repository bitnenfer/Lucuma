#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

#include "../../debug.h"
#include "../../types.h"

#define LU_LOG_BUFFER_SIZE 4096
#define LU_LOG_BUFFER_COUNT 3

static char* pLogBuffers[LU_LOG_BUFFER_SIZE * LU_LOG_BUFFER_COUNT];
static uint32_t gLogBufferIndex = 0;

void lu::debug::Log(const char* pFmt, ...)
{
	va_list vl;
	va_start(vl, pFmt);
	char *const pBuffer = (char *const)&pLogBuffers[gLogBufferIndex * LU_LOG_BUFFER_SIZE];
	gLogBufferIndex = (gLogBufferIndex + 1) % LU_LOG_BUFFER_COUNT;
	vsprintf_s(pBuffer, LU_LOG_BUFFER_SIZE, pFmt, vl);
	OutputDebugStringA(pBuffer);
	puts(pBuffer);
	va_end(vl);
}
void lu::debug::LogAndAlert(const char* pFmt, ...)
{
	va_list vl;
	va_start(vl, pFmt);
	char *const pBuffer = (char *const)&pLogBuffers[gLogBufferIndex * LU_LOG_BUFFER_SIZE];
	gLogBufferIndex = (gLogBufferIndex + 1) % LU_LOG_BUFFER_COUNT;
	vsprintf_s(pBuffer, LU_LOG_BUFFER_SIZE, pFmt, vl);
	OutputDebugStringA(pBuffer);
	puts(pBuffer);
	va_end(vl);
	ErrorBox("Error", "Failed Assertion\n%s", pBuffer);
}
void lu::debug::Break()
{
	DebugBreak();
}
void lu::debug::Panic()
{
	DebugBreak();
	exit(EXIT_FAILURE);
}

void lu::debug::ErrorBox(const char* pTitle, const char* pFmt, ...)
{
	va_list vl;
	va_start(vl, pFmt);
	char *const pBuffer = (char *const)&pLogBuffers[gLogBufferIndex * LU_LOG_BUFFER_SIZE];
	gLogBufferIndex = (gLogBufferIndex + 1) % LU_LOG_BUFFER_COUNT;
	vsprintf_s(pBuffer, LU_LOG_BUFFER_SIZE, pFmt, vl);
	OutputDebugStringA(pBuffer);
	puts(pBuffer);
	va_end(vl);
	MessageBox(NULL, pBuffer, pTitle, MB_OK | MB_ICONWARNING);
}
