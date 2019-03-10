#include "imgui_renderer.h"
#include "../../input.h"

void inputCallback(lu::EventType type, lu::EventData data)
{
	ImGuiIO& io = ImGui::GetIO();
	if (type == lu::EventType::MOUSE_MOVE)
	{
		io.MousePos.x = data.point.x;
		io.MousePos.y = data.point.y;
	}
	else if (type == lu::EventType::MOUSE_BUTTON_DOWN)
	{
		io.MouseDown[data.button] = true;
	}
	else if (type == lu::EventType::MOUSE_BUTTON_UP)
	{
		io.MouseDown[data.button] = false;
	}
	else if (type == lu::EventType::MOUSE_WHEEL)
	{
		io.MouseWheel += data.delta;
	}
	else if (type == lu::EventType::KEY_DOWN)
	{
		io.KeysDown[data.key] = true;
	}
	else if (type == lu::EventType::KEY_UP)
	{
		io.KeysDown[data.key] = false;
	}
	else if (type == lu::EventType::INPUT_CHAR)
	{
		io.AddInputCharacter(data.inputChar);
	}
}

bool lu::ImGuiRenderPass::initialize(IAllocator& allocator, RendererDevice& device, const Window& window, RenderTarget* pMainRenderTarget)
{
	pRenderTarget = pMainRenderTarget;

	// shaders
	{
		VertexLayout vl;
		vl.addAttribute(ResourceFormat::FORMAT_R32G32_FLOAT, IM_OFFSETOF(ImDrawVert, pos));
		vl.addAttribute(ResourceFormat::FORMAT_R32G32_FLOAT, IM_OFFSETOF(ImDrawVert, uv));
		vl.addAttribute(ResourceFormat::FORMAT_R8G8B8A8_UNORM, IM_OFFSETOF(ImDrawVert, col));

		if (!resources::CreateVertexShaderFromSourceFile(allocator, device, "code/lucuma/utils/imgui/shaders/hlsl/dear_imgui.hlsl", "mainVS", vl, vertexShader))
			return false;

		if (!resources::CreatePixelShaderFromSourceFile(allocator, device, "code/lucuma/utils/imgui/shaders/hlsl/dear_imgui.hlsl", "mainPS", pixelShader))
			return false;
	}

	if (!states::CreateRasterizerState(device, FillMode::SOLID, CullMode::NONE, false, true, false, false, rasterizerState))
		return false;

	if (!states::CreateSamplerState(device, SamplerFilterType::POINT, TextureAddressMode::CLAMP, samplerState))
		return false;

	if (!states::CreateBlendState(device, RenderTargetBlend(true, BlendFunction::SRC_ALPHA, BlendFunction::INV_SRC_ALPHA, BlendOp::ADD, BlendFunction::ONE, BlendFunction::ZERO, BlendOp::ADD), blendState))
		return false;

	DepthState depth;
	StencilState stencil;
	depth.enabled = false;
	stencil.enabled = false;
	if (!states::CreateDepthStencilState(device, depth, stencil, DepthStencilOp::GetDefault(), DepthStencilOp::GetDefault(), depthStencilState))
		return false;

	return true;
}

void lu::ImGuiRenderPass::finalize(RendererDevice& device)
{
	resources::DestroyVertexShader(device, vertexShader);
	resources::DestroyPixelShader(device, pixelShader);
	states::DestroyBlendState(device, blendState);
	states::DestroyRasterizerState(device, rasterizerState);
	states::DestroySamplerState(device, samplerState);
	states::DestroyDepthStencilState(device, depthStencilState);
}

void lu::ImGuiRenderPass::commit(CommandList& cmds)
{
	float32_t blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	commands::SetBlendState(cmds, blendState, blendFactor, 0xFFFFFFFF);
	commands::SetVertexShader(cmds, &vertexShader);
	commands::SetPixelShader(cmds, &pixelShader);
	commands::SetGeometryShader(cmds, NULL);
	commands::SetRasterizerState(cmds, rasterizerState);
	commands::SetPixelShaderSamplers(cmds, 0, 1, &samplerState);
	commands::SetDepthStencilState(cmds, depthStencilState, 0);
	commands::SetRenderTargets(cmds, 1, pRenderTarget, NULL);
}

