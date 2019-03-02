#include "../../../states/rasterizer_state.h"
#include <d3d11.h>
#include <malloc.h>
#include <stdio.h>

static const D3D11_FILL_MODE kFillMode[] = {
	D3D11_FILL_WIREFRAME,
	D3D11_FILL_SOLID
};

static const D3D11_CULL_MODE kCullMode[] = {
	D3D11_CULL_NONE,
	D3D11_CULL_BACK,
	D3D11_CULL_FRONT
};

bool lu::states::CreateRasterizerState(const RendererDevice& device, FillMode fillMode, CullMode cullMode, bool frontCounterClockwise, bool scissorEnabled, bool multisampleEnabled, bool depthClipEnabled, RasterizerState& rasterizer)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	ID3D11RasterizerState* pRasterizerState = NULL;

	rasterizerDesc.FillMode = kFillMode[(uint32_t)fillMode];
	rasterizerDesc.CullMode = kCullMode[(uint32_t)cullMode];
	rasterizerDesc.FrontCounterClockwise = frontCounterClockwise;
	rasterizerDesc.ScissorEnable = scissorEnabled;
	rasterizerDesc.MultisampleEnable = multisampleEnabled;
	rasterizerDesc.DepthClipEnable = depthClipEnabled;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.AntialiasedLineEnable = FALSE;

	HRESULT result = pDevice->CreateRasterizerState(&rasterizerDesc, &pRasterizerState);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Rasterizer: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	rasterizer.rasterizer = (handle_t)pRasterizerState;

	return true;
}

void lu::states::DestroyRasterizerState(const RendererDevice& device, RasterizerState& rasterizer)
{
	ID3D11RasterizerState* pRasterizerState = (ID3D11RasterizerState*)rasterizer.rasterizer;
	rasterizer.rasterizer = LU_INVALID_HANDLE;
	if (pRasterizerState != NULL)
	{
		pRasterizerState->Release();
	}
}
