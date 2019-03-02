#include "../../../states/blend_state.h"
#include <d3d11.h>
#include <malloc.h>
#include <stdio.h>

static const D3D11_BLEND kBlendFunction [] = 
{
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
	D3D11_BLEND_SRC_COLOR,
	D3D11_BLEND_INV_SRC_COLOR,
	D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_INV_DEST_ALPHA,
	D3D11_BLEND_DEST_COLOR,
	D3D11_BLEND_INV_DEST_COLOR,
	D3D11_BLEND_SRC_ALPHA_SAT,
	D3D11_BLEND_BLEND_FACTOR,
	D3D11_BLEND_INV_BLEND_FACTOR,
	D3D11_BLEND_SRC1_COLOR,
	D3D11_BLEND_INV_SRC1_COLOR,
	D3D11_BLEND_SRC1_ALPHA,
	D3D11_BLEND_INV_SRC1_ALPHA
};

static const D3D11_BLEND_OP kBlendOp[] = 
{
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_OP_SUBTRACT,
	D3D11_BLEND_OP_REV_SUBTRACT,
	D3D11_BLEND_OP_MIN,
	D3D11_BLEND_OP_MAX
};

bool lu::states::CreateBlendState(const RendererDevice& device, const RenderTargetBlend& renderTargetBlend, BlendState& blend)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11BlendState* pBlendState = NULL;
	D3D11_BLEND_DESC blendDesc = {};

	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = renderTargetBlend.enabled;
	blendDesc.RenderTarget[0].SrcBlend = kBlendFunction[(uint32_t)renderTargetBlend.srcRGB];
	blendDesc.RenderTarget[0].DestBlend = kBlendFunction[(uint32_t)renderTargetBlend.dstRGB];
	blendDesc.RenderTarget[0].SrcBlendAlpha = kBlendFunction[(uint32_t)renderTargetBlend.srcAlpha];
	blendDesc.RenderTarget[0].DestBlendAlpha = kBlendFunction[(uint32_t)renderTargetBlend.dstAlpha];
	blendDesc.RenderTarget[0].BlendOp = kBlendOp[(uint32_t)renderTargetBlend.opRGB];
	blendDesc.RenderTarget[0].BlendOpAlpha = kBlendOp[(uint32_t)renderTargetBlend.opAlpha];
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT result = pDevice->CreateBlendState(&blendDesc, &pBlendState);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Blend: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	blend.blend = (handle_t)pBlendState;

	return true;
}

bool lu::states::CreateIndependentBlend(const RendererDevice& device, const RenderTargetBlend* pRenderTargetBlendArray, uint32_t count, BlendState& blend)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11BlendState* pBlendState = NULL;
	D3D11_BLEND_DESC blendDesc = {};

	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = TRUE;

	for (uint32_t index = 0; index < count; ++index)
	{
		const RenderTargetBlend& renderTargetBlend = pRenderTargetBlendArray[index];
		blendDesc.RenderTarget[index].BlendEnable = renderTargetBlend.enabled;
		blendDesc.RenderTarget[index].SrcBlend = kBlendFunction[(uint32_t)renderTargetBlend.srcRGB];
		blendDesc.RenderTarget[index].DestBlend = kBlendFunction[(uint32_t)renderTargetBlend.dstRGB];
		blendDesc.RenderTarget[index].SrcBlendAlpha = kBlendFunction[(uint32_t)renderTargetBlend.srcAlpha];
		blendDesc.RenderTarget[index].DestBlendAlpha = kBlendFunction[(uint32_t)renderTargetBlend.dstAlpha];
		blendDesc.RenderTarget[index].BlendOp = kBlendOp[(uint32_t)renderTargetBlend.opRGB];
		blendDesc.RenderTarget[index].BlendOpAlpha = kBlendOp[(uint32_t)renderTargetBlend.opAlpha];
		blendDesc.RenderTarget[index].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	HRESULT result = pDevice->CreateBlendState(&blendDesc, &pBlendState);

	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Blend: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	blend.blend = (handle_t)pBlendState;

	return true;
}

void lu::states::DestroyBlendState(const RendererDevice& device, BlendState& blend)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11BlendState* pBlendState = (ID3D11BlendState*)blend.blend;
	blend.blend = LU_INVALID_HANDLE;
	if (pBlendState != NULL) pBlendState->Release();
}
