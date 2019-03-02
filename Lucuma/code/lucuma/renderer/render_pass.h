#pragma once

#include "renderer.h"
#include "resources/command_list.h"
#include "../utils/allocator.h"

namespace lu
{
	struct IRenderPass
	{
		virtual bool initialize(lu::IAllocator& allocator, lu::RendererDevice& device, const lu::Window& window, lu::RenderTarget* pMainRenderTarget) = 0;
		virtual void finalize(lu::RendererDevice& device) = 0;
		virtual void commit(lu::CommandList& cmds) = 0;
	};
}
