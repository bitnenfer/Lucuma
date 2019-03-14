#include "gltf.h"

namespace lu
{
	namespace gltf
	{
		struct Header
		{
			uint32_t magic;
			uint32_t version;
			uint32_t length;
		};
	}
}

void* lu::gltf::Parse(const void* pBuffer, uint64_t bufferSize)
{
	byte_t* pByteBuffer = (byte_t*)pBuffer;


	return NULL;
}
