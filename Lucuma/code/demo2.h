#include "lucuma/renderer.h"
#include "lucuma/debug.h"
#include "lucuma/utils/page_allocator.h"
#include "lucuma/input.h"
#include "lucuma/utils/imgui/imgui_renderer.h"

void demo2()
{
	lu::PageAllocator pageAllocator;
	lu::CommandList commandList;
	lu::RendererDevice device;
	lu::RendererContext context;
	lu::RenderTarget mainRenderTarget;
	lu::Window window;
	lu::CreateWindowDesc initDesc;
	lu::ImGuiRenderer imguiRenderer;
	lu::RenderTarget imguiRenderTarget;
	lu::Texture2D imguiRenderTexture;
	lu::VertexShader fullscreenVS;
	lu::PixelShader fullscreenPS;
	lu::VertexLayout fullscreenLayout;
	lu::Buffer fullscreenVB;
	lu::Buffer fullscreenCB;
	lu::SamplerState fullscreenSS;
	lu::DepthStencilState fullscreenDS;
	lu::Vec4 fullscreenCBData = { 640, 480, 0, 0 };
	lu::Texture3D texture0;
	struct Vertex {
		float32_t x, y;
		float32_t u, v;
	} fullscreenVertices[] = {
		{ -1.0f, 1.0f, 0.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f },

		{ -1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 0.0f }
	};

	fullscreenLayout.addAttribute(lu::ResourceFormat::FORMAT_R32G32_FLOAT, 0);
	fullscreenLayout.addAttribute(lu::ResourceFormat::FORMAT_R32G32_FLOAT, sizeof(float32_t) * 2);

	initDesc.width = 640;
	initDesc.height = 480;
	initDesc.pTitle = "Demo 2";
	initDesc.style = lu::WINDOW_RESIZE | lu::WINDOW_BUTTON_MAXIMIZE;

#if 1
	LU_ASSERT(lu::renderer::CreateWindowAndInitializeGraphicsAPI(initDesc, window, device, context, mainRenderTarget));
#else
	LU_ASSERT(lu::renderer::CreateWindowAndInitializeGraphicsAPIAlsoWaitForRenderDoc(initDesc, window, device, context, mainRenderTarget));
#endif
	LU_ASSERT(lu::commands::CreateCommandList(pageAllocator, 100, commandList));
	LU_ASSERT(lu::resources::CreateTexture2D(device, lu::ResourceFormat::FORMAT_R8G8B8A8_UNORM, NULL, 640, 480, lu::TextureBinding::RENDER_TARGET_BINDING | lu::TextureBinding::SHADER_BINDING, imguiRenderTexture));
	LU_ASSERT(lu::resources::CreateRenderTarget2D(device, imguiRenderTexture, imguiRenderTarget));
	LU_ASSERT(imguiRenderer.initialize(pageAllocator, device, window, &imguiRenderTarget));
	
	LU_ASSERT(lu::resources::CreateVertexShaderFromSourceFile(pageAllocator, device, "code/lucuma/shaders/hlsl/fullscreen.hlsl", "mainVS", fullscreenLayout, fullscreenVS));
	LU_ASSERT(lu::resources::CreatePixelShaderFromSourceFile(pageAllocator, device, "code/lucuma/shaders/hlsl/fullscreen.hlsl", "mainPS", fullscreenPS));
	LU_ASSERT(lu::resources::CreateBuffer(device, sizeof(fullscreenVertices), lu::BufferUsage::USAGE_IMMUTABLE, lu::BIND_VERTEX_BUFFER, lu::CPU_ACCESS_NONE, lu::RESOURCE_NONE, 0, fullscreenVertices, fullscreenVB));
	fullscreenCBData.x = (float32_t)window.width;
	fullscreenCBData.y = (float32_t)window.height;
	fullscreenCBData.z = 0.5f;
	LU_ASSERT(lu::resources::CreateBuffer(device, sizeof(fullscreenCBData), lu::BufferUsage::USAGE_DYNAMIC, lu::BIND_CONSTANT_BUFFER, lu::CPU_ACCESS_WRITE, lu::RESOURCE_NONE, 0, &fullscreenCBData, fullscreenCB));
	LU_ASSERT(lu::states::CreateSamplerState(device, lu::SamplerFilterType::POINT, lu::TextureAddressMode::CLAMP, fullscreenSS));
	LU_ASSERT(lu::states::CreateDepthStencilState(device, lu::DepthState::GetDefault(false), lu::StencilState::GetDefault(false), lu::DepthStencilOp::GetDefault(), lu::DepthStencilOp::GetDefault(), fullscreenDS));

	{
		const uint32_t size = 256;
		byte_t* pixels = (byte_t*)pageAllocator.allocate(size * size * size * 4);
		
		for (uint32_t z = 0; z < size; z++)
		{
			for (uint32_t y = 0; y < size; y++)
			{
				for (uint32_t x = 0; x < size; x++)
				{
					uint32_t index = x + y * size + z * size * size;
					index *= 4;

					pixels[index + 0] = x;
					pixels[index + 1] = y;
					pixels[index + 2] = z;
					pixels[index + 3] = 0xFF;
				}
			}
		}

		LU_ASSERT(lu::resources::CreateTexture3D(device, lu::ResourceFormat::FORMAT_R8G8B8A8_UNORM, pixels, size, size, size, lu::SHADER_BINDING, texture0));
		pageAllocator.deallocate(pixels);
	}

	float32_t time = 0.0f;
	bool run = true;

	while (run)
	{
		// Update 
		lu::input::PollEvents(window);
		if (lu::renderer::ShouldResize(window))
		{
			LU_ASSERT(lu::renderer::ResizeWindow(device, window, mainRenderTarget));
			LU_ASSERT(lu::resources::ResizeTexture2D(device, window.width, window.height, imguiRenderTexture));
			lu::resources::DestroyRenderTarget(device, imguiRenderTarget);
			LU_ASSERT(lu::resources::CreateRenderTarget2D(device, imguiRenderTexture, imguiRenderTarget));
			fullscreenCBData.x = (float32_t)window.width;
			fullscreenCBData.y = (float32_t)window.height;
			fullscreenCBData.z = time;
			void* pMappedBuffer = lu::resources::MapBuffer(context, fullscreenCB, lu::MapType::MAP_WRITE_DISCARD);
			memcpy(pMappedBuffer, &fullscreenCBData, sizeof(fullscreenCBData));
			lu::resources::UnmapBuffer(context, fullscreenCB);
		}

		fullscreenCBData.x = (float32_t)window.width;
		fullscreenCBData.y = (float32_t)window.height;
		fullscreenCBData.z = time;
		void* pMappedBuffer = lu::resources::MapBuffer(context, fullscreenCB, lu::MapType::MAP_WRITE_DISCARD);
		memcpy(pMappedBuffer, &fullscreenCBData, sizeof(fullscreenCBData));
		lu::resources::UnmapBuffer(context, fullscreenCB);


		if (lu::input::ShouldQuit(window) ||
			lu::input::IsKeyDown(window, lu::KeyCode::KEY_ESC))
		{
			run = false;
		}
		imguiRenderer.update(window);

		ImGui::Text("Hello World\n");

		// Render
		float32_t clearColor[] = { 0,0,0,1 };
		lu::commands::ClearRenderTarget(commandList, mainRenderTarget, clearColor);
		lu::commands::ClearRenderTarget(commandList, imguiRenderTarget, clearColor);
		lu::commands::SetPixelShaderTextures(commandList, 0, 1, NULL);

		imguiRenderer.draw(commandList, device, context);

		// Full screen pass
		{
			lu::Viewport viewport;
			lu::Scissor scissor;
			viewport.x = 0;
			viewport.y = 0;
			viewport.width = (float32_t)window.width;
			viewport.height = (float32_t)window.height;
			viewport.farDepth = 1.0f;
			viewport.nearDepth = 0.0f;

			scissor.x = 0;
			scissor.y = 0;
			scissor.width = window.width;
			scissor.height = window.height;

			lu::commands::SetVertexShader(commandList, &fullscreenVS);
			lu::commands::SetPixelShader(commandList, &fullscreenPS);
			lu::commands::SetRenderTargets(commandList, 1, &mainRenderTarget, NULL);
			lu::commands::SetPixelShaderTextures(commandList, 0, 1, &imguiRenderTexture);
			lu::commands::SetPixelShaderTextures(commandList, 1, 1, &texture0);
			lu::commands::SetPixelShaderSamplers(commandList, 0, 1, &fullscreenSS);
			lu::commands::SetPixelShaderConstantBuffers(commandList, 1, 1, &fullscreenCB);
			uint32_t strides = sizeof(Vertex);
			uint32_t offset = 0;
			lu::commands::SetVertexBuffers(commandList, 0, 1, &fullscreenVB, &strides, &offset);
			lu::commands::SetPrimitiveTopology(commandList, lu::PrimitiveTopology::TRIANGLE_LIST);
			lu::commands::SetViewports(commandList, 1, &viewport);
			lu::commands::SetScissors(commandList, 1, &scissor);
			lu::commands::SetDepthStencilState(commandList, fullscreenDS, 0);
			lu::commands::Draw(commandList, 6, 0);

		}

		lu::commands::ExecuteCommandList(context, commandList);
		lu::renderer::Present(window);
		commandList.reset();
		time += 0.01f;
	}

	lu::resources::DestroyTexture3D(device, texture0);
	lu::states::DestroyDepthStencilState(device, fullscreenDS);
	lu::states::DestroySamplerState(device, fullscreenSS);
	lu::resources::DestroyVertexShader(device, fullscreenVS);
	lu::resources::DestroyPixelShader(device, fullscreenPS);
	lu::resources::DestroyBuffer(device, fullscreenVB);
	lu::resources::DestroyBuffer(device, fullscreenCB);
	imguiRenderer.finalize(device);
	lu::resources::DestroyRenderTarget(device, imguiRenderTarget);
	lu::resources::DestroyTexture2D(device, imguiRenderTexture);
	lu::commands::DestroyCommandList(pageAllocator, commandList);
	lu::renderer::DestroyWindowAndFinalizeGraphicsAPI(window, device, context, mainRenderTarget);
}
