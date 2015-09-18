#ifdef _WIN32 //Windows platform

#include "Platform/DisplayInfo.h"
#include "Core/Engine.h"


#include <Windows.h>

bool CALLBACK MonitorEnumCallback(HMONITOR monitor, HDC device, RECT* rc, LPARAM engine);

void DisplayInfo::EnumerateDisplays(Engine* engine) {
	EnumDisplayMonitors(NULL, NULL, (MONITORENUMPROC)MonitorEnumCallback, (LPARAM)engine);
}

bool CALLBACK MonitorEnumCallback(HMONITOR monitor, HDC device, RECT* rc, LPARAM data) {
	Engine* engine = (Engine*)data;

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

	engine->AddAvailableDisplayInfo(std::move(info));

	return TRUE;
}

#endif