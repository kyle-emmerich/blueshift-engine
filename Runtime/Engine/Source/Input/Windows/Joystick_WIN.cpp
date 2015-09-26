#ifdef _WIN32
#include "Joystick_WIN.h"
#include "Platform/Windows/Window_WIN.h"

using namespace Blueshift;
using namespace Blueshift::Input;
using namespace Blueshift::Input::Devices;

static const double AXIS_MIN = -32768.0;
static const double AXIS_MAX =  32768.0;

std::vector<Joystick*> Joystick::all_joystick_devices;
std::map<HANDLE, size_t> Joystick::device_map;

Joystick* Joystick::primary = nullptr;
bool Joystick::primary_found = false;

double Joystick::GetAxis(AxisHandle Axis, bool ignore_deadzone) const {
	if (Axis > num_axes) {
		return 0.0;
	}
	double value = axes[Axis].value;
	if (std::abs(value) > deadzone && !ignore_deadzone) {
		return value;
	}
	return 0.0;
}

void Joystick::Poll() {
	joyGetPosEx(id, &info);

	DWORD pos[6] = {
		info.dwXpos,
		info.dwYpos,
		info.dwZpos,
		info.dwRpos,
		info.dwUpos,
		info.dwVpos
	};

	for (size_t i = 0; i < num_axes; i++) {
		axes[i].raw_value = pos[i];

		double old = axes[i].value;
		axes[i].value = ((double)pos[i] + axes[i].offset) * axes[i].scale;
		axes[i].delta = axes[i].value - old;
	}
}

void Joystick::Initialize() {
	int axis_min[6], axis_max[6];

	axis_min[0] = caps.wXmin;
	axis_max[0] = caps.wXmax;
	axis_min[1] = caps.wYmin;
	axis_max[1] = caps.wYmax;
	axis_min[2] = caps.wZmin;
	axis_max[2] = caps.wZmax;
	axis_min[3] = caps.wRmin;
	axis_max[3] = caps.wRmax;
	axis_min[4] = caps.wUmin;
	axis_max[4] = caps.wUmax;
	axis_min[5] = caps.wVmin;
	axis_max[5] = caps.wVmax;

	num_axes = caps.wNumAxes;
	axes.reserve(num_axes);

	for (size_t i = 0; i < num_axes; i++) {
		AxisDescription desc;
		desc.offset = AXIS_MIN - axis_min[i];
		desc.scale = 2.0 / (axis_max[i] - axis_min[i]);

		desc.raw_value = 0;
		desc.value = 0.0;
		desc.delta = 0.0;
		axes.push_back(desc);
	}
}

#endif