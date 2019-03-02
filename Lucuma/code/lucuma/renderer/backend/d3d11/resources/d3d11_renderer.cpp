#include "../../../renderer.h"
#include "../../../states/blend_state.h"
#include "../../../states/rasterizer_state.h"
#include "../../../states/sampler_state.h"
#include "../../../resources/render_target.h"
#include "../../../resources/buffer.h"
#include "../../../resources/shader.h"
#include "../../../resources/texture.h"
#include <d3d11.h>
#include <Windows.h>
#include <Windowsx.h>
#include <stdio.h>
#include "../../../../debug.h"

#define LU_MAX_WINDOWS 20

static lu::Window* gWindowList[LU_MAX_WINDOWS] = { NULL };
static uint32_t gWindowListSize = 0;

static void InvalidateWindow(lu::Window* pWindow)
{
	for (uint32_t i = 0; i < gWindowListSize; ++i)
	{
		if(pWindow == gWindowList[i])
		{
			gWindowList[i] = NULL;
			return;
		}
	}
}

static lu::Window* GetWindowByHandle(HWND handle)
{
	for (uint32_t i = 0; i < gWindowListSize; ++i)
	{
		lu::Window* pWindow = gWindowList[i];
		if (pWindow != NULL && (HWND)(pWindow->window) == handle)
		{
			return pWindow;
		}
	}
	return NULL;
}

static LRESULT CALLBACK luWindowProc(HWND a, UINT b, WPARAM c, LPARAM d)
{
	switch (b)
	{
	case WM_SIZE:
	{
		uint32_t newWidth = GET_X_LPARAM(d);
		uint32_t newHeight = GET_Y_LPARAM(d);
		lu::Window* pWindow = GetWindowByHandle(a);
		if (pWindow != NULL)
		{
			pWindow->inputShouldResize = true;
			pWindow->width = newWidth;
			pWindow->height = newHeight;
		}
		return 0;
	}
	case WM_CREATE:
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_DESTROY:
		return 0;
	default:
		return DefWindowProc(a, b, c, d);
	}
}

bool lu::renderer::CreateWindowAndInitializeGraphicsAPIAlsoWaitForRenderDoc(CreateWindowDesc& desc, Window& window, RendererDevice& device, RendererContext& context, RenderTarget& mainRenderTarget)
{
	char buffer[128];
	sprintf_s(buffer, 128, "Process ID (%u)", GetCurrentProcessId());
	MessageBoxA(NULL, buffer, "Waiting for RenderDoc", MB_OK);
	return CreateWindowAndInitializeGraphicsAPI(desc, window, device, context, mainRenderTarget);
}

bool lu::renderer::CreateWindowAndInitializeGraphicsAPI(CreateWindowDesc& desc, Window& window, RendererDevice& device, RendererContext& context, RenderTarget& mainRenderTarget)
{
	// Create Window
	//DWORD style = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;
	DWORD style = WS_VISIBLE | WS_CAPTION | WS_SYSMENU;
	RECT fullsize = { 0, 0 , (LONG)desc.width, (LONG)desc.height};
	HMODULE program = GetModuleHandle(NULL);
	WNDCLASS windowClass = { 0 };
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = &luWindowProc;
	windowClass.hInstance = program;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = "WindowClass0";
	RegisterClass(&windowClass);

	// Set window style
	if ((desc.style & WINDOW_FULLSCREEN) != 0)
	{
		// Nothing for now
	}
	if ((desc.style & WINDOW_MAXIMIZE) != 0)
	{
		style |= WS_MAXIMIZE;
	}
	if ((desc.style & WINDOW_BUTTON_MAXIMIZE) != 0)
	{
		style |= WS_MAXIMIZEBOX;
	}
	if ((desc.style & WINDOW_BUTTON_MINIMIZE) != 0)
	{
		style |= WS_MINIMIZEBOX;
	}
	if ((desc.style & WINDOW_RESIZE) != 0)
	{
		style |= WS_SIZEBOX;
	}

	AdjustWindowRect(&fullsize, style, 0);
	HWND windowHandle = CreateWindow("WindowClass0", desc.pTitle, style, CW_USEDEFAULT, CW_USEDEFAULT, fullsize.right - fullsize.left, fullsize.bottom - fullsize.top, NULL, NULL, program, NULL);

	// Initialize D3D11
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	ID3D11Texture2D* pColorBuffer = NULL;
	ID3D11Device* pDeviceD3D = NULL;
	ID3D11DeviceContext* pDeviceContextD3D = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	ID3D11RenderTargetView* pMainRenderTargetView = NULL;
	HRESULT result;

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = windowHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

#if defined(_DEBUG)
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, 0, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDeviceD3D, NULL, &pDeviceContextD3D);
#else
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDeviceD3D, NULL, &pDeviceContextD3D);
#endif

	LU_ASSERT(result == S_OK);
	result = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pColorBuffer);
	LU_ASSERT(result == S_OK);
	result = pDeviceD3D->CreateRenderTargetView((ID3D11Resource*)pColorBuffer, NULL, &pMainRenderTargetView);
	LU_ASSERT(result == S_OK);
	pColorBuffer->Release();

	ZeroMemory(&window, sizeof(Window));

	RECT winRect;
	BOOL sizeResult = GetWindowRect(windowHandle, &winRect);

	if (sizeResult && (desc.style & WINDOW_MAXIMIZE) != 0)
	{
		AdjustWindowRect(&winRect, style, 0);
		desc.width = winRect.right + winRect.left;
		desc.height = winRect.bottom + winRect.top;
	}

	window.width = desc.width;
	window.height = desc.height;
	window.window = windowHandle;
	window.swapChain = (handle_t)pSwapChain;
	window.inputMousePos.x = 0.0f;
	window.inputMousePos.y = 0.0f;
	window.inputMouseWheel = 0.0f;
	mainRenderTarget.renderTarget = (handle_t)pMainRenderTargetView;
	mainRenderTarget.pTexture = NULL;

	if (gWindowListSize < LU_MAX_WINDOWS)
		gWindowList[gWindowListSize++] = &window;
	else
		LU_WARN_MSG(0, "Exceeded the amount of windows allowed. Some events won't be fired.");

	device = (handle_t)pDeviceD3D;
	context = (handle_t)pDeviceContextD3D;

	return true;
}

