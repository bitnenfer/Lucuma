#include "../array.h"
#include "json.h"
#include <float.h>
#include <string.h>
#include <stdlib.h>

#include "../../utils/macros.h"
#include "../../debug.h"

#define JSON_WARN(message, ...) LU_WARN_MSG(0, message, ##__VA_ARGS__)

namespace lu
{
	enum JSONTokenType
	{
		TOKEN_FLOAT,
		TOKEN_INT,
		TOKEN_STRING,
		TOKEN_TRUE,
		TOKEN_FALSE,
		TOKEN_NULL,
		TOKEN_LBRACE,
		TOKEN_RBRACE,
		TOKEN_LBRACK,
		TOKEN_RBRACK,
		TOKEN_COMMA,
		TOKEN_COLON
	};

	struct JSONToken
	{
		JSONTokenType type;
		union Data
		{
			float32_t numberFloat;
			int32_t numberInt;
			struct { char* pStr; uint32_t length; } string;
		} data;
		#if _DEBUG
		int32_t line;
		uint64_t index;
		#endif
	};

	namespace json
	{
		void LogToken(JSONToken& token)
		{
			switch (token.type)
			{
			case lu::TOKEN_FLOAT:
				debug::Log("Token Float %f\n", token.data.numberFloat);
				break;
			case lu::TOKEN_INT:
				debug::Log("Token Int %d\n", token.data.numberInt);
				break;
			case lu::TOKEN_STRING:
			{
				char last = token.data.string.pStr[token.data.string.length];
				token.data.string.pStr[token.data.string.length] = 0;
				debug::Log("Token String %s\n", token.data.string.pStr);
				token.data.string.pStr[token.data.string.length] = last;
				break;
			}
			case lu::TOKEN_TRUE:
				debug::Log("Token True\n");
				break;
			case lu::TOKEN_FALSE:
				debug::Log("Token False\n");
				break;
			case lu::TOKEN_NULL:
				debug::Log("Token Null\n");
				break;
			case lu::TOKEN_LBRACE:
				debug::Log("Token Left Brace ({)\n");
				break;
			case lu::TOKEN_RBRACE:
				debug::Log("Token Right Brace (})\n");
				break;
			case lu::TOKEN_LBRACK:
				debug::Log("Token Left Bracket ([)\n");
				break;
			case lu::TOKEN_RBRACK:
				debug::Log("Token Right Bracket (])\n");
				break;
			case lu::TOKEN_COMMA:
				debug::Log("Token Comma (,)\n");
				break;
			case lu::TOKEN_COLON:
				debug::Log("Token Color (:)\n");
				break;
			default:
				debug::Log("Token Invalid\n");
				break;
			}
		}
		
		int32_t ParseValue(IAllocator& allocator, JSONTable& table, uint32_t& tokenIndex, IArray<JSONToken>&tokens, JSONValueType& type);
		int32_t ParseObject(IAllocator& allocator, JSONTable& table, uint32_t& tokenIndex, IArray<JSONToken>&tokens, JSONValueType& type);
		int32_t ParseArray(IAllocator& allocator, JSONTable& table, uint32_t& tokenIndex, IArray<JSONToken>&tokens, JSONValueType& type);

