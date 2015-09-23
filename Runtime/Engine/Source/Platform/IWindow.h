#pragma once
#include "Core/Core.h"

namespace Blueshift {
	namespace Platform {

		class IWindow {
		public:
			virtual void Resize(uint32_t, uint32_t, bool) = 0;
			virtual void Position(uint32_t, uint32_t) = 0;

			virtual void Close() = 0;
			virtual bool IsClosed() const = 0;

			virtual void SetMaximized(bool IsMaximized) = 0;
			virtual bool IsMaximized() const = 0;

			virtual void SetMinimized(bool IsMinimized) = 0;
			virtual bool IsMinimized() const = 0;

			virtual void SetFullscreen(bool IsFullscreen) = 0;
			virtual bool IsFullscreen() const = 0;

			virtual void SetFullscreenDesktop(bool IsFullscreen, bool SpanAllDisplays = false) = 0;
			virtual bool IsFullscreenDesktop() const = 0;

			virtual void SetTitle(std::string Title) = 0;
			virtual std::string GetTitle() const = 0;

			virtual bool ProcessEvents() = 0;
		};

	}
}