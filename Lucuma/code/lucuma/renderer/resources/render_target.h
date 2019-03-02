#pragma once

#include "texture.h"
#include "../renderer.h"

namespace lu
{
	struct Viewport
	{
		float32_t x, y, width, height, nearDepth, farDepth;
	};

	struct Scissor
	{
		int32_t x, y, width, height;
	};

	struct RenderTarget
	{
		RenderTarget() : pTexture(NULL), renderTarget(LU_INVALID_HANDLE) {}
		bool isValid() { return renderTarget != LU_INVALID_HANDLE; }
		const Texture* pTexture;
		handle_t renderTarget;
	};

	namespace resources
	{
		bool CreateRenderTarget1D(const RendererDevice& device, const Texture1D& renderTexture, RenderTarget& renderTarget);
		bool CreateRenderTarget2D(const RendererDevice& device, const Texture2D& renderTexture, RenderTarget& renderTarget);
		bool CreateRenderTarget3D(const RendererDevice& device, const Texture3D& renderTexture, RenderTarget& renderTarget);
		void DestroyRenderTarget(const RendererDevice& device, RenderTarget& renderTarget);
	}
}