		int32_t ParsePackedValues(JSONTable& table, uint32_t& tokenIndex, IArray<JSONToken>& tokens, JSONValueType& type)
		{
			JSONToken& token = tokens.at(tokenIndex);
			if (token.type == TOKEN_FLOAT)
			{
				table.floats.push(token.data.numberFloat);
				type = JSON_FLOAT;
				tokenIndex += 1;
				return table.floats.getSize() - 1;
			}
			else if (token.type == TOKEN_INT)
			{
				table.ints.push(token.data.numberInt);
				type = JSON_INT;
				tokenIndex += 1;
				return table.ints.getSize() - 1;
			}
			else if (token.type == TOKEN_FALSE || token.type == TOKEN_TRUE)
			{
				table.bools.push(token.type == TOKEN_TRUE);
				type = JSON_BOOL;
				tokenIndex += 1;
				return table.bools.getSize() - 1;
			}
			else if (token.type == TOKEN_STRING)
			{
				uint32_t valueIndex = table.strings.getSize();
				table.strings.pushBuffer(token.data.string.pStr, token.data.string.length);
				table.strings.push(0);
				type = JSON_STRING;
				tokenIndex += 1;
				return valueIndex;
			}
			else if (token.type == TOKEN_NULL)
			{
				type = JSON_NULL;
				tokenIndex += 1;
				return -1;
			}
			return -1;
		}
		int32_t ParseObject(IAllocator& allocator, JSONTable& table, uint32_t& tokenIndex, IArray<JSONToken>&tokens, JSONValueType& type)
		{
			JSONToken& token = tokens.at(tokenIndex);
			if (token.type == TOKEN_LBRACE)
			{
				tokenIndex += 1;
				if (tokenIndex >= tokens.getSize())
				{
					type = JSON_NULL;
					return -1;
				}
				token = tokens.at(tokenIndex);
				if (token.type == TOKEN_RBRACE) return -1;

				if (token.type != TOKEN_STRING)
				{
					JSON_WARN("Invalid JSON Object at line %u\n", token.line);
					return -1;
				}
				type = JSON_OBJECT;
				JSONObject& object = *(LU_NEW(table.objects.alloc()) JSONObject);
				uint32_t objectIndex = table.objects.getSize() - 1;
				object.keys.setAllocator(&allocator);
				bool repeat = true;

				do
				{
					// Key
					if (token.type == TOKEN_STRING)
					{
						JSONKey key;
						memset(key.key, 0, 64);
						memcpy(key.key, token.data.string.pStr, token.data.string.length);
						key.index = -1;
						key.type = JSON_NULL;
						
						// Colon (:)
						tokenIndex += 1;
						if (tokenIndex >= tokens.getSize())
						{
							type = JSON_NULL;
							return -1;
						}
						token = tokens.at(tokenIndex);
						if (token.type != TOKEN_COLON)
						{
							JSON_WARN("Invalid JSON Object at line %u\n", token.line);
							return -1;
						}

						// Value
						tokenIndex += 1;
						if (tokenIndex >= tokens.getSize())
						{
							type = JSON_NULL;
							return -1;
						}
						key.index = ParseValue(allocator, table, tokenIndex, tokens, key.type);
						object.keys.push(key);

						token = tokens.at(tokenIndex);
						if (token.type == TOKEN_COMMA)
						{
							tokenIndex += 1;
							if (tokenIndex >= tokens.getSize())
							{
								type = JSON_NULL;
								return -1;
							}
							token = tokens.at(tokenIndex);
							continue;
						}
						else if (token.type == TOKEN_RBRACE)
						{
							tokenIndex += 1;
							repeat = false;
						}
						else
						{
							JSON_WARN("Invalid JSON Object at line %u\n", token.line);
						}
					}
					else
						repeat = false;
				}
				while (repeat);
				return objectIndex;
			}
			return -1;
		}
		int32_t ParseArray(IAllocator& allocator, JSONTable& table, uint32_t& tokenIndex, IArray<JSONToken>&tokens, JSONValueType& type)
		{
			JSONToken& token = tokens.at(tokenIndex);
			if (token.type == TOKEN_LBRACK)
			{
				type = JSON_ARRAY;
				tokenIndex += 1;
				if (tokenIndex >= tokens.getSize())
				{
					type = JSON_NULL;
					return -1;
				}
				token = tokens.at(tokenIndex);
				if (token.type == TOKEN_RBRACK) return -1;
				JSONArray& array = *(LU_NEW(table.arrays.alloc()) JSONArray);
				uint32_t arrayIndex = table.arrays.getSize() - 1;
				array.elements.setAllocator(&allocator);
				bool repeat = true;

				do
				{
					JSONValueType valueType = JSON_NULL;
					int32_t valueIndex = ParseValue(allocator, table, tokenIndex, tokens, valueType);
					if (valueIndex > -1)
					{
						JSONElement element;
						element.index = valueIndex;
						element.type = valueType;
						array.elements.push(element);
					}

					token = tokens.at(tokenIndex);
					if (token.type == TOKEN_COMMA)
					{
						tokenIndex += 1;
						if (tokenIndex >= tokens.getSize())
						{
							type = JSON_NULL;
							return -1;
						}
						token = tokens.at(tokenIndex);
						continue;
					}
					else if (token.type == TOKEN_RBRACK)
					{
						tokenIndex += 1;
						repeat = false;
					}
					else
					{
						JSON_WARN("Invalid JSON Array at line %u\n", token.line);
					}

				} while (repeat);
				return arrayIndex;
			}
			return -1;
		}
		int32_t ParseValue(IAllocator& allocator, JSONTable& table, uint32_t& tokenIndex, IArray<JSONToken>&tokens, JSONValueType& type)
		{
			int32_t result = -1;
			if (tokenIndex < tokens.getSize() && (result = ParsePackedValues(table, tokenIndex, tokens, type)) != -1) return result;
			else if (tokenIndex < tokens.getSize() && (result = ParseObject(allocator, table, tokenIndex, tokens, type)) != -1) return result;
			else if (tokenIndex < tokens.getSize() && (result = ParseArray(allocator, table, tokenIndex, tokens, type)) != -1) return result;
			return result;
		}
	}
}

