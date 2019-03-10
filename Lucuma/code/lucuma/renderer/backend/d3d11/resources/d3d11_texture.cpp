#include "../../../resources/texture.h"
#include "../../../resources/format.h"
#include <d3d11.h>
#include <malloc.h>
#include <stdio.h>

extern DXGI_FORMAT kResourceFormat[];

bool lu::resources::CreateTexture1D(const RendererDevice& device, ResourceFormat format, const void* pData, uint32_t xSize, uint32_t bindFlags, Texture1D& texture1D)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	D3D11_TEXTURE1D_DESC textureDesc = {};
	ID3D11Texture1D* pTexture = NULL;
	uint32_t binding = 0;

	if ((bindFlags & BindFlag::BIND_SHADER_RESOURCE) != 0)
		binding |= D3D11_BIND_SHADER_RESOURCE;
	if ((bindFlags & BindFlag::BIND_RENDER_TARGET) != 0)
		binding |= D3D11_BIND_RENDER_TARGET;
	if ((bindFlags & BindFlag::BIND_DEPTH_STENCIL) != 0)
		binding |= D3D11_BIND_DEPTH_STENCIL;
	if ((bindFlags & BindFlag::BIND_UNORDERED_ACCESS) != 0)
		binding |= D3D11_BIND_UNORDERED_ACCESS;

	textureDesc.Format = kResourceFormat[(uint32_t)format];
	textureDesc.BindFlags = (D3D11_BIND_FLAG)binding;
	textureDesc.ArraySize = 1;
	textureDesc.Width = xSize;
	textureDesc.MipLevels = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (pData != NULL)
	{
		uint32_t pixelByteSize = GetResourceFormatBytesPerPixel(format);
		D3D11_SUBRESOURCE_DATA dataDesc = {};
		dataDesc.pSysMem = pData;
		dataDesc.SysMemPitch = xSize * pixelByteSize;
		HRESULT result = pDevice->CreateTexture1D(&textureDesc, &dataDesc, &pTexture);
		if (result != S_OK)
		{
			char* pErrorMessage = (char*)alloca(512);
			sprintf_s(pErrorMessage, 512, "Error Creating Texture 1D: 0x%4X\n", result);
			OutputDebugStringA(pErrorMessage);
			return false;
		}
	}
	else
	{
		HRESULT result = pDevice->CreateTexture1D(&textureDesc, NULL, &pTexture);
		if (result != S_OK)
		{
			char* pErrorMessage = (char*)alloca(512);
			sprintf_s(pErrorMessage, 512, "Error Creating Texture 1D: 0x%4X\n", result);
			OutputDebugStringA(pErrorMessage);
			return false;
		}
	}

	texture1D.width = xSize;
	texture1D.texture = (handle_t)pTexture;
	texture1D.format = format;
	texture1D.bindingFlags = bindFlags;
	return true;
}

