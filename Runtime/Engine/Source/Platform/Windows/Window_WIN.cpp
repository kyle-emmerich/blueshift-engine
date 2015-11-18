#ifdef _WIN32
#include "Window_WIN.h"
#include "Core/Engine.h"
#include "Graphics/RenderSystem.h"
#include "Platform/DisplayInfo.h"
#include "Input/Windows/Mouse_WIN.h"
#include "Input/Windows/Keyboard_WIN.h"
#include "bgfx/bgfxplatform.h"
#include <string>
#include <clocale>
#include <Windowsx.h>

//Since Windows only has the macros we need in Windowsx.h, we need
//to undef some others to get rid of errors
//THIS IS WRONG, MICROSOFT
#undef IsMaximized
#undef IsMinimized

using namespace Blueshift;
using namespace Blueshift::Core;
using namespace Blueshift::Platform;

Window::Window(uint32_t Width, uint32_t Height, bool IsInnerMeasurement) {
	Window::_RegisterClass();

	//Choose a style
	DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	//Get the primary display's left/top
	const DisplayInfo& primary = Engine::Get().GetSystem<Graphics::RenderSystem>()->GetPrimaryDisplay();

	//Make a RECT out of our measurements
	RECT size = { primary.X, primary.Y, primary.X + (LONG)Width, primary.Y + (LONG)Height };
	//If we want an inner measurement, we need to adjust it.
	if (IsInnerMeasurement) {
		AdjustWindowRect(&size, style, false);
	}
	size.left += 50;
	size.right += 50;
	size.top += 50;
	size.bottom += 50;

	//Create the window
	handle = CreateWindow(
		"BlueshiftNormalWindow",
		"Blueshift",
		style,
		size.left, size.top,
		size.right - size.left, size.bottom - size.top,
		NULL, NULL,
		GetModuleHandle(NULL),
		0
	);

	//Now we can set our pointer
	SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)this);

	//And show it.
	ShowWindow(handle, SW_SHOWNORMAL);
}

Window::~Window() {
	Close();
}

void Window::setup_platform() {
	bgfx::winSetHwnd(this->handle);
}

void Window::resize_viewport(uint32_t width, uint32_t height) {
	last_width = width;
	last_height = height;
}

void Window::Resize(uint32_t Width, uint32_t Height, bool IsInnerMeasurement) {
	DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	//Get our original rect, so we don't move positions
	RECT rect; GetWindowRect(handle, &rect);
	//Make a RECT out of our measurements
	RECT size = { 0, 0, (LONG)Width, (LONG)Height };
	//If we want an inner measurement, we need to adjust it.
	if (IsInnerMeasurement) {
		AdjustWindowRect(&size, style, false);
	}
	MoveWindow(handle, rect.left, rect.top, size.right - size.left, size.bottom - size.top, false);
	last_width = Width;
	last_height = Height;
}

void Window::Position(uint32_t X, uint32_t Y) {
	//Get our original rect, so we don't change size
	RECT rect; GetWindowRect(handle, &rect);
	MoveWindow(handle, X, Y, rect.right - rect.left, rect.bottom - rect.top, false);
}

void Window::Close() {
	if (handle != 0) {
		DestroyWindow(handle);
		handle = 0;
	}
}

bool Window::IsClosed() const {
	return handle != 0;
}

