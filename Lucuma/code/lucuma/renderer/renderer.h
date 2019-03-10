#pragma once

#include "../types.h"
#include "../math.h"
namespace lu
{
	typedef handle_t RendererDevice;
	typedef handle_t RendererContext;

	struct RenderTarget;
	struct Window;

	struct Window
	{
		Window() : width(0), height(0), window(LU_INVALID_HANDLE) {}
		uint32_t width;
		uint32_t height;
		handle_t window;
		handle_t swapChain;

		// Input Data
		bool inputMouseClick[3];
		bool inputMouseDown[3];
		bool inputMouseUp[3];
		bool inputShouldQuit;
		bool inputShouldResize;
		bool inputKeyClick[255];
		bool inputKeyDown[255];
		bool inputKeyUp[255];
		Vec2 inputMousePos;
		float32_t inputMouseWheel;
		uint16_t inputLastChar;
	};

	enum WindowStyle
	{
		WINDOW_NO_STYLE = 0x0000,
		WINDOW_FULLSCREEN = 0x0001,
		WINDOW_MAXIMIZE = 0x0002,
		WINDOW_BUTTON_MAXIMIZE = 0x0008,
		WINDOW_BUTTON_MINIMIZE = 0x0010,
		WINDOW_RESIZE = 0x0020
	};
	

	struct CreateWindowDesc
	{
		const char* pTitle;
		uint32_t width;
		uint32_t height;
		uint32_t style;
	};

	namespace renderer
	{
		bool CreateWindowAndInitializeGraphicsAPI(CreateWindowDesc& desc, Window& window, RendererDevice& device, RendererContext& context, RenderTarget& mainRenderTarget);
		bool CreateWindowAndInitializeGraphicsAPIAlsoWaitForRenderDoc(CreateWindowDesc& desc, Window& window, RendererDevice& device, RendererContext& context, RenderTarget& mainRenderTarget);
		void DestroyWindowAndFinalizeGraphicsAPI(Window& window, RendererDevice& device, RendererContext& context, RenderTarget& mainRenderTarget);
		void CloseWindow(Window& window);
		bool GetBackBuffer(const RendererDevice& device, const Window& window, RenderTarget& renderTarget);
		bool ShouldResize(const Window& window);
		bool ResizeWindow(const RendererDevice& device, uint32_t width, uint32_t height, Window& window, RenderTarget& mainRenderTarget);
		bool ResizeWindow(const RendererDevice& device, Window& window, RenderTarget& mainRenderTarget);
		void DestroyRendererDevice(RendererDevice& device);
		void DestroyRendererContext(const RendererDevice& device, RendererContext& context);
		void Present(const Window& window, uint32_t interval = 1);
	}
}
