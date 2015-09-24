#include "Input/InputManager.h"

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace Blueshift;
using namespace Blueshift::Input;

InputManager* InputManager::_instance = 0;

InputManager::InputManager() {
	InputManager::_instance = this;

#ifdef _WIN32
	uint32_t num_devices;
	RAWINPUTDEVICELIST* device_list;
	
	GetRawInputDeviceList(NULL, &num_devices, sizeof(RAWINPUTDEVICELIST));
	device_list = new RAWINPUTDEVICELIST[num_devices];
	GetRawInputDeviceList(device_list, &num_devices, sizeof(RAWINPUTDEVICELIST));

	for (uint32_t i = 0; i < num_devices; i++) {
		//Grab the length of the device name
		std::string device_name; uint32_t device_name_length;
		GetRawInputDeviceInfo(device_list[i].hDevice, RIDI_DEVICENAME, NULL, &device_name_length);
		//Grab the actual name of the device
		device_name.reserve(device_name_length);
		GetRawInputDeviceInfo(device_list[i].hDevice, RIDI_DEVICENAME, &device_name[0], &device_name_length);

		//Now get the device info
		RID_DEVICE_INFO info; info.cbSize = sizeof(RID_DEVICE_INFO);
		uint32_t info_size;
		GetRawInputDeviceInfo(device_list[i].hDevice, RIDI_DEVICEINFO, &info, &info_size);
		


		switch (device_list[i].dwType) {
		case RIM_TYPEHID:
			//generic device, need to figure out more info about it.
			//the usage page ID tells all...
			uint16_t usage_page_id = info.hid.usUsagePage;
			uint16_t usage_id = info.hid.usUsage;



			break;
		case RIM_TYPEKEYBOARD:
			//open a keyboard device

			break;
		case RIM_TYPEMOUSE:
			//open a mouse device

			break;
		}
	}

	delete[] device_list;

#endif
}

InputManager::~InputManager() {

}