bool lu::resources::CreateTexture2D(const RendererDevice& device, ResourceFormat format, const void* pData, uint32_t xSize, uint32_t ySize, uint32_t bindFlags, Texture2D& texture2D)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	D3D11_TEXTURE2D_DESC textureDesc = {};
	ID3D11Texture2D* pTexture = NULL;
	uint32_t binding = 0;

	if ((bindFlags & BindFlag::BIND_SHADER_RESOURCE) != 0)
		binding |= D3D11_BIND_SHADER_RESOURCE;
	if ((bindFlags & BindFlag::BIND_RENDER_TARGET) != 0)
		binding |= D3D11_BIND_RENDER_TARGET;
	if ((bindFlags & BindFlag::BIND_DEPTH_STENCIL) != 0)
		binding |= D3D11_BIND_DEPTH_STENCIL;
	if ((bindFlags & BindFlag::BIND_UNORDERED_ACCESS) != 0)
		binding |= D3D11_BIND_UNORDERED_ACCESS;

	textureDesc.Format = kResourceFormat[(uint32_t)format];
	textureDesc.BindFlags = (D3D11_BIND_FLAG)binding;
	textureDesc.ArraySize = 1;
	textureDesc.Width = xSize;
	textureDesc.Height = ySize;
	textureDesc.MipLevels = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;

	if (pData != NULL)
	{
		uint32_t pixelByteSize = GetResourceFormatBytesPerPixel(format);
		D3D11_SUBRESOURCE_DATA dataDesc = {};
		dataDesc.pSysMem = pData;
		dataDesc.SysMemPitch = xSize * pixelByteSize;
		dataDesc.SysMemSlicePitch = xSize * ySize * pixelByteSize;
		HRESULT result = pDevice->CreateTexture2D(&textureDesc, &dataDesc, &pTexture);
		if (result != S_OK)
		{
			char* pErrorMessage = (char*)alloca(512);
			sprintf_s(pErrorMessage, 512, "Error Creating Texture 2D: 0x%4X\n", result);
			OutputDebugStringA(pErrorMessage);
			return false;
		}
	}
	else
	{
		HRESULT result = pDevice->CreateTexture2D(&textureDesc, NULL, &pTexture);
		if (result != S_OK)
		{
			char* pErrorMessage = (char*)alloca(512);
			sprintf_s(pErrorMessage, 512, "Error Creating Texture 2D: 0x%4X\n", result);
			OutputDebugStringA(pErrorMessage);
			return false;
		}
	}
	
	texture2D.width = xSize;
	texture2D.height = ySize;
	texture2D.texture = (handle_t)pTexture;
	texture2D.format = format;
	texture2D.bindingFlags = bindFlags;
	return true;
}
bool lu::resources::CreateTexture3D(const RendererDevice& device, ResourceFormat format, const void* pData, uint32_t xSize, uint32_t ySize, uint32_t zSize, uint32_t bindFlags, Texture3D& texture3D)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	D3D11_TEXTURE3D_DESC textureDesc = {};
	ID3D11Texture3D* pTexture = NULL;
	uint32_t binding = 0;

	if ((bindFlags & BindFlag::BIND_SHADER_RESOURCE) != 0)
		binding |= D3D11_BIND_SHADER_RESOURCE;
	if ((bindFlags & BindFlag::BIND_RENDER_TARGET) != 0)
		binding |= D3D11_BIND_RENDER_TARGET;
	if ((bindFlags & BindFlag::BIND_DEPTH_STENCIL) != 0)
		binding |= D3D11_BIND_DEPTH_STENCIL;
	if ((bindFlags & BindFlag::BIND_UNORDERED_ACCESS) != 0)
		binding |= D3D11_BIND_UNORDERED_ACCESS;

	textureDesc.Format = kResourceFormat[(uint32_t)format];
	textureDesc.BindFlags = (D3D11_BIND_FLAG)binding;
	textureDesc.Width = xSize;
	textureDesc.Height = ySize;
	textureDesc.Depth = zSize;
	textureDesc.MipLevels = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (pData != NULL)
	{
		uint32_t pixelByteSize = GetResourceFormatBytesPerPixel(format);
		D3D11_SUBRESOURCE_DATA dataDesc = {};
		dataDesc.pSysMem = pData;
		dataDesc.SysMemPitch = xSize * pixelByteSize;
		dataDesc.SysMemSlicePitch = xSize * ySize * pixelByteSize;
		HRESULT result = pDevice->CreateTexture3D(&textureDesc, &dataDesc, &pTexture);
		if (result != S_OK)
		{
			char* pErrorMessage = (char*)alloca(512);
			sprintf_s(pErrorMessage, 512, "Error Creating Texture 3D: 0x%4X\n", result);
			OutputDebugStringA(pErrorMessage);
			return false;
		}
	}
	else
	{
		HRESULT result = pDevice->CreateTexture3D(&textureDesc, NULL, &pTexture);
		if (result != S_OK)
		{
			char* pErrorMessage = (char*)alloca(512);
			sprintf_s(pErrorMessage, 512, "Error Creating Texture 3D: 0x%4X\n", result);
			OutputDebugStringA(pErrorMessage);
			return false;
		}
	}

	texture3D.width = xSize;
	texture3D.height = ySize;
	texture3D.depth = zSize;
	texture3D.texture = (handle_t)pTexture;
	texture3D.format = format;
	texture3D.bindingFlags = bindFlags;
	return true;
}

bool lu::resources::ResizeTexture1D(const RendererDevice& device, uint32_t xSize, Texture1D& texture)
{
	if (texture.isValid())
	{
		DestroyTexture1D(device, texture);
	}
	return CreateTexture1D(device, texture.format, NULL, xSize, texture.bindingFlags, texture);
}

bool lu::resources::ResizeTexture2D(const RendererDevice& device, uint32_t xSize, uint32_t ySize, Texture2D& texture)
{
	uint32_t bindingFlags = texture.bindingFlags;
	lu::ResourceFormat format = texture.format;
	if (texture.isValid())
	{
		DestroyTexture2D(device, texture);
	}
	return CreateTexture2D(device, format, NULL, xSize, ySize, bindingFlags, texture);
}

bool lu::resources::ResizeTexture3D(const RendererDevice& device, uint32_t xSize, uint32_t ySize, uint32_t zSize, Texture3D& texture)
{
	if (texture.isValid())
	{
		DestroyTexture3D(device, texture);
	}
	return CreateTexture3D(device, texture.format, NULL, xSize, ySize, zSize, texture.bindingFlags, texture);
}

void lu::resources::DestroyTexture1D(const RendererDevice& device, Texture1D& texture1D)
{
	ID3D11Texture1D* pTexture = (ID3D11Texture1D*)texture1D.texture;
	texture1D.format = ResourceFormat::FORMAT_UNKNOWN;

	if (pTexture != NULL) pTexture->Release();
	texture1D.texture = LU_INVALID_HANDLE;
}

void lu::resources::DestroyTexture2D(const RendererDevice& device, Texture2D& texture2D)
{
	ID3D11Texture2D* pTexture = (ID3D11Texture2D*)texture2D.texture;
	texture2D.format = ResourceFormat::FORMAT_UNKNOWN;

	if (pTexture != NULL) pTexture->Release();
	texture2D.texture = LU_INVALID_HANDLE;
}

void lu::resources::DestroyTexture3D(const RendererDevice& device, Texture3D& texture3D)
{
	ID3D11Texture3D* pTexture = (ID3D11Texture3D*)texture3D.texture;
	texture3D.format = ResourceFormat::FORMAT_UNKNOWN;

	if (pTexture != NULL) pTexture->Release();
	texture3D.texture = LU_INVALID_HANDLE;
}
