#pragma once

#include "../renderer.h"
#include "buffer.h"
#include "texture.h"
#include "format.h"

namespace lu
{
	struct UnorderedAccessView
	{
		UnorderedAccessView() : uav(LU_INVALID_HANDLE) {}
		bool isValid() { return uav != LU_INVALID_HANDLE; }
		handle_t uav;
	};

	namespace resources
	{
		bool CreateUnorderedAccessViewWithBuffer(const RendererDevice& device, const Buffer& buffer, bool structured, UnorderedAccessView& uav);
		bool CreateUnorderedAccessViewWithTexture1D(const RendererDevice& device, const Texture1D& texture, bool structured, UnorderedAccessView& uav);
		bool CreateUnorderedAccessViewWithTexture2D(const RendererDevice& device, const Texture2D& texture, bool structured, UnorderedAccessView& uav);
		bool CreateUnorderedAccessViewWithTexture3D(const RendererDevice& device, const Texture3D& texture, bool structured, UnorderedAccessView& uav);
		void DestroyUnorderedAccessView(const RendererDevice& device, UnorderedAccessView& uav);
	}
}
