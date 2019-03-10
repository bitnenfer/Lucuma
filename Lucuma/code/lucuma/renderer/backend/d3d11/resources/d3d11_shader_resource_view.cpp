#include "../../../resources/shader_resource_view.h"
#include <d3d11.h>
#include <malloc.h>
#include <stdio.h>

extern DXGI_FORMAT kResourceFormat[];

bool lu::resources::CreateStructuredBufferShaderResourceView(RendererDevice& device, Buffer& buffer, ResourceFormat format, uint32_t firstElement, uint32_t numOfElements, ShaderResourceView& shaderResourceView)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11ShaderResourceView* pShaderResourceView = NULL;
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));

	viewDesc.Format = kResourceFormat[(uint32_t)format];
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	viewDesc.BufferEx.FirstElement = firstElement;
	viewDesc.BufferEx.NumElements = numOfElements;
	viewDesc.BufferEx.Flags = 0;

	HRESULT result = pDevice->CreateShaderResourceView((ID3D11Resource*)buffer.buffer, &viewDesc, &pShaderResourceView);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Resource View: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	shaderResourceView.shaderResourceView = pShaderResourceView;
	return true;
}

bool lu::resources::CreateTexture1DShaderResourceView(RendererDevice& device, Texture1D& texture, uint32_t mipLevels, uint32_t mostDetailedMip, ShaderResourceView& shaderResourceView)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11ShaderResourceView* pShaderResourceView = NULL;
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));

	viewDesc.Format = kResourceFormat[(uint32_t)texture.format];
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = mipLevels;
	viewDesc.Texture1D.MostDetailedMip = mostDetailedMip;

	HRESULT result = pDevice->CreateShaderResourceView((ID3D11Resource*)texture.texture, &viewDesc, &pShaderResourceView);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Resource View: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	shaderResourceView.shaderResourceView = pShaderResourceView;
	return true;
}

bool lu::resources::CreateTexture2DShaderResourceView(RendererDevice& device, Texture2D& texture, uint32_t mipLevels, uint32_t mostDetailedMip, ShaderResourceView& shaderResourceView)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11ShaderResourceView* pShaderResourceView = NULL;
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));

	viewDesc.Format = kResourceFormat[(uint32_t)texture.format];
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture1D.MipLevels = mipLevels;
	viewDesc.Texture1D.MostDetailedMip = mostDetailedMip;

	HRESULT result = pDevice->CreateShaderResourceView((ID3D11Resource*)texture.texture, &viewDesc, &pShaderResourceView);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Resource View: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	shaderResourceView.shaderResourceView = pShaderResourceView;
	return true;
}

bool lu::resources::CreateTexture3DShaderResourceView(RendererDevice& device, Texture3D& texture, uint32_t mipLevels, uint32_t mostDetailedMip, ShaderResourceView& shaderResourceView)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11ShaderResourceView* pShaderResourceView = NULL;
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(viewDesc));

	viewDesc.Format = kResourceFormat[(uint32_t)texture.format];
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
	viewDesc.Texture1D.MipLevels = mipLevels;
	viewDesc.Texture1D.MostDetailedMip = mostDetailedMip;

	HRESULT result = pDevice->CreateShaderResourceView((ID3D11Resource*)texture.texture, &viewDesc, &pShaderResourceView);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Resource View: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	shaderResourceView.shaderResourceView = pShaderResourceView;
	return true;
}

void lu::resources::DestroyShaderResourceView(RendererDevice& device, ShaderResourceView& shaderResourceView)
{
	if (shaderResourceView.isValid())
	{
		ID3D11ShaderResourceView* pShaderResourceView = (ID3D11ShaderResourceView*)shaderResourceView.shaderResourceView;
		pShaderResourceView->Release();
		shaderResourceView.shaderResourceView = LU_INVALID_HANDLE;
	}
}

