#pragma once

#include "format.h"
#include "../renderer.h"

// TODO: Added more features to textures like mip levels

namespace lu
{
	enum class TextureType
	{
		TEXTURE_1D,
		TEXTURE_2D,
		TEXTURE_3D
	};
	
	struct Texture
	{
		Texture() : texture(LU_INVALID_HANDLE), format(ResourceFormat::FORMAT_UNKNOWN) {}
		bool isValid() { return texture != LU_INVALID_HANDLE; }

		handle_t texture;
		ResourceFormat format;
		uint32_t bindingFlags;
	};

	struct Texture1D : public Texture
	{
		uint32_t width;
	};

	struct Texture2D : public Texture
	{
		uint32_t width, height;
	};

	struct Texture3D : public Texture
	{
		uint32_t width, height, depth;
	};

	namespace resources
	{
		bool CreateTexture1D(const RendererDevice& device, ResourceFormat format, const void* pData, uint32_t xSize, uint32_t bindingFlags, Texture1D& texture);
		bool CreateTexture2D(const RendererDevice& device, ResourceFormat format, const void* pData, uint32_t xSize, uint32_t ySize, uint32_t bindingFlags, Texture2D& texture);
		bool CreateTexture3D(const RendererDevice& device, ResourceFormat format, const void* pData, uint32_t xSize, uint32_t ySize, uint32_t zSize, uint32_t bindingFlags, Texture3D& texture);
		bool ResizeTexture1D(const RendererDevice& device, uint32_t xSize, Texture1D& texture);
		bool ResizeTexture2D(const RendererDevice& device, uint32_t xSize, uint32_t ySize, Texture2D& texture);
		bool ResizeTexture3D(const RendererDevice& device, uint32_t xSize, uint32_t ySize, uint32_t zSize, Texture3D& texture);
		void DestroyTexture1D(const RendererDevice& device, Texture1D& texture);
		void DestroyTexture2D(const RendererDevice& device, Texture2D& texture);
		void DestroyTexture3D(const RendererDevice& device, Texture3D& texture);
	}
}