bool lu::ImGuiRenderer::initialize(IAllocator& allocator, RendererDevice& device, const Window& window, RenderTarget* pMainRenderTarget)
{
	if (!pipeline.initialize(allocator, device, window, pMainRenderTarget))
		return false;

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

	input::RegisterInputEventCallback(&inputCallback);

	constantBuffer.buffer = LU_INVALID_HANDLE;
	vertexBuffer.buffer = LU_INVALID_HANDLE;
	indexBuffer.buffer = LU_INVALID_HANDLE;
	fontTexture.texture = LU_INVALID_HANDLE;
	vertexBufferSize = 0;
	indexBufferSize = 0;

	// font
	{
		uint8_t* pPixels = NULL;
		int32_t width, height;
		io.Fonts->GetTexDataAsRGBA32(&pPixels, &width, &height, NULL);
		if (pPixels == NULL)
			return false;
		if (!resources::CreateTexture2D(device, ResourceFormat::FORMAT_R8G8B8A8_UNORM, pPixels, width, height, BindFlag::BIND_SHADER_RESOURCE, fontTexture))
			return false;
		if (!resources::CreateTexture2DShaderResourceView(device, fontTexture, 1, 0, fontTextureResourceView))
			return false;
		io.Fonts->TexID = &fontTextureResourceView;
	}

	// constant buffer
	if (!resources::CreateBuffer(device, sizeof(cbuffer), BufferUsage::USAGE_DYNAMIC, BindFlag::BIND_CONSTANT_BUFFER, CPUAccess::CPU_ACCESS_WRITE, 0, 0, NULL, constantBuffer))
		return false;

	return true;
}

void lu::ImGuiRenderer::finalize(RendererDevice& device)
{
	resources::DestroyShaderResourceView(device, fontTextureResourceView);
	resources::DestroyTexture2D(device, fontTexture);
	resources::DestroyBuffer(device, constantBuffer);
	resources::DestroyBuffer(device, vertexBuffer);
	resources::DestroyBuffer(device, indexBuffer);

	pipeline.finalize(device);
}

void lu::ImGuiRenderer::update(const Window& window)
{
	cbuffer.resolution.x = (float32_t)window.width;
	cbuffer.resolution.y = (float32_t)window.height;

	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize.x = cbuffer.resolution.x;
	io.DisplaySize.y = cbuffer.resolution.y;

	//io.DeltaTime = sue::app::GetFrameDelta() / 1000.0f;

	ImGui::NewFrame();
}

