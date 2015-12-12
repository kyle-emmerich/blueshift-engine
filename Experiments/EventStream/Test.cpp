#include "EventStream.h"
#include <iostream>

__declspec(align(16)) struct TestEvent {
	__m128 vec;
};


int main(int argc, char* argv[]) {
	EventStream<TestEvent> stream(7);

	stream.BeginWrite();
	__declspec(align(16)) float zero = 0.0f;
	__m128 sum = _mm_load_ps(&zero);
	for (int i = 0; i < 10; i++) {
		TestEvent* ev_in = stream.PushEvent();
		//ev_in->x = i;
		__declspec(align(16)) float vec = (float)i * 10.0f;
		ev_in->vec = _mm_load1_ps(&vec);
		sum = _mm_add_ps(sum, ev_in->vec);
	}
	stream.EndWrite();

	for (int i = 0; i < 10; i++) {
		TestEvent* ev_out = stream.PopEvent();
		if (ev_out != nullptr) {
		//	std::cout << "out: " << ev_out->x << std::endl;
			std::cout << "vec: " << ev_out->vec.m128_f32[0] << std::endl;
		}
	}
	std::cout << "Sum: " << sum.m128_f32[0] << std::endl;
	std::cout << "Lost: " << stream.GetNumberLost() << std::endl;

	//yucky
	system("PAUSE");
	return 0;
}