#include "../../input.h"
#include "Windows.h"
#include "Windowsx.h"

static lu::InputEventCallback gInputEventCallback = NULL;

#define LU_EMIT_EVENT(type) if (gInputEventCallback != NULL) gInputEventCallback(type, evtData)

void lu::input::PollEvents(Window& window)
{
	MSG msg;

	window.inputMouseWheel = 0.0f;
	window.inputLastChar = 0;

	ZeroMemory(window.inputMouseUp, sizeof(window.inputMouseUp));
	ZeroMemory(window.inputKeyUp, sizeof(window.inputKeyUp));
	ZeroMemory(window.inputKeyClick, sizeof(window.inputKeyClick));
	ZeroMemory(window.inputMouseClick, sizeof(window.inputMouseClick));

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		EventData evtData = { 0 };
		switch (msg.message)
		{
		
		case WM_MOUSEMOVE:
			evtData.point.x = (float32_t)GET_X_LPARAM(msg.lParam);
			evtData.point.y = (float32_t)GET_Y_LPARAM(msg.lParam);
			window.inputMousePos.x = evtData.point.x;
			window.inputMousePos.y = evtData.point.y;
			LU_EMIT_EVENT(EventType::MOUSE_MOVE);
			break;
		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
			evtData.point.x = (float32_t)GET_X_LPARAM(msg.lParam);
			evtData.point.y = (float32_t)GET_Y_LPARAM(msg.lParam);
			window.inputMousePos.x = evtData.point.x;
			window.inputMousePos.y = evtData.point.y;
			evtData.button = 0;
			if (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK) evtData.button = 0;
			if (msg.message == WM_RBUTTONDOWN || msg.message == WM_RBUTTONDBLCLK) evtData.button = 1;
			if (msg.message == WM_MBUTTONDOWN || msg.message == WM_MBUTTONDBLCLK) evtData.button = 2;
			if (!window.inputMouseDown[evtData.button])
			{
				window.inputMouseClick[evtData.button] = true;
			}
			window.inputMouseDown[evtData.button] = true;
			window.inputMouseUp[evtData.button] = false;
			LU_EMIT_EVENT(EventType::MOUSE_BUTTON_DOWN);
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			evtData.point.x = (float32_t)GET_X_LPARAM(msg.lParam);
			evtData.point.y = (float32_t)GET_Y_LPARAM(msg.lParam);
			window.inputMousePos.x = evtData.point.x;
			window.inputMousePos.y = evtData.point.y;
			evtData.button = 0;
			if (msg.message == WM_LBUTTONUP) evtData.button = 0;
			if (msg.message == WM_RBUTTONUP) evtData.button = 1;
			if (msg.message == WM_MBUTTONUP) evtData.button = 2;
			window.inputMouseDown[evtData.button] = false;
			window.inputMouseClick[evtData.button] = false;
			window.inputMouseUp[evtData.button] = true;
			LU_EMIT_EVENT(EventType::MOUSE_BUTTON_UP);
			break;
		case WM_MOUSEWHEEL:
			evtData.delta = (float32_t)GET_WHEEL_DELTA_WPARAM(msg.wParam) / (float)WHEEL_DELTA;
			window.inputMouseWheel = evtData.delta;
			LU_EMIT_EVENT(EventType::MOUSE_WHEEL);
			break;
		case WM_QUIT:
			window.inputShouldQuit = true;
			LU_EMIT_EVENT(EventType::QUIT);
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (msg.wParam < 256)
			{
				evtData.key = (int32_t)msg.wParam;
				if (!window.inputKeyDown[evtData.key])
				{
					window.inputKeyClick[evtData.key] = true;
				}
				window.inputKeyDown[evtData.key] = true;
				window.inputKeyUp[evtData.key] = false;
				LU_EMIT_EVENT(EventType::KEY_DOWN);
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (msg.wParam < 256)
			{
				evtData.key = (int32_t)msg.wParam;
				window.inputKeyClick[evtData.key] = false;
				window.inputKeyDown[evtData.key] = false;
				window.inputKeyUp[evtData.key] = true;
				LU_EMIT_EVENT(EventType::KEY_UP);
			}
			break;
		case WM_CHAR:
			if (msg.wParam > 0 && msg.wParam < 0x10000)
			{
				evtData.inputChar = ((uint16_t)msg.wParam);
				window.inputLastChar = evtData.inputChar;
				LU_EMIT_EVENT(EventType::INPUT_CHAR);
			}
			break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}
}
void lu::input::RegisterInputEventCallback(InputEventCallback callback)
{
	gInputEventCallback = callback;
}

bool lu::input::IsMouseButtonClick(const Window& window, MouseButton button)
{
	return window.inputMouseClick[(uint32_t)button];
}

bool lu::input::IsMouseButtonDown(const Window& window, MouseButton button)
{
	return window.inputMouseDown[(uint32_t)button];
}
bool lu::input::IsMouseButtonUp(const Window& window, MouseButton button)
{
	return window.inputMouseUp[(uint32_t)button];
}
bool lu::input::IsKeyClick(const Window& window, KeyCode key)
{
	return window.inputKeyClick[(uint32_t)key];
}
bool lu::input::IsKeyDown(const Window& window, KeyCode key)
{
	return window.inputKeyDown[(uint32_t)key];
}
bool lu::input::IsKeyUp(const Window& window, KeyCode key)
{
	return window.inputKeyUp[(uint32_t)key];
}
bool lu::input::ShouldQuit(const Window& window)
{
	return window.inputShouldQuit;
}
lu::Vec2 lu::input::GetMousePos(const Window& window)
{
	return window.inputMousePos;
}
float32_t lu::input::GetMouseWheel(const Window& window)
{
	return window.inputMouseWheel;
}
uint16_t lu::input::GetLastChar(const Window& window)
{
	return window.inputLastChar;
}
