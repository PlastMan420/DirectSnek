#include "WindowContainer.h"
#include <memory>

WindowContainer::WindowContainer()
{
	static bool raw_input_initialized = false;
	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;

		rid.usUsagePage = 0x01; //Mouse
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			ErrorLogger::Log(GetLastError(), "Failed to register raw input devices.");
			exit(-1);
		}

		raw_input_initialized = true;
	}
}

//extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	//route all messages to imgui when proc handler
	//if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
	//	return true;

	int mousex = LOWORD(lParam);
	int mousey = HIWORD(lParam);
	switch (uMsg) {
		// KB
		case WM_CHAR:
		{
			unsigned char ch = static_cast<unsigned char>(wParam);
			if (keyboard.IsCharsAutoRepeat()) {
				keyboard.OnChar(ch);
			}
			else {
				const bool wasPressed = lParam & 0x40000000;
				if (!wasPressed) {
					keyboard.OnChar(ch);
				}
			}
			return 0;
		}
		case WM_KEYUP:
		{
			unsigned char ch = static_cast<unsigned char>(wParam);
			keyboard.OnKeyReleased(ch);
			return 0;
		}
		case WM_KEYDOWN: 
		{
			unsigned char ch = static_cast<unsigned char>(wParam);
			if (keyboard.IsKeysAutoRepeat()) {
				keyboard.OnKeyPressed(ch);
			}
			else {
				const bool wasPressed = lParam & 0x40000000;
				if (!wasPressed) {
					keyboard.OnKeyPressed(ch);
				}
			}
			return 0;
		}
		// mouse
		case WM_MOUSEMOVE: {
			mouse.OnMouseMove(mousex, mousey);
			return 0;
		}
		case WM_LBUTTONDOWN: {
			mouse.OnLeftPressed(mousex, mousey);
			return 0;
		}
		case WM_LBUTTONUP: {
			mouse.OnLeftReleased(mousex, mousey);
			return 0;
		}
		case WM_RBUTTONDOWN: {
			mouse.OnRightPressed(mousex, mousey);
			return 0;
		}
		case WM_RBUTTONUP: {
			mouse.OnRightReleased(mousex, mousey);
			return 0;
		}
		case WM_MBUTTONDOWN: {
			mouse.OnMiddlePressed(mousex, mousey);
			return 0;
		}
		case WM_MBUTTONUP: {
			mouse.OnMiddleReleased(mousex, mousey);
			return 0;
		}
		case WM_MOUSEWHEEL: {
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
				mouse.OnWheelUp(mousex, mousey);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
				mouse.OnWheelDown(mousex, mousey);
			}
			return 0;
		}
		case WM_INPUT:{
			UINT dataSize;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

			if (dataSize > 0) {
				std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);

				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize) {
					RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());

					if (raw->header.dwType == RIM_TYPEMOUSE) {
						mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
					}
				}
			}
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		case WM_PAINT: {
			this->gfx.RenderFrame();
		}
		default: {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}
