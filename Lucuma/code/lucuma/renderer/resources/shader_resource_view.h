#pragma once

#include "../../types.h"
#include "../renderer.h"
#include "../resources/texture.h"
#include "../resources/buffer.h"

namespace lu
{
	struct ShaderResourceView
	{
		ShaderResourceView() : shaderResourceView(LU_INVALID_HANDLE) {}
		bool isValid() { return shaderResourceView != LU_INVALID_HANDLE; }
		handle_t shaderResourceView;
	};

	namespace resources
	{
		bool CreateStructuredBufferShaderResourceView(RendererDevice& device, Buffer& buffer, ResourceFormat format, uint32_t firstElement, uint32_t numOfElements, ShaderResourceView& shaderResourceView);
		bool CreateTexture1DShaderResourceView(RendererDevice& device, Texture1D& texture, uint32_t mipLevels, uint32_t mostDetailedMip, ShaderResourceView& shaderResourceView);
		bool CreateTexture2DShaderResourceView(RendererDevice& device, Texture2D& texture, uint32_t mipLevels, uint32_t mostDetailedMip, ShaderResourceView& shaderResourceView);
		bool CreateTexture3DShaderResourceView(RendererDevice& device, Texture3D& texture, uint32_t mipLevels, uint32_t mostDetailedMip, ShaderResourceView& shaderResourceView);
		void DestroyShaderResourceView(RendererDevice& device, ShaderResourceView& shaderResourceView);
	}
}