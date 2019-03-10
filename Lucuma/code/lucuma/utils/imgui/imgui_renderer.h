#pragma once

#include "../../renderer/renderer.h"
#include "../../renderer/resources/command_list.h"
#include "../../renderer/render_pass.h"
#include "imgui/imgui.h"

namespace lu
{
	struct ImGuiRenderPass : public IRenderPass
	{
		virtual bool initialize(IAllocator& allocator, RendererDevice& device, const Window& window, RenderTarget* pMainRenderTarget) override;
		virtual void finalize(RendererDevice& device) override;
		virtual void commit(CommandList& cmds) override;

		void update(const Window& window);

		VertexShader vertexShader;
		PixelShader pixelShader;
		RasterizerState rasterizerState;
		BlendState blendState;
		SamplerState samplerState;
		DepthStencilState depthStencilState;
		RenderTarget* pRenderTarget;
	};

	struct ImGuiRenderer
	{
		bool initialize(IAllocator& allocator, RendererDevice& device, const Window& window, RenderTarget* pMainRenderTarget);
		void finalize(RendererDevice& device);
		void update(const Window& window);
		void draw(CommandList& cmds, RendererDevice& device, RendererContext& context);
		
		ImGuiRenderPass pipeline;

		struct
		{
			Vec4 resolution;
		} cbuffer;

		uint64_t vertexBufferSize;
		uint64_t indexBufferSize;
		Buffer vertexBuffer;
		Buffer indexBuffer;
		Buffer constantBuffer;
		Texture2D fontTexture;
		ShaderResourceView fontTextureResourceView;
	};
}