#include "../../../states/sampler_state.h"
#include <d3d11.h>
#include <malloc.h>
#include <stdio.h>

static const D3D11_FILTER kSamplerFilterType[] = {
	D3D11_FILTER_MIN_MAG_MIP_LINEAR,
	D3D11_FILTER_MIN_MAG_MIP_POINT
};

static const D3D11_TEXTURE_ADDRESS_MODE kTextureAddressMode[] = {
	D3D11_TEXTURE_ADDRESS_WRAP,
	D3D11_TEXTURE_ADDRESS_MIRROR,
	D3D11_TEXTURE_ADDRESS_CLAMP
};

bool lu::states::CreateSamplerState(const RendererDevice& device, SamplerFilterType filter, TextureAddressMode textureAddressMode, SamplerState& sampler)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11SamplerState* pSamplerState = NULL;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = kSamplerFilterType[(uint32_t)filter];
	samplerDesc.AddressU = kTextureAddressMode[(uint32_t)textureAddressMode];
	samplerDesc.AddressV = kTextureAddressMode[(uint32_t)textureAddressMode];
	samplerDesc.AddressW = kTextureAddressMode[(uint32_t)textureAddressMode];
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = 0.0f;
	samplerDesc.MinLOD = 0.0f;

	HRESULT result = pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
	if (result != S_OK)
	{
		char* pErrorMessage = (char*)alloca(512);
		sprintf_s(pErrorMessage, 512, "Error Creating Render Target 1D: 0x%4X\n", result);
		OutputDebugStringA(pErrorMessage);
		return false;
	}

	sampler.sampler = (handle_t)pSamplerState;

	return true;
}

void lu::states::DestroySamplerState(const RendererDevice& device, SamplerState& sampler)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11SamplerState* pSamplerState = (ID3D11SamplerState*)sampler.sampler;
	sampler.sampler = LU_INVALID_HANDLE;
	if (pSamplerState != NULL)
	{
		pSamplerState->Release();
	}
}