void Window::SetMaximized(bool IsMaximized) {
	ShowWindow(handle, IsMaximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
}
bool Window::IsMaximized() const {
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(handle, &wp);
	return wp.showCmd == SW_SHOWMAXIMIZED;
}

void Window::SetMinimized(bool IsMinimized) {
	ShowWindow(handle, IsMinimized ? SW_SHOWMINIMIZED : SW_SHOWNORMAL);
}
bool Window::IsMinimized() const {
	WINDOWPLACEMENT wp; 
	wp.length = sizeof(WINDOWPLACEMENT); 
	GetWindowPlacement(handle, &wp);
	return wp.showCmd == SW_SHOWMINIMIZED;
}

void Window::SetFullscreen(bool IsFullscreen) {
	if (fullscreen_desktop) {
		SetFullscreenDesktop(false);
	}
	fullscreen = IsFullscreen;

	//TODO: Wait for bgfx to support exclusive mode fullscreen with proper resolution switching
}
bool Window::IsFullscreen() const {
	return fullscreen;
}

void Window::SetFullscreenDesktop(bool IsFullscreen, bool SpanAllDisplays) {
	//If we're in (normal) fullscreen, kick it out if we want to go fullscreen-desktop
	if (fullscreen && IsFullscreen) {
		SetFullscreen(false);
	}
	fullscreen_desktop = IsFullscreen;

	if (IsFullscreen) {
		RECT rc;
		if (SpanAllDisplays) {
			const std::vector<DisplayInfo>& displays = Engine::Get().GetSystem<Graphics::RenderSystem>()->GetDisplays();
			for (const auto& display : displays) {
				//We need to span all displays,
				//so figure out each one's size.
				RECT subrect;
				subrect.left = display.X;
				subrect.top = display.Y;
				subrect.right = subrect.left + display.Width;
				subrect.bottom = subrect.top + display.Height;

				//Then use Win32 API to figure out the containing rect
				UnionRect(&rc, &rc, &subrect);
			}
		} else {
			//We just need to span the primary display
			const DisplayInfo& primary = Engine::Get().GetSystem<Graphics::RenderSystem>()->GetPrimaryDisplay();
			rc.left = primary.X;
			rc.top = primary.Y;
			rc.right = rc.left + primary.Width;
			rc.bottom = rc.top + primary.Height;
		}
		SetWindowLong(handle, GWL_STYLE, WS_VISIBLE | WS_POPUP);
		SetWindowPos(handle, HWND_TOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 0);
	} else {
		RECT rc = { 0,0,0,0 };
		rc.right = last_width;
		rc.bottom = last_height;
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
		SetWindowLong(handle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		SetWindowPos(handle, HWND_NOTOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0);
	}
}
bool Window::IsFullscreenDesktop() const {
	return fullscreen_desktop;
}

void Window::SetTitle(std::string Title) {
	SetWindowText(handle, Title.c_str());
}
std::string Window::GetTitle() const {
	std::string str; str.resize(256);
	GetWindowText(handle, &str[0], 256);
	return str;
}

bool Window::ProcessEvents() {
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT) {
			Close();
			return false;
		}
	}
	
	return true;
}

void Window::_RegisterClass() {
	//Back out if we've already done this...
	static bool initialized = false;
	if (initialized) {
		return;
	}
	initialized = true;

	//If not, register a WNDCLASSEX
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.lpszClassName = "BlueshiftNormalWindow";
	wc.lpfnWndProc = Window::WindowCallback;
	wc.hInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	//Finish up the class.
	RegisterClassEx(&wc);
}

LRESULT CALLBACK Window::WindowCallback(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
	Window* window = (Window*)GetWindowLongPtr(handle, GWLP_USERDATA);
	if (!window) {
		//We don't care about windows we aren't managing through this class.
		return DefWindowProc(handle, msg, wParam, lParam);
	}

	switch (msg) {
		case WM_DESTROY:
			break;
		case WM_QUIT:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			RECT rect; GetClientRect(handle, &rect);
			window->resize_viewport(rect.right - rect.left, rect.bottom - rect.top);
			break;
		case WM_KILLFOCUS:
			if (window->fullscreen && window->fullscreen_desktop) {
				SetWindowPos(handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOREPOSITION | SWP_NOSIZE);
			}
			break;
		case WM_SETFOCUS:
			if (window->fullscreen && window->fullscreen_desktop) {
				SetWindowPos(handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOREPOSITION | SWP_NOSIZE);
			}
			break;
		case WM_MOUSEMOVE:
			//Tell the input subsystem where the mouse cursor is; device doesn't matter,
			//cursor is OS-level on Windows. Might change for other OSes; watch this space.
			//TODO: update mouse cursor input if API changes during porting 
			Input::Devices::Mouse::_set_last_position(
				GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam)
			);
			break;
		case WM_CHAR:
			Input::Devices::Keyboard::TextInput((char)wParam);
			break;
		case WM_INPUT:
			//Make sure we have focus; we don't want to be grabbing input and doing stuff
			//if the user isn't looking. Kinda creepy if we do.
			if (GetFocus() != handle) {
				break;
			}
			RAWINPUT input;
			UINT size = sizeof(input), size_header = sizeof(RAWINPUTHEADER);

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &input, &size, size_header);		
			if (input.header.dwType == RIM_TYPEMOUSE) {
				Input::Devices::Mouse* mouse = Input::Devices::Mouse::_get_from_handle(input.header.hDevice);
				if (mouse != nullptr) {
					mouse->_pass_event((RAWMOUSE*)&input.data);
				}
			} else if (input.header.dwType == RIM_TYPEKEYBOARD) {
				Input::Devices::Keyboard* keyboard = Input::Devices::Keyboard::_get_from_handle(input.header.hDevice);
				if (keyboard != nullptr) {
					keyboard->_pass_event((RAWKEYBOARD*)&input.data);
				}
			}
			break;
		
	}

	return DefWindowProc(handle, msg, wParam, lParam);
}

#endif