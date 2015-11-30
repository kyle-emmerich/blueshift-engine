#pragma once
#ifdef _WIN32 //Windows platform

#include "Platform/IWindow.h"
#include <Windows.h>

namespace Blueshift {
	namespace Platform {

		class Window : public IWindow {
		protected:
			HWND handle;

			bool fullscreen = false;
			bool fullscreen_desktop = true;

			uint32_t last_width = 0;
			uint32_t last_height = 0;

			virtual void setup_platform();
			virtual void resize_viewport(uint32_t width, uint32_t height);
		public:
			Window(uint32_t Width, uint32_t Height, bool IsInnerMeasurement = true);
			virtual ~Window();

			virtual void Resize(uint32_t Width, uint32_t Height, bool IsInnerMeasurement = true);
			virtual void SetPosition(uint32_t X, uint32_t Y);
			virtual void GetPosition(uint32_t&, uint32_t&) const;

			virtual void Close();
			virtual bool IsClosed() const;

			virtual void SetMaximized(bool IsMaximized);
			virtual bool IsMaximized() const;

			virtual void SetMinimized(bool IsMinimized);
			virtual bool IsMinimized() const;

			virtual void SetFullscreen(bool IsFullscreen);
			virtual bool IsFullscreen() const;

			virtual void SetFullscreenDesktop(bool IsFullscreen, bool SpanAllDisplays = false);
			virtual bool IsFullscreenDesktop() const;

			virtual void SetTitle(std::string Title);
			virtual std::string GetTitle() const;

			virtual bool ProcessEvents();

			inline HWND GetHWND() const {
				return handle;
			}

			static void _RegisterClass();
			static LRESULT CALLBACK WindowCallback(HWND, UINT, WPARAM, LPARAM);
		};

	}
}

#endif