bool lu::json::Parse(IAllocator& allocator, const void* pBuffer, uint64_t bufferSize, JSONTable& table, IArray<JSONToken>* pTokens)
{
	// Tokenizer
	const char* pString = (const char*)pBuffer;
	char currentChar = 0;
	int32_t currentLine = 1;
	uint64_t currentCharIndex = 0;
	bool tokenStreamAllocated = false;

	uint32_t stringsSize = 0;
	uint32_t intsSize = 0;
	uint32_t floatsSize = 0;
	uint32_t boolsSize = 0;
	uint32_t objectsSize = 0;
	uint32_t arraysSize= 0;

	if (pTokens == NULL)
	{
		pTokens = LU_NEW (allocator.allocate(sizeof(Array<JSONToken>)))Array<JSONToken>(&allocator);
		tokenStreamAllocated = true;
	}

	while (currentCharIndex < bufferSize)
	{
		currentChar = pString[currentCharIndex];

		// Number
		if (currentChar == '-' || (currentChar >= '0' && currentChar <= '9'))
		{
			char* pNumStr = (char*)&pString[currentCharIndex];
			uint32_t numSize = 1;
			JSONToken token;
			token.type = TOKEN_INT;
			currentCharIndex += 1;
			currentChar = pString[currentCharIndex];

			if (currentChar == '.')
			{
				token.type = TOKEN_FLOAT;
				numSize += 1;
				currentCharIndex += 1;
				currentChar = pString[currentCharIndex];
			}

			while (currentCharIndex < bufferSize && currentChar >= '0' && currentChar <= '9')
			{
				currentCharIndex += 1;
				currentChar = pString[currentCharIndex];
				
				if (currentChar == '.' && token.type == TOKEN_INT)
				{
					token.type = TOKEN_FLOAT;
					numSize += 1;
					currentCharIndex += 1;
					currentChar = pString[currentCharIndex];
				}
				else if (currentChar == '.')
				{
					JSON_WARN("JSON Parser: Invalid token at line %u\n", currentLine);
					return false;
				}
				numSize += 1;
			}

			if (token.type == TOKEN_INT)
			{
				char oldValue = pNumStr[numSize];
				pNumStr[numSize] = 0;
				int32_t value = atoi(pNumStr);
				pNumStr[numSize] = oldValue;
				token.data.numberInt = value;
				pTokens->push(token);
			}
			else if (token.type == TOKEN_FLOAT)
			{
				char oldValue = pNumStr[numSize];
				pNumStr[numSize] = 0;
				float32_t value = (float32_t)atof(pNumStr);
				pNumStr[numSize] = oldValue;
				token.data.numberFloat = value;
				pTokens->push(token);
			}
			else
			{
				JSON_WARN("JSON Parser: Invalid token at line %u\n", currentLine);
				return false;
			}
		}
		// String
		else if (currentChar == '"')
		{
			JSONToken token;
			token.type = TOKEN_STRING;
			currentCharIndex += 1;

			if (currentCharIndex >= bufferSize)
			{
				JSON_WARN("JSON Parser: Incomplete string at line %u\n", currentLine);
				return false;
			}

			currentChar = pString[currentCharIndex];
			token.data.string.pStr = (char*)&pString[currentCharIndex];
			token.data.string.length = 0;
			while (currentCharIndex < bufferSize && currentChar != '"')
			{
				currentCharIndex += 1;
				if (currentCharIndex >= bufferSize)
				{
					JSON_WARN("JSON Parser: Incomplete string at line %u\n", currentLine);
					return false;
				}
				currentChar = pString[currentCharIndex];
				if (currentChar == '\n') currentLine = 1;
				token.data.string.length += 1;
			}
			pTokens->push(token);
			currentCharIndex += 1;
		}
		// Null
		else if (currentChar == 'n')
		{
			if (currentCharIndex + 3 >= bufferSize)
			{
				JSON_WARN("JSON Parser: Undefined token at line %u\n", currentLine);
				return false;
			}
			if (pString[currentCharIndex + 1] == 'u' &&
				pString[currentCharIndex + 2] == 'l' &&
				pString[currentCharIndex + 3] == 'l')
			{
				JSONToken token;
				token.type = TOKEN_NULL;
				pTokens->push(token);
				currentCharIndex += 4;
			}
			else
			{
				JSON_WARN("JSON Parser: Undefined token at line %u\n", currentLine);
				return false;
			}
		}
		// True
		else if (currentChar == 't')
		{
			if (currentCharIndex + 3 >= bufferSize)
			{
				JSON_WARN("JSON Parser: Undefined token at line %u", currentLine);
				return false;
			}
			if (pString[currentCharIndex + 1] == 'r' &&
				pString[currentCharIndex + 2] == 'u' &&
				pString[currentCharIndex + 3] == 'e')
			{
			JSONToken token;
			token.type = JSONTokenType::TOKEN_TRUE;
			pTokens->push(token);
			currentCharIndex += 4;
			}
			else
			{
				JSON_WARN("JSON Parser: Undefined token at line %u", currentLine);
				return false;
			}
		}
		// False
		else if (currentChar == 'f')
		{
			if (currentCharIndex + 4 >= bufferSize)
			{
				JSON_WARN("JSON Parser:Undefined token at line %u", currentLine);
				return false;
			}
			if (pString[currentCharIndex + 1] == 'a' &&
				pString[currentCharIndex + 2] == 'l' &&
				pString[currentCharIndex + 3] == 's' &&
				pString[currentCharIndex + 4] == 'e')
			{
				JSONToken token;
				token.type = JSONTokenType::TOKEN_FALSE;
				pTokens->push(token);
				currentCharIndex += 5;
			}
			else
			{
				JSON_WARN("JSON Parser:Undefined token at line %u", currentLine);
				return false;
			}
		}
		// Read special chars
		else if (currentChar == '[')
		{
			JSONToken token;
			token.type = JSONTokenType::TOKEN_LBRACK;
			pTokens->push(token);
			currentCharIndex += 1;
		}
		else if (currentChar == ']')
		{
			JSONToken token;
			token.type = JSONTokenType::TOKEN_RBRACK;
			pTokens->push(token);
			currentCharIndex += 1;
		}
		else if (currentChar == '{')
		{
			JSONToken token;
			token.type = JSONTokenType::TOKEN_LBRACE;
			pTokens->push(token);
			currentCharIndex += 1;
		}
		else if (currentChar == '}')
		{
			JSONToken token;
			token.type = JSONTokenType::TOKEN_RBRACE;
			pTokens->push(token);
			currentCharIndex += 1;
		}
		else if (currentChar == ':')
		{
			JSONToken token;
			token.type = JSONTokenType::TOKEN_COLON;
			pTokens->push(token);
			currentCharIndex += 1;
		}
		else if (currentChar == ',')
		{
			JSONToken token;
			token.type = JSONTokenType::TOKEN_COMMA;
			pTokens->push(token);
			currentCharIndex += 1;
		}
		else if (currentChar == ' ' ||
			currentChar == '\n' ||
			currentChar == '\r' ||
			currentChar == '\t' ||
			currentChar == '\0')
		{
			if (currentChar == '\n') currentLine += 1;
			currentCharIndex += 1;
		}
		else
		{
			JSON_WARN("JSON Parser: Undefined token at line %u", currentLine);
			return false;
		}

#if _DEBUG
		pTokens->at(pTokens->getSize() - 1).line = currentLine;
		pTokens->at(pTokens->getSize() - 1).index = currentCharIndex;
#endif
	}

	// Check how much memory is needed
	for (uint32_t index = 0, length = pTokens->getSize(); index < length; ++index)
	{
		JSONToken& token = pTokens->at(index);
		if (token.type == TOKEN_FLOAT) floatsSize += 1;
		else if (token.type == TOKEN_INT) intsSize += 1;
		else if (token.type == TOKEN_FALSE || token.type == TOKEN_TRUE) boolsSize += 1;
		else if (token.type == TOKEN_STRING)
		{
			index += 1;
			if (index < length && pTokens->at(index).type != TOKEN_COLON)
			{
				stringsSize += token.data.string.length + 1;
			}
		}
		else if (token.type == TOKEN_LBRACE)
		{
			objectsSize += 1;
		}
		else if (token.type == TOKEN_LBRACK)
		{
			arraysSize += 1;
		}
	}

	// Initialize arrays
	LU_NEW(&table) JSONTable;

	table.strings.setAllocator(&allocator);
	table.ints.setAllocator(&allocator);
	table.floats.setAllocator(&allocator);
	table.bools.setAllocator(&allocator);
	table.objects.setAllocator(&allocator);
	table.arrays.setAllocator(&allocator);

	table.strings.resize(stringsSize);
	table.ints.resize(intsSize);
	table.floats.resize(floatsSize);
	table.bools.resize(boolsSize);
	table.objects.resize(objectsSize);
	table.arrays.resize(arraysSize);

	uint32_t tokenIndex = 0;
	JSONValueType type = JSON_NULL;
	int32_t valueIndex = ParseValue(allocator, table, tokenIndex, *pTokens, type);

	table.rootIndex = valueIndex;
	table.rootType = type;

	if (tokenStreamAllocated)
	{
		allocator.deallocate(pTokens);
	}

	return valueIndex > -1;
}

