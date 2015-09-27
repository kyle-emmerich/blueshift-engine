#ifdef _WIN32
#include "Joystick_WIN.h"
#include "Platform/Windows/Window_WIN.h"

using namespace Blueshift;
using namespace Blueshift::Input;
using namespace Blueshift::Input::Devices;

static const double AXIS_MIN = -32768.0;
static const double AXIS_MAX =  32768.0;

std::vector<Joystick*> Joystick::all_joystick_devices;
std::map<GUID, size_t> Joystick::device_map;

Joystick* Joystick::primary = nullptr;
bool Joystick::primary_found = false;

LPDIRECTINPUT8 Joystick::dinput;

double Joystick::GetAxis(AxisHandle Axis, bool ignore_deadzone) const {
	
	return 0.0;
}

void Joystick::Poll() {
	
}

void Joystick::initialize(const DIDEVICEINSTANCE* instance, Platform::IWindow* window) {
	all_joystick_devices.push_back(this);

	name = std::string(instance->tszProductName);
	guid = instance->guidProduct;

	LPDIRECTINPUTDEVICE8 device;
	IDirectInput8_CreateDevice(dinput, instance->guidInstance, &device, NULL);
	IDirectInputDevice8_QueryInterface(device, IID_IDirectInputDevice8, (LPVOID*)&input_device);
	IDirectInputDevice8_Release(device);

	device->SetCooperativeLevel(reinterpret_cast<Platform::Window*>(window)->GetHWND(),
		DISCL_EXCLUSIVE | DISCL_BACKGROUND);
	device->SetDataFormat(&c_dfDIJoystick2);
	device->SendForceFeedbackCommand(DISFFC_SETACTUATORSON);

	//todo: finish
}

BOOL CALLBACK ::EnumJoysticksCallback(const DIDEVICEINSTANCE* instance, VOID* context) {
	Joystick* js = new Joystick;
	js->initialize(instance, (Platform::IWindow*)context);

	return DIENUM_CONTINUE;
}

void Joystick::Register(Platform::IWindow* Window) {
	CoCreateInstance(CLSID_DirectInput8, NULL, CLSCTX_INPROC_SERVER, IID_IDirectInput8, (LPVOID*)&dinput);
	IDirectInput8_Initialize(dinput, GetModuleHandle(NULL), DIRECTINPUT_VERSION);
	IDirectInput8_EnumDevices(dinput, DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, (LPVOID*)&Window, DIEDFL_ATTACHEDONLY);
}

void Joystick::Shutdown() {
	for (auto* joystick : all_joystick_devices) {
		joystick->input_device->Unacquire();
		joystick->input_device->Release();
		delete joystick;
	}
	all_joystick_devices.clear();

	dinput->Release();
}

#endif