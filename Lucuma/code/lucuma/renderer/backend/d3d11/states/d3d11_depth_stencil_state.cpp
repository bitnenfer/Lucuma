#include "../../../states/depth_stencil_state.h"
#include <d3d11.h>
#include <malloc.h>
#include <stdio.h>

static D3D11_DEPTH_WRITE_MASK kDepthWriteMask[] =
{
	D3D11_DEPTH_WRITE_MASK_ZERO,
	D3D11_DEPTH_WRITE_MASK_ALL
};

static D3D11_COMPARISON_FUNC kComparisonFunc[] =
{
	D3D11_COMPARISON_NEVER,
	D3D11_COMPARISON_LESS,
	D3D11_COMPARISON_EQUAL,
	D3D11_COMPARISON_LESS_EQUAL,
	D3D11_COMPARISON_GREATER,
	D3D11_COMPARISON_NOT_EQUAL,
	D3D11_COMPARISON_GREATER_EQUAL,
	D3D11_COMPARISON_ALWAYS
};

static D3D11_STENCIL_OP kStencilOp[] =
{
	D3D11_STENCIL_OP_KEEP,
	D3D11_STENCIL_OP_ZERO,
	D3D11_STENCIL_OP_REPLACE,
	D3D11_STENCIL_OP_INCR_SAT,
	D3D11_STENCIL_OP_DECR_SAT,
	D3D11_STENCIL_OP_INVERT,
	D3D11_STENCIL_OP_INCR,
	D3D11_STENCIL_OP_DECR
};

lu::DepthState lu::DepthState::GetDefault(bool enabled)
{
	DepthState ds;
	ds.enabled = enabled;
	ds.writeMask = DepthWriteMask::ALL;
	ds.comparisonFunc = ComparisonFunc::LESS_EQUAL;
	return ds;
}

lu::DepthStencilOp lu::DepthStencilOp::GetDefault()
{
	DepthStencilOp dsop;
	dsop.stencilFail = StencilOp::KEEP;
	dsop.stencilDepthFail = StencilOp::KEEP;
	dsop.stencilPass = StencilOp::KEEP;
	dsop.stencilFunc = ComparisonFunc::ALWAYS;
	return dsop;
}

lu::StencilState lu::StencilState::GetDefault(bool enabled)
{
	StencilState ss;
	ss.enabled = enabled;
	ss.readMask = 0xFF;
	ss.writeMask = 0xFF;
	return ss;
}

bool lu::states::CreateDepthStencilState(const RendererDevice& device, const DepthState& depth, const StencilState& stencil, const DepthStencilOp& frontFacing, const DepthStencilOp& backFacing, DepthStencilState& depthStencilState)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	ID3D11DepthStencilState* pState = NULL;

	depthStencilDesc.DepthEnable = depth.enabled;
	depthStencilDesc.StencilEnable = stencil.enabled;

	if (depth.enabled)
	{
		depthStencilDesc.DepthFunc = kComparisonFunc[(uint32_t)depth.comparisonFunc];
		depthStencilDesc.DepthWriteMask = kDepthWriteMask[(uint32_t)depth.writeMask];
	}
	if (stencil.enabled)
	{
		depthStencilDesc.StencilReadMask = stencil.readMask;
		depthStencilDesc.StencilWriteMask = stencil.writeMask;
		depthStencilDesc.FrontFace.StencilFailOp = kStencilOp[(uint32_t)frontFacing.stencilFail];
		depthStencilDesc.FrontFace.StencilDepthFailOp = kStencilOp[(uint32_t)frontFacing.stencilDepthFail];
		depthStencilDesc.FrontFace.StencilPassOp = kStencilOp[(uint32_t)frontFacing.stencilPass];
		depthStencilDesc.FrontFace.StencilFunc = kComparisonFunc[(uint32_t)frontFacing.stencilFunc];
		depthStencilDesc.BackFace.StencilFailOp = kStencilOp[(uint32_t)backFacing.stencilFail];
		depthStencilDesc.BackFace.StencilDepthFailOp = kStencilOp[(uint32_t)backFacing.stencilDepthFail];
		depthStencilDesc.BackFace.StencilPassOp = kStencilOp[(uint32_t)backFacing.stencilPass];
		depthStencilDesc.BackFace.StencilFunc = kComparisonFunc[(uint32_t)backFacing.stencilFunc];
	}
	
	HRESULT result = pDevice->CreateDepthStencilState(&depthStencilDesc, &pState);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Depth Stencil State: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	depthStencilState.depthStencilState = (handle_t)pState;

	return true;
}

void lu::states::DestroyDepthStencilState(const RendererDevice& device, DepthStencilState& depthStencilState)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11DepthStencilState* pState = (ID3D11DepthStencilState*)depthStencilState.depthStencilState;
	depthStencilState.depthStencilState = LU_INVALID_HANDLE;
	if (pState != NULL)
	{
		pState->Release();
	}
}