void lu::json::Free(JSONTable& table)
{
	for (uint32_t index = 0, length = table.objects.getSize(); index < length; ++index)
	{
		JSONObject& object = table.objects[index];
		object.keys.finalize();
	}

	for (uint32_t index = 0, length = table.arrays.getSize(); index < length; ++index)
	{
		JSONArray& array = table.arrays[index];
		array.elements.finalize();
	}

	table.strings.finalize();
	table.ints.finalize();
	table.floats.finalize();
	table.bools.finalize();
	table.objects.finalize();
	table.arrays.finalize();
}

lu::JSONValueType lu::json::GetTypeOfRoot(const JSONTable& table)
{
	return table.rootType;
}

const lu::JSONObject* lu::json::GetRootObject(const JSONTable& table)
{
	if (table.rootType != JSON_OBJECT) return NULL;
	return &table.objects.constAt(0);
}

const lu::JSONArray* lu::json::GetRootArray(const JSONTable& table)
{
	if (table.rootType != JSON_ARRAY) return NULL;
	return &table.arrays.constAt(0);
}

float32_t lu::json::GetRootFloat(const JSONTable& table)
{
	if (table.rootType != JSON_FLOAT) return 0.0f;
	return table.floats.constAt(0);
}

int32_t lu::json::GetRootInt(const JSONTable& table)
{
	if (table.rootType != JSON_INT) return 0;
	return table.ints.constAt(0);
}

