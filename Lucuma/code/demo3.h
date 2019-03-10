#pragma once

#include "lucuma/lucuma.h"

void demo3()
{
	lu::PageAllocator pageAllocator;
	lu::Window window;
	lu::RendererDevice device;
	lu::RendererContext context;
	lu::CommandList commandList;
	lu::RenderTarget mainRenderTarget;
	lu::UnorderedAccessView uav;
	lu::Texture2D texture;
	lu::ShaderResourceView textureSRV;
	lu::Buffer uavBuffer;
	lu::Buffer vertexBuffer;
	lu::Buffer resolutionCB;
	lu::Buffer dynamicCB;
	lu::ComputeShader computeShader;
	lu::VertexShader quadVertexShader;
	lu::PixelShader quadPixelShader;
	lu::VertexLayout quadVertexLayout;
	lu::SamplerState samplerState;
	lu::RasterizerState rasterizerState;
	lu::CreateWindowDesc windowDesc;
	bool run = true;

	float32_t vertices[] = {
		-1.0f, +1.0f, +0.0f,		0.0f, 0.0f,
		-1.0f, -1.0f, +0.0f,		0.0f, 1.0f,
		+1.0f, -1.0f, +0.0f,		1.0f, 1.0f,
		-1.0f, +1.0f, +0.0f,		0.0f, 0.0f,
		+1.0f, -1.0f, +0.0f,		1.0f, 1.0f,
		+1.0f, +1.0f, +0.0f,		1.0f, 0.0f
	};
		
	struct ResolutionBuffer
	{
		uint32_t uResolution[4];
		float32_t fResolution[4];
	} resolutionBuffer;

	lu::Vec4 dynamicBuffer;

	windowDesc.width = 1540;
	windowDesc.height = 800;
	windowDesc.pTitle = "Demo 3";
	windowDesc.style = lu::WINDOW_RESIZE | lu::WINDOW_BUTTON_MAXIMIZE;

	quadVertexLayout.addAttribute(lu::ResourceFormat::FORMAT_R32G32B32_FLOAT, 0);
	quadVertexLayout.addAttribute(lu::ResourceFormat::FORMAT_R32G32_FLOAT, sizeof(float32_t)*3);

#if 0
	LU_ASSERT(lu::renderer::CreateWindowAndInitializeGraphicsAPIAlsoWaitForRenderDoc(windowDesc, window, device, context, mainRenderTarget));
#else
	LU_ASSERT(lu::renderer::CreateWindowAndInitializeGraphicsAPI(windowDesc, window, device, context, mainRenderTarget));
#endif
	LU_ASSERT(lu::commands::CreateCommandList(pageAllocator, 100, commandList));
	LU_ASSERT(lu::resources::CreateTexture2D(device, lu::ResourceFormat::FORMAT_R8G8B8A8_UNORM, NULL, window.width, window.height, lu::BindFlag::BIND_SHADER_RESOURCE | lu::BindFlag::BIND_UNORDERED_ACCESS, texture));
	LU_ASSERT(lu::resources::CreateUnorderedAccessViewWithTexture2D(device, texture, true, uav));
	LU_ASSERT(lu::resources::CreateComputeShaderFromSourceFile(pageAllocator, device, "code/shaders/hlsl/compute.hlsl", "mainCS", computeShader));
	LU_ASSERT(lu::resources::CreateVertexShaderFromSourceFile(pageAllocator, device, "code/shaders/hlsl/quad.hlsl", "quadVS", quadVertexLayout, quadVertexShader));
	LU_ASSERT(lu::resources::CreatePixelShaderFromSourceFile(pageAllocator, device, "code/shaders/hlsl/quad.hlsl", "quadPS", quadPixelShader));
	LU_ASSERT(lu::resources::CreateBuffer(device, sizeof(vertices), lu::BufferUsage::USAGE_IMMUTABLE, lu::BIND_VERTEX_BUFFER, lu::CPU_ACCESS_NONE, 0, 0, vertices, vertexBuffer));
	LU_ASSERT(lu::resources::CreateTexture2DShaderResourceView(device, texture, 1, 0, textureSRV));
	LU_ASSERT(lu::states::CreateRasterizerState(device, lu::FillMode::SOLID, lu::CullMode::NONE, false, false, false, false, rasterizerState));
	LU_ASSERT(lu::states::CreateSamplerState(device, lu::SamplerFilterType::LINEAR, lu::TextureAddressMode::WRAP, samplerState));

	resolutionBuffer.uResolution[0] = window.width;
	resolutionBuffer.uResolution[1] = window.height;
	resolutionBuffer.fResolution[0] = (float32_t)window.width;
	resolutionBuffer.fResolution[1] = (float32_t)window.height;
	dynamicBuffer.x = 0.0f;

	LU_ASSERT(lu::resources::CreateBuffer(device, sizeof(resolutionBuffer), lu::BufferUsage::USAGE_DYNAMIC, lu::BIND_CONSTANT_BUFFER, lu::CPU_ACCESS_WRITE, 0, 0, &resolutionBuffer, resolutionCB));
	LU_ASSERT(lu::resources::CreateBuffer(device, sizeof(dynamicBuffer), lu::BufferUsage::USAGE_DYNAMIC, lu::BIND_CONSTANT_BUFFER, lu::CPU_ACCESS_WRITE, 0, 0, &dynamicBuffer, dynamicCB));

	
	while (run)
	{
		lu::input::PollEvents(window);
		run = !lu::input::ShouldQuit(window) && !lu::input::IsKeyDown(window, lu::KeyCode::KEY_ESC);

		if (lu::renderer::ShouldResize(window))
		{
			lu::renderer::ResizeWindow(device, window, mainRenderTarget);
			lu::resources::ResizeTexture2D(device, window.width, window.height, texture);
			resolutionBuffer.uResolution[0] = window.width;
			resolutionBuffer.uResolution[1] = window.height;
			resolutionBuffer.fResolution[0] = (float32_t)window.width;
			resolutionBuffer.fResolution[1] = (float32_t)window.height;
			void* pBuff = lu::resources::MapBuffer(context, resolutionCB, lu::MapType::MAP_WRITE_DISCARD);
			memcpy(pBuff, &resolutionBuffer, sizeof(resolutionBuffer));
			lu::resources::UnmapBuffer(context, resolutionCB);
		}

		const float32_t clearColor[] = { 1, 0, 0, 1 };
		uint32_t strides = sizeof(float32_t) * 5, offsets = 0;
		lu::Viewport viewports;

		viewports.x = 0.0f;
		viewports.y = 0.0f;
		viewports.width = (float32_t)window.width;
		viewports.height = (float32_t)window.height;
		viewports.nearDepth = 0.0f;
		viewports.farDepth = 1.0f;

		// Compute Pass
		{
			lu::Vec4* pBuff = (lu::Vec4*)lu::resources::MapBuffer(context, dynamicCB, lu::MapType::MAP_WRITE_DISCARD);
			memcpy(pBuff, &dynamicBuffer, sizeof(dynamicBuffer));
			lu::resources::UnmapBuffer(context, dynamicCB);
		}
		lu::commands::SetComputeShaderConstantBuffer(commandList, 0, 1, &resolutionCB);
		lu::commands::SetComputeShaderConstantBuffer(commandList, 1, 1, &dynamicCB);
		lu::commands::SetComputeShaderUAV(commandList, 0, 1, &uav);
		lu::commands::SetComputeShader(commandList, &computeShader);
		lu::commands::Dispatch(commandList, window.width / 8, window.height / 8, 1);
		lu::commands::SetComputeShaderUAV(commandList, 0, 1, NULL);
		lu::commands::SetComputeShaderConstantBuffer(commandList, 0, 1, NULL);
		lu::commands::SetComputeShaderConstantBuffer(commandList, 1, 1, NULL);
		lu::commands::SetComputeShader(commandList, NULL);
		
		// Graphics Pass
		lu::commands::SetViewports(commandList, 1, &viewports);
		lu::commands::SetRasterizerState(commandList, rasterizerState);
		lu::commands::ClearRenderTarget(commandList, mainRenderTarget, clearColor);
		lu::commands::SetRenderTargets(commandList, 1, &mainRenderTarget, NULL);
		lu::commands::SetVertexShader(commandList, &quadVertexShader);
		lu::commands::SetPixelShader(commandList, &quadPixelShader);
		lu::commands::SetPixelShaderResourceViews(commandList, 0, 1, &textureSRV);
		lu::commands::SetVertexBuffers(commandList, 0, 1, &vertexBuffer, &strides, &offsets);
		lu::commands::SetPrimitiveTopology(commandList, lu::PrimitiveTopology::TRIANGLE_LIST);
		lu::commands::SetPixelShaderSamplers(commandList, 0, 1, &samplerState);
		lu::commands::Draw(commandList, 6, 0);
		lu::commands::SetPixelShaderResourceViews(commandList, 0, 1, NULL);

		lu::commands::ExecuteCommandList(context, commandList);
		lu::renderer::Present(window);
		commandList.reset();

		dynamicBuffer.x += 0.01f;
		if (lu::input::IsMouseButtonDown(window, lu::MouseButton::LEFT))
		{
			lu::Vec2 pos = lu::input::GetMousePos(window);
			dynamicBuffer.z = pos.x;
			dynamicBuffer.w = pos.y;
		}
	}
	
	lu::states::DestroySamplerState(device, samplerState);
	lu::states::DestroyRasterizerState(device, rasterizerState);
	lu::resources::DestroyBuffer(device, dynamicCB);
	lu::resources::DestroyBuffer(device, resolutionCB);
	lu::resources::DestroyBuffer(device, vertexBuffer);
	lu::resources::DestroyShaderResourceView(device, textureSRV);
	lu::resources::DestroyPixelShader(device, quadPixelShader);
	lu::resources::DestroyVertexShader(device, quadVertexShader);
	lu::resources::DestroyTexture2D(device, texture);
	lu::resources::DestroyComputeShader(device, computeShader);
	lu::resources::DestroyUnorderedAccessView(device, uav);
	lu::commands::DestroyCommandList(pageAllocator, commandList);
	lu::renderer::DestroyWindowAndFinalizeGraphicsAPI(window, device, context, mainRenderTarget);

}
