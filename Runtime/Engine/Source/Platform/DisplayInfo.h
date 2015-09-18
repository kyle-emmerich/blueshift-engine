#pragma once
#include "Core/BasicTypes/String.h"

class Engine;

struct DisplayInfo {
	DisplayInfo() {}
	DisplayInfo(DisplayInfo& other) {
		AdapterName = other.AdapterName;
		X = other.X;
		Y = other.Y;
		Width = other.Width;
		Height = other.Height;
		NativeHandle = other.NativeHandle;
	}
	uint32_t Index;
	String AdapterName;

	int32_t X;
	int32_t Y;

	int32_t Width;
	int32_t Height;

	void* NativeHandle;

	static void EnumerateDisplays(Engine* engine);
};

