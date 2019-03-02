#include "../../../resources/buffer.h"
#include <d3d11.h>
#include <malloc.h>
#include <stdio.h>
#include "../../../../debug.h"

static const D3D11_USAGE kBufferUsage[] = 
{
	D3D11_USAGE_DEFAULT,
	D3D11_USAGE_IMMUTABLE,
	D3D11_USAGE_DYNAMIC,
	D3D11_USAGE_STAGING,
};

static const D3D11_MAP kMapType[] = 
{
	D3D11_MAP_READ,
	D3D11_MAP_WRITE,
	D3D11_MAP_READ_WRITE,
	D3D11_MAP_WRITE_DISCARD,
	D3D11_MAP_WRITE_NO_OVERWRITE
};

bool lu::resources::CreateBuffer(const RendererDevice& device, uint32_t bufferBytesSize, lu::BufferUsage usage, uint32_t binding, uint32_t cpuAccess, uint32_t resourceType, uint32_t structureByteSize, const void* pInitialData, Buffer& bufferObject)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11Buffer* pBuffer = NULL;
	D3D11_BUFFER_DESC bufferDesc = {};
	uint32_t bindFlags = 0;
	uint32_t cpuAccessFlags = 0;
	uint32_t resourceMiscFlags = 0;

	if ((binding & BufferBind::BIND_VERTEX_BUFFER) != 0)
		bindFlags |= D3D11_BIND_VERTEX_BUFFER;
	if ((binding & BufferBind::BIND_INDEX_BUFFER) != 0)
		bindFlags |= D3D11_BIND_INDEX_BUFFER;
	if ((binding & BufferBind::BIND_CONSTANT_BUFFER) != 0)
	{
		LU_ASSERT_MSG(bufferBytesSize > 0 && (bufferBytesSize % 16) == 0, "Buffer size needs to be multiple of 16. Selected size is %u bytes", bufferBytesSize);
		bindFlags |= D3D11_BIND_CONSTANT_BUFFER;
	}
	if ((binding & BufferBind::BIND_UNORDERED_ACCESS) != 0)
		bindFlags |= D3D11_BIND_UNORDERED_ACCESS;

	if ((cpuAccess & CPUAccess::CPU_ACCESS_WRITE) != 0)
		cpuAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	if ((cpuAccess & CPUAccess::CPU_ACCESS_READ) != 0)
		cpuAccessFlags |= D3D11_CPU_ACCESS_READ;

	if ((resourceType & RESOURCE_GENERATE_MIPS) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	if ((resourceType & RESOURCE_SHARED) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_SHARED;
	if ((resourceType & RESOURCE_TEXTURECUBE) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
	if ((resourceType & RESOURCE_DRAWINDIRECT_ARGS) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	if ((resourceType & RESOURCE_BUFFER_ALLOW_RAW_VIEWS) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	if ((resourceType & RESOURCE_BUFFER_STRUCTURED) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	if ((resourceType & RESOURCE_RESOURCE_CLAMP) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_RESOURCE_CLAMP;
	if ((resourceType & RESOURCE_SHARED_KEYEDMUTEX) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;
	if ((resourceType & RESOURCE_GDI_COMPATIBLE) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_GDI_COMPATIBLE;
	if ((resourceType & RESOURCE_SHARED_NTHANDLE) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_SHARED_NTHANDLE;
	if ((resourceType & RESOURCE_RESTRICTED_CONTENT) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_RESTRICTED_CONTENT;
	if ((resourceType & RESOURCE_RESTRICT_SHARED_RESOURCE) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_RESTRICT_SHARED_RESOURCE;
	if ((resourceType & RESOURCE_RESTRICT_SHARED_RESOURCE_DRIVER) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_RESTRICT_SHARED_RESOURCE_DRIVER;
	if ((resourceType & RESOURCE_GUARDED) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_GUARDED;
	if ((resourceType & RESOURCE_TILE_POOL) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_TILE_POOL;
	if ((resourceType & RESOURCE_TILED) != 0)
		resourceMiscFlags |= D3D11_RESOURCE_MISC_TILED;

	bufferDesc.ByteWidth = bufferBytesSize;
	bufferDesc.Usage = kBufferUsage[(uint32_t)usage];
	bufferDesc.BindFlags = bindFlags;
	bufferDesc.CPUAccessFlags = cpuAccessFlags;
	bufferDesc.MiscFlags = resourceMiscFlags;
	bufferDesc.StructureByteStride = structureByteSize;

	if (pInitialData != NULL)
	{
		D3D11_SUBRESOURCE_DATA dataDesc = {};
		dataDesc.pSysMem = pInitialData;
		dataDesc.SysMemPitch = 0;
		dataDesc.SysMemSlicePitch = 0;
		HRESULT result = pDevice->CreateBuffer(&bufferDesc, &dataDesc, &pBuffer);
		if (result != S_OK)
		{
			char* pErrorMessage = (char*)alloca(512);
			sprintf_s(pErrorMessage, 512, "Error Creating Buffer: 0x%4X\n", result);
			OutputDebugStringA(pErrorMessage);
			return false;
		}
	}
	else
	{
		HRESULT result = pDevice->CreateBuffer(&bufferDesc, NULL, &pBuffer);
		if (result != S_OK)
		{
			char* pErrorMessage = (char*)alloca(512);
			sprintf_s(pErrorMessage, 512, "Error Creating Buffer: 0x%4X\n", result);
			OutputDebugStringA(pErrorMessage);
			return false;
		}
	}

	bufferObject.buffer = (handle_t)pBuffer;

	return true;
}

void lu::resources::DestroyBuffer(const RendererDevice& device, Buffer& bufferObject)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11Buffer* pBuffer = (ID3D11Buffer*)bufferObject.buffer;
	bufferObject.buffer = LU_INVALID_HANDLE;
	if (pBuffer != NULL)
	{
		pBuffer->Release();
	}
}

void* lu::resources::MapBuffer(const RendererContext& context, const Buffer& buffer, MapType mapType)
{
	ID3D11DeviceContext* pContext = (ID3D11DeviceContext*)context;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = pContext->Map((ID3D11Buffer*)buffer.buffer, 0, kMapType[(uint32_t)mapType], 0, &mappedResource);
	if (result != S_OK)
		return NULL;
	return mappedResource.pData;
}

void lu::resources::UnmapBuffer(const RendererContext& context, const Buffer& buffer)
{
	ID3D11DeviceContext* pContext = (ID3D11DeviceContext*)context;
	pContext->Unmap((ID3D11Buffer*)buffer.buffer, 0);
}
