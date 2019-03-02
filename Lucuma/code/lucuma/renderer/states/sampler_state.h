#pragma once

#include "../../types.h"
#include "../renderer.h"

namespace lu
{
	enum class SamplerFilterType
	{
		LINEAR,
		POINT
	};

	enum class TextureAddressMode
	{
		WRAP,
		MIRROR,
		CLAMP
	};

	struct SamplerState
	{
		SamplerState() : sampler(LU_INVALID_HANDLE) {}
		bool isValid() { return sampler != LU_INVALID_HANDLE; }
		handle_t sampler;
	};

	namespace states
	{
		bool CreateSamplerState(const RendererDevice& device, SamplerFilterType filter, TextureAddressMode textureAddressMode, SamplerState& sampler);
		void DestroySamplerState(const RendererDevice& device, SamplerState& sampler);
	}
}
