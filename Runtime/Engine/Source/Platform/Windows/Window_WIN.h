#pragma once
#ifdef _WIN32 //Windows platform

#include "Platform/IWindow.h"
#include <Windows.h>

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

	void Resize(uint32_t Width, uint32_t Height, bool IsInnerMeasurement = true);
	void Position(uint32_t X, uint32_t Y);

	void Close();
	bool IsClosed() const;

	void SetMaximized(bool IsMaximized);
	bool IsMaximized() const;

	void SetMinimized(bool IsMinimized);
	bool IsMinimized() const;

	void SetFullscreen(bool IsFullscreen);
	bool IsFullscreen() const;

	void SetFullscreenDesktop(bool IsFullscreen, bool SpanAllDisplays = false);
	bool IsFullscreenDesktop() const;

	void SetTitle(String Title);
	String GetTitle() const;

	bool ProcessEvents();

	inline HWND GetHWND() const {
		return handle;
	}

	static void _RegisterClass();
	static LRESULT CALLBACK WindowCallback(HWND, UINT, WPARAM, LPARAM);
};

#endif