#pragma once

#include "../renderer.h"

namespace lu
{
	enum class DepthWriteMask
	{
		ZERO,
		ALL
	};

	enum class ComparisonFunc
	{
		NEVER,
		LESS,
		EQUAL,
		LESS_EQUAL,
		GREATER,
		NOT_EQUAL,
		GREATER_EQUAL,
		ALWAYS
	};

	enum class StencilOp
	{
		KEEP,
		ZERO,
		REPLACE,
		INCR_SAT,
		DECR_SAT,
		INVERT,
		INCR,
		DECR
	};

	struct DepthStencilState
	{
		DepthStencilState() : depthStencilState(LU_INVALID_HANDLE) {}
		bool isValid() { return depthStencilState != LU_INVALID_HANDLE; }
		handle_t depthStencilState;
	};

	struct DepthState
	{
		static DepthState GetDefault(bool enabled = true);
		bool enabled;
		DepthWriteMask writeMask;
		ComparisonFunc comparisonFunc;
	};

	struct StencilState
	{
		static StencilState GetDefault(bool enabled = true);
		bool enabled;
		uint8_t readMask;
		uint8_t writeMask;
	};

	struct DepthStencilOp
	{
		static DepthStencilOp GetDefault();
		StencilOp stencilFail;
		StencilOp stencilDepthFail;
		StencilOp stencilPass;
		ComparisonFunc stencilFunc;
	};

	namespace states
	{
		bool CreateDepthStencilState(const RendererDevice& device, const DepthState& depth, const StencilState& stencil, const DepthStencilOp& frontFacing, const DepthStencilOp& backFacing, DepthStencilState& depthStencilState);
		void DestroyDepthStencilState(const RendererDevice& device, DepthStencilState& depthStencilState);
	}
}