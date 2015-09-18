#pragma once
#include "Platform/PlatformSpecific.h"
#include <thread>

class RenderWindow : public Window {
protected:
	bool is_primary = false;

	bool dirty_viewport = true;
	uint32_t view_width;
	uint32_t view_height;
	virtual void resize_viewport(uint32_t width, uint32_t height);
public:
	RenderWindow(uint32_t Width = 1280, uint32_t Height = 720);
	virtual ~RenderWindow();

	void PreRender();
	void PostRender();
};