void lu::ImGuiRenderer::draw(CommandList& cmds, RendererDevice& device, RendererContext& context)
{
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	ImDrawData* pDrawData = ImGui::GetDrawData();
	ImVec2 pos = pDrawData->DisplayPos;
	uint64_t totalVerticesInBytes = (uint64_t)pDrawData->TotalVtxCount * sizeof(ImDrawVert);
	uint64_t totalIndicesInBytes = (uint64_t)pDrawData->TotalIdxCount * sizeof(uint16_t);

	if (totalVerticesInBytes == 0) return;

	if (vertexBufferSize < totalVerticesInBytes)
	{
		if (vertexBuffer.buffer != LU_INVALID_HANDLE)
			resources::DestroyBuffer(device, vertexBuffer);
		resources::CreateBuffer(device, (uint32_t)totalVerticesInBytes, BufferUsage::USAGE_DYNAMIC, BindFlag::BIND_VERTEX_BUFFER, CPUAccess::CPU_ACCESS_WRITE, ResourceType::RESOURCE_NONE, 0, NULL, vertexBuffer);
		vertexBufferSize = totalVerticesInBytes;
	}

	if (indexBufferSize < totalIndicesInBytes)
	{
		if (indexBuffer.buffer != LU_INVALID_HANDLE)
			resources::DestroyBuffer(device, indexBuffer);
		resources::CreateBuffer(device, (uint32_t)totalIndicesInBytes, BufferUsage::USAGE_DYNAMIC, BindFlag::BIND_INDEX_BUFFER, CPUAccess::CPU_ACCESS_WRITE, ResourceType::RESOURCE_NONE, 0, NULL, indexBuffer);
		indexBufferSize = totalIndicesInBytes;
	}

	// Buffer Mapping
	{
		void* pConstBuffer = resources::MapBuffer(context, constantBuffer, MapType::MAP_WRITE_DISCARD);
		memcpy(pConstBuffer, &cbuffer, sizeof(cbuffer));
		resources::UnmapBuffer(context, constantBuffer);

		ImDrawVert* pVertices = (ImDrawVert*)resources::MapBuffer(context, vertexBuffer, MapType::MAP_WRITE_DISCARD);
		ImDrawIdx* pIndices = (ImDrawIdx*)resources::MapBuffer(context, indexBuffer, MapType::MAP_WRITE_DISCARD);

		for (int32_t n = 0; n < pDrawData->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = pDrawData->CmdLists[n];
			memcpy(pVertices, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
			memcpy(pIndices, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
			pVertices += cmd_list->VtxBuffer.Size;
			pIndices += cmd_list->IdxBuffer.Size;
		}

		resources::UnmapBuffer(context, indexBuffer);
		resources::UnmapBuffer(context, vertexBuffer);
	}

	uint32_t strides = sizeof(ImDrawVert);
	uint32_t offsets = 0;
	Viewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = io.DisplaySize.x;
	viewport.height = io.DisplaySize.y;
	viewport.nearDepth = 0.0f;
	viewport.farDepth = 1.0f;

	pipeline.commit(cmds);

	commands::SetViewports(cmds, 1, &viewport);
	commands::SetVertexBuffers(cmds, 0, 1, &vertexBuffer, &strides, &offsets);
	commands::SetIndexBuffer(cmds, ResourceFormat::FORMAT_R16_UINT, indexBuffer);
	commands::SetVertexShaderConstantBuffers(cmds, 0, 1, &constantBuffer);
	commands::SetPrimitiveTopology(cmds, PrimitiveTopology::TRIANGLE_LIST);

	uint32_t indexOffset = 0;
	uint32_t vertexOffset = 0;

	for (int32_t n = 0; n < pDrawData->CmdListsCount; ++n)
	{
		const ImDrawList* pCmdList = pDrawData->CmdLists[n];
		for (int32_t i = 0; i < pCmdList->CmdBuffer.Size; ++i)
		{
			const ImDrawCmd* pCmd = &pCmdList->CmdBuffer[i];
			if (pCmd->UserCallback)
			{
				pCmd->UserCallback(pCmdList, pCmd);
			}
			else
			{
				ImVec4 clipRect = ImVec4(pCmd->ClipRect.x - pos.x, pCmd->ClipRect.y - pos.y, pCmd->ClipRect.z - pos.x, pCmd->ClipRect.w - pos.y);
				Scissor scissor = { (int32_t)clipRect.x, (int32_t)clipRect.y, (int32_t)clipRect.z, (int32_t)clipRect.w };
				ShaderResourceView* pShaderResourceView = (ShaderResourceView*)pCmd->TextureId;

				commands::SetScissors(cmds, 1, &scissor);
				commands::SetPixelShaderResourceViews(cmds, 0, 1, pShaderResourceView);
				commands::DrawIndexed(cmds, pCmd->ElemCount, indexOffset, vertexOffset);
			}
			indexOffset += pCmd->ElemCount;
		}
		vertexOffset += pCmdList->VtxBuffer.Size;
	}
}