bool lu::json::GetRootBool(const JSONTable& table)
{
	if (table.rootType != JSON_BOOL) return false;
	return table.bools.constAt(0);
}

uint32_t lu::json::GetObjectKeyCount(const JSONObject& object)
{
	return object.keys.getSize();
}
const lu::JSONKey* lu::json::GetKeyFromObject(const JSONObject& object, const char* pKey)
{
	const IArray<JSONKey>& keys = object.keys;
	for (uint32_t index = 0, length = keys.getSize(); index < length; ++index)
	{
		const JSONKey& key = keys.constAt(index);
		if (strcmp(key.key, pKey) == 0)
		{
			return &keys.constAt(index);
		}
	}
	return NULL;
}
lu::JSONValueType lu::json::GetTypeFromObject(const JSONObject& object, const char* pKey)
{
	const JSONKey* pJKey = GetKeyFromObject(object, pKey);
	if (pJKey == NULL)
		return JSON_NULL;

	return pJKey->type;
}

const lu::JSONObject* lu::json::GetObjectFromObject(const JSONTable& table, const JSONObject& object, const char* pKey)
{
	const JSONKey* pJKey = GetKeyFromObject(object, pKey);
	if (pJKey == NULL || pJKey->type != JSON_OBJECT) return NULL;
	return &table.objects.constAt(pJKey->index);
}

const lu::JSONArray* lu::json::GetArrayFromObject(const JSONTable& table, const JSONObject& object, const char* pKey)
{
	const JSONKey* pJKey = GetKeyFromObject(object, pKey);
	if (pJKey == NULL || pJKey->type != JSON_ARRAY) return NULL;
	return &table.arrays.constAt(pJKey->index);
}

