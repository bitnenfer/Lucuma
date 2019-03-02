#include "../../../resources/depth_stencil_target.h"
#include <d3d11.h>
#include <malloc.h>
#include <stdio.h>

bool lu::resources::CreateDepthStencilTarget(const RendererDevice& device, const Texture2D& renderTexture, DepthStencilTarget& depthStencilTarget)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11DepthStencilView* pDepthStencilView = NULL;
	D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc = {};

	viewDesc.Texture2D.MipSlice = 0;
	viewDesc.Format = (DXGI_FORMAT)renderTexture.format;
	viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Flags = 0;

	HRESULT result = pDevice->CreateDepthStencilView((ID3D11Resource*)renderTexture.texture, &viewDesc, &pDepthStencilView);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Depth Stencil Target: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	depthStencilTarget.depthStencilTarget = (handle_t)pDepthStencilView;
	depthStencilTarget.pTexture = &renderTexture;

	return true;
}

void lu::resources::DestroyDepthStencilTarget(const RendererDevice& device, DepthStencilTarget& depthStencilTarget)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11DepthStencilView* pDepthStencilView = (ID3D11DepthStencilView*)depthStencilTarget.depthStencilTarget;
	depthStencilTarget.depthStencilTarget = LU_INVALID_HANDLE;
	depthStencilTarget.pTexture = NULL;
	if (pDepthStencilView != NULL)
	{
		pDepthStencilView->Release();
	}
}
