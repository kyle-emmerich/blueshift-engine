#ifdef _WIN32
#include "Window_WIN.h"
#include "Core/Engine.h"
#include "Graphics/RenderSystem.h"
#include "Platform/DisplayInfo.h"
#include "Input/InputSystem.h"
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

	//now is an opportune time to register the input devices.
	RAWINPUTDEVICE device;
	device.usUsagePage = 0x01;  //standard PC controls
	device.usUsage = 0x06;		//keyboard
	device.dwFlags = RIDEV_NOLEGACY; //no legacy messages like WM_KEYDOWN
	device.hwndTarget = this->handle;
	RegisterRawInputDevices(&device, 1, sizeof(device));

	device.usUsage = 0x02; //mouse
	RegisterRawInputDevices(&device, 1, sizeof(device));
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

void Window::SetPosition(uint32_t X, uint32_t Y) {
	//Get our original rect, so we don't change size
	RECT rect; GetWindowRect(handle, &rect);
	MoveWindow(handle, X, Y, rect.right - rect.left, rect.bottom - rect.top, false);
}

void Window::GetPosition(uint32_t& X, uint32_t& Y) const {
	RECT rect; GetWindowRect(handle, &rect);
	X = rect.left;
	Y = rect.top;
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

	Input::InputSystem* inputsystem = Core::Engine::Get().GetSystem<Input::InputSystem>();

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
			Input::MouseEvent* ev = inputsystem->MouseEvents.Next();
			ev->type = Input::MouseEvent::Type::MoveTo;
			ev->x = GET_X_LPARAM(lParam);
			ev->y = GET_Y_LPARAM(lParam);
			break;
		case WM_CHAR:
			
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
			if (input.header.dwType == RIM_TYPEKEYBOARD) {
				const RAWKEYBOARD& kb = input.data.keyboard;
				UINT vkey = kb.VKey;
				UINT scancode = kb.MakeCode;
				UINT flags = kb.Flags;

				if (vkey == 255) {
					return 0;
				} else if (vkey == VK_SHIFT) {
					vkey = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
				} else if (vkey == VK_NUMLOCK) {
					vkey = MapVirtualKey(scancode, MAPVK_VK_TO_VSC) | 0x100;
				}

				const bool E0 = (flags & RI_KEY_E0) != 0;
				const bool E1 = (flags & RI_KEY_E1) != 0;

				if (E1) {
					if (vkey == VK_PAUSE) {
						scancode = 0x45;
					} else {
						scancode = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
					}
				}

				Input::KeyboardEvent* ev = inputsystem->KeyboardEvents.Next();
				ev->is_down = (flags & RI_KEY_BREAK) == 0;
				UINT key_scancode = (scancode << 16) | (E0 << 24);
				GetKeyNameText((LONG)key_scancode, ev->name, 16);

				switch (vkey) {
				case VK_CONTROL:
					ev->key = E0 ? Input::Button::RightControl : Input::Button::LeftControl;
					break;
				case VK_MENU:
					ev->key = E0 ? Input::Button::RightAlt : Input::Button::LeftAlt;
					break;
				case VK_RETURN:
					ev->key = E0 ? Input::Button::NumPadEnter : Input::Button::Enter;
					break;
				case VK_INSERT:
					ev->key = E0 ? Input::Button::Insert : Input::Button::NumPad0;
					break;
				case VK_DELETE:
					ev->key = E0 ? Input::Button::Delete : Input::Button::NumPadDecimal;
					break;
				case VK_HOME:
					ev->key = E0 ? Input::Button::Home : Input::Button::NumPad7;
					break;
				case VK_END:
					ev->key = E0 ? Input::Button::End : Input::Button::NumPad1;
					break;
				case VK_PRIOR:
					if (!E0) ev->key = Input::Button::NumPad9;
					break;
				case VK_NEXT:
					if (!E0) ev->key = Input::Button::NumPad3;
					break;
				case VK_LEFT:
					ev->key = E0 ? Input::Button::Left : Input::Button::NumPad4;
					break;
				case VK_RIGHT:
					ev->key = E0 ? Input::Button::Right : Input::Button::NumPad6;
					break;
				case VK_UP:
					ev->key = E0 ? Input::Button::Up : Input::Button::NumPad8;
					break;
				case VK_DOWN:
					ev->key = E0 ? Input::Button::Down : Input::Button::NumPad2;
					break;
				case VK_CLEAR:
					if (!E0) ev->key = Input::Button::NumPad5;
					break;

				//Now for the non-weirdos
				case VK_CAPITAL:	ev->key = Input::Button::CapsLock;	break;
				case VK_ESCAPE:		ev->key = Input::Button::Esc;		break;
				case VK_TAB:		ev->key = Input::Button::Tab;		break;
				case VK_SPACE:		ev->key = Input::Button::Space;		break;
				case VK_BACK:		ev->key = Input::Button::Backspace;	break;

				case VK_MULTIPLY:	ev->key = Input::Button::Multiply;	break;
				case VK_DIVIDE:		ev->key = Input::Button::Divide;	break;
				case VK_ADD:		ev->key = Input::Button::Add;		break;
				case VK_SUBTRACT:	ev->key = Input::Button::Subtract;	break;

				case VK_NUMLOCK:	ev->key = Input::Button::NumLock;	break;
				
				case VK_SNAPSHOT:	ev->key = Input::Button::PrintScreen; break;
				case VK_DECIMAL:	ev->key = Input::Button::Period;	break;
				case VK_OEM_102:	ev->key = Input::Button::Backslash;	break;
				case VK_OEM_3:		ev->key = Input::Button::Tilde;		break;

				case VK_OEM_4:		ev->key = Input::Button::LeftBracket; break;
				case VK_OEM_7:		ev->key = Input::Button::RightBracket; break;

				default:
					//Decode anything else; typically key ranges like A-Z and numbers.
					//0 key is 0x30, 9 key is 0x39 
					if (vkey >= 0x30 && vkey <= 0x39) {
						ev->key = (Input::Button)((vkey - 0x30) + (int)Input::Button::Key0);
					}
					//A key is 0x41, Z key is 0x5A
					if (vkey >= 0x41 && vkey <= 0x5A) {
						ev->key = (Input::Button)((vkey - 0x41) + (int)Input::Button::KeyA);
					}
					//NP0 is 0x60, NP9 is 0x69
					if (vkey >= 0x60 && vkey <= 0x69) {
						ev->key = (Input::Button)((vkey - 0x60) + (int)Input::Button::NumPad0);
					}
					//F1 is 0x70, F12 is 0x7B
					if (vkey >= 0x70 && vkey <= 0x7B) {
						ev->key = (Input::Button)((vkey - 0x70) + (int)Input::Button::F1);
					}
				}

			} else if (input.header.dwType == RIM_TYPEMOUSE) {
				RAWMOUSE& ms = input.data.mouse;
				Input::MouseEvent* ev = inputsystem->MouseEvents.Next();
				ev->type = Input::MouseEvent::Type::StateUpdate;

				ev->x = 0;
				ev->y = 0;
				if (ms.usFlags & MOUSE_MOVE_RELATIVE != 0) {
					ev->x = ms.lLastX;
					ev->y = ms.lLastY;
				}

				ev->buttons[0] = false;
				if (ms.usFlags & MOUSE_ATTRIBUTES_CHANGED != 0) {
					ev->buttons[0] = true;
					switch (ms.usButtonFlags) {
					case RI_MOUSE_LEFT_BUTTON_DOWN:
						ev->buttons[(size_t)Input::Button::MouseLeft] = true;
						break;
					case RI_MOUSE_LEFT_BUTTON_UP:
						ev->buttons[(size_t)Input::Button::MouseLeft] = false;
						break;
					case RI_MOUSE_RIGHT_BUTTON_DOWN:
						ev->buttons[(size_t)Input::Button::MouseRight] = true;
						break;
					case RI_MOUSE_RIGHT_BUTTON_UP:
						ev->buttons[(size_t)Input::Button::MouseRight] = false;
						break;
					case RI_MOUSE_MIDDLE_BUTTON_DOWN:
						ev->buttons[(size_t)Input::Button::MouseMiddle] = true;
						break;
					case RI_MOUSE_MIDDLE_BUTTON_UP:
						ev->buttons[(size_t)Input::Button::MouseMiddle] = false;
						break;
					case RI_MOUSE_WHEEL:
						ev->scroll_amount = ms.usButtonData;
						break;
					}
				}
			}
			break;
		
	}

	return DefWindowProc(handle, msg, wParam, lParam);
}

#endif