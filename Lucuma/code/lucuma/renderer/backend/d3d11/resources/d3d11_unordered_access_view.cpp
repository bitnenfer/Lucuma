#include "../../../resources/unordered_access_view.h"
#include <d3d11.h>
#include <malloc.h>
#include <stdio.h>

extern DXGI_FORMAT kResourceFormat[];

bool lu::resources::CreateUnorderedAccessViewWithBuffer(const RendererDevice& device, const Buffer& buffer, bool structured, UnorderedAccessView& uav)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	ID3D11UnorderedAccessView* pView = NULL;
	D3D11_BUFFER_DESC bufferDesc;
	ID3D11Buffer* pBuff = (ID3D11Buffer*)buffer.buffer;

	pBuff->GetDesc(&bufferDesc);

	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;

	if ((bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS) != 0 && !structured)
	{
		uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		uavDesc.Buffer.NumElements = bufferDesc.ByteWidth / 4;
	}
	else
	{
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.Buffer.NumElements = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
	}

	HRESULT result = pDevice->CreateUnorderedAccessView((ID3D11Resource*)pBuff, &uavDesc, &pView);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating UAV: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}
	
	uav.uav = (handle_t)pView;
	return true;
}
bool lu::resources::CreateUnorderedAccessViewWithTexture1D(const RendererDevice& device, const Texture1D& texture, bool structured, UnorderedAccessView& uav)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	ID3D11UnorderedAccessView* pView = NULL;
	D3D11_TEXTURE1D_DESC texDesc;
	ID3D11Texture1D* pTex1D = (ID3D11Texture1D*)texture.texture;

	pTex1D->GetDesc(&texDesc);

	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
	uavDesc.Format = texDesc.Format;
	uavDesc.Texture1D.MipSlice = 0;

	HRESULT result = pDevice->CreateUnorderedAccessView((ID3D11Resource*)pTex1D, &uavDesc, &pView);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating UAV: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	uav.uav = (handle_t)pView;
	return true;
}
bool lu::resources::CreateUnorderedAccessViewWithTexture2D(const RendererDevice& device, const Texture2D& texture, bool structured, UnorderedAccessView& uav)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	ID3D11UnorderedAccessView* pView = NULL;
	D3D11_TEXTURE2D_DESC texDesc;
	ID3D11Texture2D* pTex2D = (ID3D11Texture2D*)texture.texture;

	pTex2D->GetDesc(&texDesc);

	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Format = texDesc.Format;
	uavDesc.Texture2D.MipSlice = 0;

	HRESULT result = pDevice->CreateUnorderedAccessView((ID3D11Resource*)pTex2D, &uavDesc, &pView);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating UAV: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	uav.uav = (handle_t)pView;
	return true;
}
bool lu::resources::CreateUnorderedAccessViewWithTexture3D(const RendererDevice& device, const Texture3D& texture, bool structured, UnorderedAccessView& uav)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	ID3D11UnorderedAccessView* pView = NULL;
	D3D11_TEXTURE3D_DESC texDesc;
	ID3D11Texture3D* pTex3D = (ID3D11Texture3D*)texture.texture;

	pTex3D->GetDesc(&texDesc);

	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
	uavDesc.Format = texDesc.Format;
	uavDesc.Texture2D.MipSlice = 0;

	HRESULT result = pDevice->CreateUnorderedAccessView((ID3D11Resource*)pTex3D, &uavDesc, &pView);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating UAV: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	uav.uav = (handle_t)pView;
	return true;
}
void lu::resources::DestroyUnorderedAccessView(const RendererDevice& device, UnorderedAccessView& uav)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11UnorderedAccessView* pView = (ID3D11UnorderedAccessView*)uav.uav;
	uav.uav = LU_INVALID_HANDLE;
	if (pView != NULL)
	{
		pView->Release();
	}
}
