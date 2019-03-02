#pragma once

#include "../../types.h"
#include "../renderer.h"

namespace lu
{
	enum class BlendFunction
	{
		ZERO,
		ONE,
		SRC_COLOR,
		INV_SRC_COLOR,
		SRC_ALPHA,
		INV_SRC_ALPHA,
		DEST_ALPHA,
		INV_DEST_ALPHA,
		DEST_COLOR,
		INV_DEST_COLOR,
		SRC_ALPHA_SAT,
		BLEND_FACTOR,
		INV_BLEND_FACTOR,
		SRC1_COLOR,
		INV_SRC1_COLOR,
		SRC1_ALPHA,
		INV_SRC1_ALPHA
	};

	enum class BlendOp
	{
		ADD,
		SUBTRACT,
		REV_SUBTRACT,
		MIN,
		MAX
	};

	struct RenderTargetBlend
	{
		RenderTargetBlend(
			bool enabled,
			BlendFunction srcRGB,
			BlendFunction dstRGB,
			BlendOp opRGB,
			BlendFunction srcAlpha,
			BlendFunction dstAlpha,
			BlendOp opAlpha
		) :
			enabled(enabled),
			srcRGB(srcRGB),
			dstRGB(dstRGB),
			opRGB(opRGB),
			srcAlpha(srcAlpha),
			dstAlpha(dstAlpha),
			opAlpha(opAlpha)
		{}

		void setState(bool enabled) { this->enabled = enabled; }

		void setBlendFunction(BlendFunction src, BlendFunction dst)
		{
			srcRGB = src;
			srcAlpha = src;
			dstRGB = dst;
			dstAlpha = dst;
		}
		void setBlendOp(BlendOp op)
		{
			opRGB = op;
			opAlpha = op;
		}

		void setBlendFunctionSeparate(BlendFunction srcRGB, BlendFunction dstRGB, BlendFunction srcAlpha, BlendFunction dstAlpha)
		{
			this->srcRGB = srcRGB;
			this->srcAlpha = srcAlpha;
			this->dstRGB = dstRGB;
			this->dstAlpha = dstAlpha;
		}
		void setBlendOpSeparate(BlendOp opRGB, BlendOp opAlpha)
		{
			this->opRGB = opRGB;
			this->opAlpha = opAlpha;
		}

		bool enabled;
		BlendFunction srcRGB;
		BlendFunction dstRGB;
		BlendOp opRGB;
		BlendFunction srcAlpha;
		BlendFunction dstAlpha;
		BlendOp opAlpha;
	};

	struct BlendState
	{
		BlendState() : blend(LU_INVALID_HANDLE) {}
		bool isValid() { return blend != LU_INVALID_HANDLE; }
		handle_t blend;
	};

	namespace states
	{
		bool CreateBlendState(const RendererDevice& device, const RenderTargetBlend& renderTargetBlend, BlendState& blend);
		bool CreateIndependentBlend(const RendererDevice& device, const RenderTargetBlend* pRenderTargetBlendArray, uint32_t count, BlendState& blend);
		void DestroyBlendState(const RendererDevice& device, BlendState& blend);
	}
}