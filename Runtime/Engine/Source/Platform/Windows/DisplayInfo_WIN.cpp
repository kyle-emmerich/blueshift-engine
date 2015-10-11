#ifdef _WIN32 //Windows platform

#include "Platform/DisplayInfo.h"
#include "Graphics/RenderSystem.h"
#include <Windows.h>

using namespace Blueshift;
using namespace Blueshift::Graphics;
using namespace Blueshift::Platform;

bool CALLBACK MonitorEnumCallback(HMONITOR monitor, HDC device, RECT* rc, LPARAM engine);

void DisplayInfo::EnumerateDisplays(RenderSystem* rendering) {
	EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)MonitorEnumCallback, (LPARAM)rendering);
}

bool CALLBACK MonitorEnumCallback(HMONITOR monitor, HDC device, RECT* rc, LPARAM data) {
	RenderSystem* rendering = (RenderSystem*)data;

	MONITORINFOEX monitor_info;
	monitor_info.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(monitor, &monitor_info);

	DisplayInfo info;

	info.AdapterName = monitor_info.szDevice;
	info.X = monitor_info.rcMonitor.left;
	info.Y = monitor_info.rcMonitor.top;
	info.Width = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
	info.Height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;

	info.NativeHandle = (void*)monitor;

	rendering->AddAvailableDisplayInfo(std::move(info));

	return TRUE;
}

#endif