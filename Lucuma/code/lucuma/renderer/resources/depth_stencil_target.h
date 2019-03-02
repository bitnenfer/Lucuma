#pragma once

#include "texture.h"
#include "../renderer.h"

namespace lu
{
	struct DepthStencilTarget
	{
		DepthStencilTarget() : pTexture(NULL), depthStencilTarget(LU_INVALID_HANDLE) {}
		bool isValid() { return depthStencilTarget != LU_INVALID_HANDLE; }
		const Texture* pTexture;
		handle_t depthStencilTarget;
	};

	namespace resources
	{
		bool CreateDepthStencilTarget(const RendererDevice& device, const Texture2D& renderTexture, DepthStencilTarget& depthStencilTarget);
		void DestroyDepthStencilTarget(const RendererDevice& device, DepthStencilTarget& depthStencilTarget);
	}
}
