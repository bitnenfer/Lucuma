#include "lucuma/lucuma.h"
#include "lucuma/utils/loaders/json.h"

void demo4()
{
	lu::PageAllocator pageAllocator;
	byte_t* pFileBuffer = NULL;
	uint64_t fileSize = 0;
	LU_ASSERT(lu::file::ReadFileData(pageAllocator, "data/hello.json", &pFileBuffer, &fileSize));

	////
	lu::JSONTable table;
	lu::json::Parse(pageAllocator, pFileBuffer, fileSize, table);
	const lu::JSONArray* pRootArray = lu::json::GetRootArray(table);
	for (uint32_t i = 0, size = lu::json::GetArrayElementCount(*pRootArray); i < size; ++i)
	{
		const lu::JSONObject* pObject = lu::json::GetObjectFromArray(table, *pRootArray, i);
		if (pObject != NULL)
		{
			float32_t one					= lu::json::GetFloatFromObject(	 table, *pObject, "One");
			bool two						= lu::json::GetBoolFromObject(	 table, *pObject, "Two");
			bool hello						= lu::json::GetBoolFromObject(	 table, *pObject, "Hello");
			const lu::JSONArray* pPotatoe	= lu::json::GetArrayFromObject(	 table, *pObject, "Potatoe");
			const char* pSH					= lu::json::GetStringFromObject( table, *pObject, "SH");
			
			lu::debug::Log("One: %f\n", one);
			lu::debug::Log("Two: %s\n", two ? "true" : "false");
			lu::debug::Log("Hello: %s\n", hello ? "true" : "false");
			lu::debug::Log("Potatoe: ");

			for (uint32_t j = 0, s = lu::json::GetArrayElementCount(*pPotatoe); j < s; ++j)
			{
				int32_t val = lu::json::GetIntFromArray(table, *pPotatoe, j);
				lu::debug::Log("%d ", val);
			}
			
			lu::debug::Log("\n");
			lu::debug::Log("SH: %s\n", pSH);
		}
	}

	lu::json::Free(table);
	////


	lu::file::FreeFileData(pageAllocator, pFileBuffer);
	pFileBuffer = NULL;
	fileSize = 0;
}
