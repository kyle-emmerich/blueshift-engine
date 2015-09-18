#pragma once
#include "Platform/DisplayInfo.h"
#include "Core/BasicTypes/Array.h"
#include "Graphics/RenderWindow.h"
#include <thread>
#include <memory>

class Engine {
private:
	Array<DisplayInfo> displays;
	size_t primary_display_index;

	Array<RenderWindow*> render_windows;

	static Engine* _instance;
	
	std::thread render_thread;
	void render_thread_func();
	bool stop = false;
public:
	inline static Engine& Get() {
		return *_instance;
	}
	Engine();
	~Engine();

	void AddAvailableDisplayInfo(DisplayInfo&& Display);

	DisplayInfo& GetPrimaryDisplay() const;
	DisplayInfo& GetDisplay(size_t index) const;
	const Array<DisplayInfo>& GetDisplays() const;

	void SetPrimaryDisplay(const DisplayInfo& info);
	void SetPrimaryDisplay(size_t index);

	void AddRenderWindow(RenderWindow& Window);
	RenderWindow& GetPrimaryRenderWindow() const;
	const Array<RenderWindow*>& GetRenderWindows() const;

	int Run();
	inline void Stop() { stop = true; }
};