#pragma once

#include "../allocator.h"
#include "../../types.h"

namespace lu
{
	
	struct JSONToken;

	enum JSONValueType : uint8_t
	{
		JSON_NULL,
		JSON_FLOAT,
		JSON_INT,
		JSON_BOOL,
		JSON_STRING,
		JSON_ARRAY,
		JSON_OBJECT,
		JSON_KEY
	};

	struct JSONKey
	{
		char key[64];
		int32_t index;
		JSONValueType type;
	};

	struct JSONElement
	{
		int32_t index;
		JSONValueType type;
	};

	struct JSONObject
	{
		Array<JSONKey> keys;
	};

	struct JSONArray
	{
		Array<JSONElement> elements;
	};

	struct JSONTable
	{
		// packed
		Array<char> strings;
		Array<int32_t> ints;
		Array<float32_t> floats;
		Array<bool> bools;

		// not packed
		Array<JSONObject> objects;
		Array<JSONArray> arrays;

		// root data
		JSONValueType rootType;
		int32_t rootIndex;
	};

	namespace json
	{
		bool Parse(IAllocator& allocator, const void* pBuffer, uint64_t bufferSize, JSONTable& table, IArray<JSONToken>* pTokens = NULL);
		void Free(JSONTable& table);

		JSONValueType GetTypeOfRoot(const JSONTable& table);
		const JSONObject* GetRootObject(const JSONTable& table);
		const JSONArray* GetRootArray(const JSONTable& table);
		float32_t GetRootFloat(const JSONTable& table);
		int32_t GetRootInt(const JSONTable& table);
		bool GetRootBool(const JSONTable& table);

		uint32_t GetObjectKeyCount(const JSONObject& object);
		const JSONKey* GetKeyFromObject(const JSONObject& object, const char* pKey);
		JSONValueType GetTypeFromObject(const JSONObject& object, const char* pKey);
		const JSONObject* GetObjectFromObject(const JSONTable& table, const JSONObject& object, const char* pKey);
		const JSONArray* GetArrayFromObject(const JSONTable& table, const JSONObject& object, const char* pKey);
		float32_t GetFloatFromObject(const JSONTable& table, const JSONObject& object, const char* pKey);
		int32_t GetIntFromObject(const JSONTable& table, const JSONObject& object, const char* pKey);
		bool GetBoolFromObject(const JSONTable& table, const JSONObject& object, const char* pKey);
		const char* GetStringFromObject(const JSONTable& table, const JSONObject& object, const char* pKey);

		uint32_t GetArrayElementCount(const JSONArray& array);
		const JSONElement* GetElementFromArray(const JSONArray& array, uint32_t index);
		JSONValueType GetTypeFromArray(const JSONArray& array, uint32_t index);
		const JSONObject* GetObjectFromArray(const JSONTable& table, const JSONArray& array, uint32_t index);
		const JSONArray* GetArrayFromArray(const JSONTable& table, const JSONArray& array, uint32_t index);
		float32_t GetFloatFromArray(const JSONTable& table, const JSONArray& array, uint32_t index);
		int32_t GetIntFromArray(const JSONTable& table, const JSONArray& array, uint32_t index);
		bool GetBoolFromArray(const JSONTable& table, const JSONArray& array, uint32_t index);
		const char* GetStringFromArray(const JSONTable& table, const JSONArray& array, uint32_t index);
	}
}
