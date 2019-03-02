#pragma once

#define LU_ASSERT(expr) if (!(expr)) lu::debug::Break();
#define LU_ASSERT_MSG(expr, fmt, ...) if (!(expr)) { lu::debug::LogAndAlert("\n-- Assertion Failed on %s at line %u --\n" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); lu::debug::Break(); }
#define LU_WARN_MSG(expr, fmt, ...) if (!(expr)) { lu::debug::Log("\n-- Warning --\n" fmt "\n", ##__VA_ARGS__); }
#define LU_MACRO_CONCAT1(x, y) x##y
#define LU_MACRO_CONCAT2(x, y) LU_MACRO_CONCAT1(x,y)
#define LU_STATIC_ASSERT(expr) static constexpr char LU_MACRO_CONCAT2(_STATIC_ASSERT_,__LINE__)[(expr) ? 1 : -1] = {0}

namespace lu
{
	namespace debug
	{
		void Log(const char* pFmt, ...);
		void LogAndAlert(const char* pFmt, ...);
		void Break();
		void Panic();
		void ErrorBox(const char* pTitle, const char* pFmt, ...);
	}
}
