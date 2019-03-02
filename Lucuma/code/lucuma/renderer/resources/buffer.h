#pragma once

#include "../../types.h"
#include "../renderer.h"

namespace lu
{
	enum class BufferUsage
	{
		USAGE_DEFAULT,
		USAGE_IMMUTABLE,
		USAGE_DYNAMIC,
		USAGE_STAGING
	};

	enum BufferBind
	{
		BIND_VERTEX_BUFFER = 1,
		BIND_INDEX_BUFFER = 2,
		BIND_CONSTANT_BUFFER = 4,
		BIND_UNORDERED_ACCESS = 8
	};

	enum CPUAccess
	{
		CPU_ACCESS_NONE = 0,
		CPU_ACCESS_WRITE = 1,
		CPU_ACCESS_READ = 2
	};

	enum ResourceType
	{
		RESOURCE_NONE = 0x00000,
		RESOURCE_GENERATE_MIPS = 0x00002,
		RESOURCE_SHARED = 0x00004,
		RESOURCE_TEXTURECUBE = 0x00008,
		RESOURCE_DRAWINDIRECT_ARGS = 0x00010,
		RESOURCE_BUFFER_ALLOW_RAW_VIEWS = 0x00020,
		RESOURCE_BUFFER_STRUCTURED = 0x00040,
		RESOURCE_RESOURCE_CLAMP = 0x00080,
		RESOURCE_SHARED_KEYEDMUTEX = 0x00100,
		RESOURCE_GDI_COMPATIBLE = 0x00200,
		RESOURCE_SHARED_NTHANDLE = 0x00400,
		RESOURCE_RESTRICTED_CONTENT = 0x00800,
		RESOURCE_RESTRICT_SHARED_RESOURCE = 0x01000,
		RESOURCE_RESTRICT_SHARED_RESOURCE_DRIVER = 0x02000,
		RESOURCE_GUARDED = 0x04000,
		RESOURCE_TILE_POOL = 0x08000,
		RESOURCE_TILED = 0x10000
	};

	enum class MapType
	{
		MAP_READ,
		MAP_WRITE,
		MAP_READ_WRITE,
		MAP_WRITE_DISCARD,
		MAP_WRITE_NO_OVERWRITE
	};

	struct Buffer
	{
		Buffer() : buffer(LU_INVALID_HANDLE) {}
		bool isValid() { return buffer != LU_INVALID_HANDLE; }
		handle_t buffer;
	};

	struct MappedBufferInfo
	{
		void* pMappedAddress;
		uint32_t byteSize;
	};

	namespace resources
	{
		bool CreateBuffer(const RendererDevice& device, uint32_t bufferBytesSize, lu::BufferUsage usage, uint32_t binding, uint32_t cpuAccess, uint32_t resourceType, uint32_t structureByteSize, const void* pInitialData, Buffer& bufferObject);
		void DestroyBuffer(const RendererDevice& device, Buffer& bufferObject);

		void* MapBuffer(const RendererContext& context, const Buffer& buffer, MapType mapType);
		void UnmapBuffer(const RendererContext& context, const Buffer& buffer);
	}
}