void lu::renderer::DestroyWindowAndFinalizeGraphicsAPI(Window& window, RendererDevice& device, RendererContext& context, RenderTarget& mainRenderTarget)
{
	lu::resources::DestroyRenderTarget(device, mainRenderTarget);
	lu::renderer::DestroyRendererContext(device, context);
	lu::renderer::DestroyRendererDevice(device);
	lu::renderer::CloseWindow(window);
}

void lu::renderer::CloseWindow(Window& window)
{
	InvalidateWindow(&window);
	DestroyWindow((HWND)window.window);
	IDXGISwapChain* pSwapChain = (IDXGISwapChain*)window.swapChain;
	if (pSwapChain != NULL)
	{
		pSwapChain->Release();
	}
	window.swapChain = NULL;
	window.width = 0;
	window.height = 0;
	window.window = NULL;
}

bool lu::renderer::GetBackBuffer(const RendererDevice& device, const Window& window, RenderTarget& renderTarget)
{
	ID3D11Device* pDevice = (ID3D11Device*)device;
	ID3D11RenderTargetView* pRenderTargetView = NULL;
	IDXGISwapChain* pSwapChain = (IDXGISwapChain*)window.swapChain;
	ID3D11Texture2D* pColorBuffer = NULL;
	HRESULT result = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pColorBuffer);
	if (result != S_OK) return false;
	result = pDevice->CreateRenderTargetView((ID3D11Resource*)pColorBuffer, NULL, &pRenderTargetView);
	if (result != S_OK) return false;
	pColorBuffer->Release();
	renderTarget.renderTarget = (handle_t)pRenderTargetView;
	renderTarget.pTexture = NULL;
	return true;
}

bool lu::renderer::ShouldResize(const Window& window)
{
	return window.inputShouldResize;
}

bool lu::renderer::ResizeWindow(const RendererDevice& device, uint32_t width, uint32_t height, Window& window, RenderTarget& mainRenderTarget)
{
	resources::DestroyRenderTarget(device, mainRenderTarget);
	IDXGISwapChain* pSwapChain = (IDXGISwapChain*)window.swapChain;
	HRESULT result = pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, 0);
	LU_ASSERT_MSG(result == S_OK, "Failed to resize swap chain buffer");
	if (!GetBackBuffer(device, window, mainRenderTarget))
	{
		return false;
	}
	
	window.inputShouldResize = false;
	return true;
}

bool lu::renderer::ResizeWindow(const RendererDevice& device, Window& window, RenderTarget& mainRenderTarget)
{
	return ResizeWindow(device, window.width, window.height, window, mainRenderTarget);
}

void lu::renderer::DestroyRendererDevice(RendererDevice& device)
{
	ID3D11Device* pDeviceD3D = (ID3D11Device*)(device);
#if _DEBUG
	//ID3D11Debug* pDebug = NULL;
	//pDeviceD3D->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pDebug));
	//if (pDebug != NULL)
	//{
	//	pDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
	//}
#endif
	if (pDeviceD3D != NULL) pDeviceD3D->Release();
}
void lu::renderer::DestroyRendererContext(const RendererDevice& device, RendererContext& context)
{
	ID3D11DeviceContext* pDeviceContextD3D = (ID3D11DeviceContext*)(context);
	if (pDeviceContextD3D != NULL) pDeviceContextD3D->Release();
}

void lu::renderer::Present(const Window& window, uint32_t interval)
{
	IDXGISwapChain* pSwapChain = (IDXGISwapChain*)window.swapChain;
	pSwapChain->Present(interval, 0);
}