float32_t lu::json::GetFloatFromObject(const JSONTable& table, const JSONObject& object, const char* pKey, float32_t defaultValue)
{
	const JSONKey* pJKey = GetKeyFromObject(object, pKey);
	if (pJKey == NULL) return defaultValue;
	if (pJKey->type == JSON_FLOAT)
		return table.floats.constAt(pJKey->index);
	else if (pJKey->type == JSON_INT)
		return (float32_t)table.ints.constAt(pJKey->index);
	return defaultValue;
}

int32_t lu::json::GetIntFromObject(const JSONTable& table, const JSONObject& object, const char* pKey, int32_t defaultValue)
{
	const JSONKey* pJKey = GetKeyFromObject(object, pKey);
	if (pJKey == NULL) return defaultValue;
	if (pJKey->type == JSON_INT)
		return table.ints.constAt(pJKey->index);
	else if (pJKey->type == JSON_FLOAT)
		return (int32_t)table.floats.constAt(pJKey->index);
	return defaultValue;
}

bool lu::json::GetBoolFromObject(const JSONTable& table, const JSONObject& object, const char* pKey, bool defaultValue)
{
	const JSONKey* pJKey = GetKeyFromObject(object, pKey);
	if (pJKey == NULL || pJKey->type != JSON_BOOL) return defaultValue;
	return table.bools.constAt(pJKey->index);
}

const char* lu::json::GetStringFromObject(const JSONTable& table, const JSONObject& object, const char* pKey)
{
	const JSONKey* pJKey = GetKeyFromObject(object, pKey);
	if (pJKey == NULL || pJKey->type != JSON_STRING) return false;
	return &table.strings.constAt(pJKey->index);
}

uint32_t lu::json::GetArrayElementCount(const JSONArray& array)
{
	return array.elements.getSize();
}

const lu::JSONElement* lu::json::GetElementFromArray(const JSONArray& array, uint32_t index)
{
	if (index >= array.elements.getSize()) return NULL;
	return &array.elements.constAt(index);
}

lu::JSONValueType lu::json::GetTypeFromArray(const JSONArray& array, uint32_t index)
{
	const lu::JSONElement* pElement = GetElementFromArray(array, index);
	if (pElement == NULL) return JSON_NULL;
	return pElement->type;
}

const lu::JSONObject* lu::json::GetObjectFromArray(const JSONTable& table, const JSONArray& array, uint32_t index)
{
	const lu::JSONElement* pElement = GetElementFromArray(array, index);
	if (pElement == NULL || pElement->type != JSON_OBJECT) return NULL;
	return &table.objects.constAt(pElement->index);
}

const lu::JSONArray* lu::json::GetArrayFromArray(const JSONTable& table, const JSONArray& array, uint32_t index)
{
	const lu::JSONElement* pElement = GetElementFromArray(array, index);
	if (pElement == NULL || pElement->type != JSON_ARRAY) return NULL;
	return &table.arrays.constAt(pElement->index);
}

float32_t lu::json::GetFloatFromArray(const JSONTable& table, const JSONArray& array, uint32_t index, float32_t defaultValue)
{
	const lu::JSONElement* pElement = GetElementFromArray(array, index);
	if (pElement == NULL) return defaultValue;
	if (pElement->type == JSON_FLOAT)
		return table.floats.constAt(pElement->index);
	else if (pElement->type == JSON_INT)
		return (float32_t)table.ints.constAt(pElement->index);
	return defaultValue;
}

int32_t lu::json::GetIntFromArray(const JSONTable& table, const JSONArray& array, uint32_t index, int32_t defaultValue)
{
	const lu::JSONElement* pElement = GetElementFromArray(array, index);
	if (pElement == NULL) return defaultValue;
	if (pElement->type == JSON_INT)
		return table.ints.constAt(pElement->index);
	else if (pElement->type == JSON_FLOAT)
		return (int32_t)table.floats.constAt(pElement->index);
	return defaultValue;
}

bool lu::json::GetBoolFromArray(const JSONTable& table, const JSONArray& array, uint32_t index, bool defaultValue)
{
	const lu::JSONElement* pElement = GetElementFromArray(array, index);
	if (pElement == NULL || pElement->type != JSON_BOOL) return defaultValue;
	return table.bools.constAt(pElement->index);
}

const char* lu::json::GetStringFromArray(const JSONTable& table, const JSONArray& array, uint32_t index)
{
	const lu::JSONElement* pElement = GetElementFromArray(array, index);
	if (pElement == NULL || pElement->type != JSON_STRING) return NULL;
	return &table.strings.constAt(pElement->index);
}
