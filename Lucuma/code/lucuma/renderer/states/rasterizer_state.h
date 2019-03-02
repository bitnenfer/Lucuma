#pragma once

#include "../../types.h"
#include "../renderer.h"

namespace lu
{
	enum class FillMode
	{
		WIREFRAME,
		SOLID
	};

	enum class CullMode
	{
		NONE,
		BACK,
		FRONT
	};

	struct RasterizerState
	{
		RasterizerState() : rasterizer(LU_INVALID_HANDLE) {}
		bool isValid() { return rasterizer != LU_INVALID_HANDLE; }
		handle_t rasterizer;
	};

	namespace states
	{
		bool CreateRasterizerState(const RendererDevice& device, FillMode fillMode, CullMode cullMode, bool frontCounterClockwise, bool scissorEnabled, bool multisampleEnabled, bool depthClipEnabled, RasterizerState& rasterizer);
		void DestroyRasterizerState(const RendererDevice& device, RasterizerState& rasterizer);
	}
}
