#include "../../../resources/render_target.h"
#include <d3d11.h>
#include <malloc.h>
#include <stdio.h>

extern DXGI_FORMAT kResourceFormat[];

bool lu::resources::CreateRenderTarget1D(const RendererDevice& device, const Texture1D& renderTexture, RenderTarget& renderTarget)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11RenderTargetView* pRenderTargetView = NULL;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	renderTargetViewDesc.Format = kResourceFormat[(uint32_t)renderTexture.format];
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
	renderTargetViewDesc.Texture1D.MipSlice = 0;

	HRESULT result = pDevice->CreateRenderTargetView((ID3D11Resource*)renderTexture.texture, &renderTargetViewDesc, &pRenderTargetView);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Render Target 1D: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	renderTarget.renderTarget = (handle_t)pRenderTargetView;
	renderTarget.pTexture = &renderTexture;
	return true;
}
bool lu::resources::CreateRenderTarget2D(const RendererDevice& device, const Texture2D& renderTexture, RenderTarget& renderTarget)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11RenderTargetView* pRenderTargetView = NULL;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	renderTargetViewDesc.Format = kResourceFormat[(uint32_t)renderTexture.format];
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	HRESULT result = pDevice->CreateRenderTargetView((ID3D11Resource*)renderTexture.texture, &renderTargetViewDesc, &pRenderTargetView);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Render Target 1D: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	renderTarget.renderTarget = (handle_t)pRenderTargetView;
	renderTarget.pTexture = &renderTexture;
	return true;
}
bool lu::resources::CreateRenderTarget3D(const RendererDevice& device, const Texture3D& renderTexture, RenderTarget& renderTarget)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11RenderTargetView* pRenderTargetView = NULL;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
	renderTargetViewDesc.Format = kResourceFormat[(uint32_t)renderTexture.format];
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
	renderTargetViewDesc.Texture3D.MipSlice = 0;
	renderTargetViewDesc.Texture3D.FirstWSlice = 0;
	renderTargetViewDesc.Texture3D.WSize = 0;

	HRESULT result = pDevice->CreateRenderTargetView((ID3D11Resource*)renderTexture.texture, &renderTargetViewDesc, &pRenderTargetView);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Render Target 1D: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	renderTarget.renderTarget = (handle_t)pRenderTargetView;
	renderTarget.pTexture = &renderTexture;
	return true;
}
void lu::resources::DestroyRenderTarget(const RendererDevice& device, RenderTarget& renderTarget)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11RenderTargetView* pRenderTargetView = (ID3D11RenderTargetView*)renderTarget.renderTarget;
	if (pRenderTargetView != NULL)
	{
		pRenderTargetView->Release();
	}
	renderTarget.renderTarget = LU_INVALID_HANDLE;
}
