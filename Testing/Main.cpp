#include <Windows.h>
#include <iostream>
using namespace std;

LRESULT CALLBACK callback(HWND, UINT, WPARAM, LPARAM);

int main() {
	WNDCLASS wc; ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.lpszClassName = "bitch";
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	wc.lpfnWndProc = callback;
	RegisterClass(&wc);

	unsigned int num_devices;
	GetRawInputDeviceList(nullptr, &num_devices, sizeof(RAWINPUTDEVICELIST));
	RAWINPUTDEVICELIST* list = new RAWINPUTDEVICELIST[num_devices];
	GetRawInputDeviceList(list, &num_devices, sizeof(RAWINPUTDEVICELIST));
	cout << GetLastError() << endl;

	for (int i = 0; i < num_devices; i++) {
		if (list[i].dwType == RIM_TYPEMOUSE) {
			cout << "Mouse(" << i << "): " << list[i].hDevice << endl;

			unsigned int name_len;
			GetRawInputDeviceInfo(list[i].hDevice, RIDI_DEVICENAME, nullptr, &name_len);
			char* name = new char[name_len];
			GetRawInputDeviceInfo(list[i].hDevice, RIDI_DEVICENAME, name, &name_len);
			cout << "    Name: " << name << endl;
			delete[] name;

			RID_DEVICE_INFO info; info.cbSize = sizeof(RID_DEVICE_INFO);
			GetRawInputDeviceInfo(list[i].hDevice, RIDI_DEVICEINFO, &info, (UINT*)&info.cbSize);

			cout << "    # buttons: " << info.mouse.dwNumberOfButtons << endl;
			cout << "    Sample Rate: " << info.mouse.dwSampleRate << endl;
			cout << "    ID: " << info.mouse.dwId << endl;
		}
	}

	HWND hwnd = CreateWindow("bitch", "Test", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, NULL, NULL, GetModuleHandle(NULL), NULL);
	ShowWindow(hwnd, SW_NORMAL);

	RAWINPUTDEVICE mouse[2];
	for (int i = 0; i < 2; i++) {
		mouse[i].usUsagePage = 0x01;
		mouse[i].usUsage = 0x02;
		mouse[i].dwFlags = RIDEV_INPUTSINK;
		mouse[i].hwndTarget = hwnd;
	}
	if (!RegisterRawInputDevices(mouse, 2, sizeof(RAWINPUTDEVICE))) {
		DWORD err = GetLastError();
		cout << "Error registering devices! Code " << err << endl;
	}
	

	delete[] list;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_INPUT) {
			RAWINPUT input;
			UINT size = sizeof(input), size_header = sizeof(RAWINPUTHEADER);
			
			GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, &input, &size, size_header);
			if (input.header.dwType == RIM_TYPEMOUSE) {
				cout << input.header.hDevice<<": " <<  input.data.mouse.lLastX << ", " << input.data.mouse.lLastY << endl;
			}
		}
	}
}

LRESULT CALLBACK callback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {


	return DefWindowProc(hwnd, msg, wParam, lParam);
}