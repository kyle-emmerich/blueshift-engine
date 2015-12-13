#include "EventStream.h"
#include <iostream>
#include <thread>

__declspec(align(16)) struct TestEvent {
	int x;
	int frame;
};

void write(EventStream<TestEvent>* stream) {
	for (int n = 0; n < 10; n++) {
		stream->BeginWrite();
		std::cout << "Began writing frame " << n << std::endl;
		for (int i = 0; i < 10; i++) {
			TestEvent* ev_in = stream->PushEvent();
			ev_in->x = i;
			ev_in->frame = n;
			std::cout << "wrote " << ev_in->x << " during frame " << ev_in->frame << std::endl;
		}
		std::cout << "Done writing frame " << n << std::endl;
		stream->EndWrite();
	}
	
}

void read(EventStream<TestEvent>* stream) {
	for (int n = 0; n < 10; n++) {
		stream->BeginRead();
		std::cout << "Began reading frame " << n << std::endl;
		for (int i = 0; i < 10; i++) {
			TestEvent* ev_out = stream->PopEvent();
			if (ev_out != nullptr) {
				std::cout << "read " << ev_out->x << " from frame " << ev_out->frame << std::endl;
			}
		}
		std::cout << "Done reading frame " << n << std::endl;
		stream->EndRead();
	}
}

int main(int argc, char* argv[]) {
	EventStream<TestEvent> stream(5);

	std::thread write_thread(write, &stream);
	std::thread read_thread(read, &stream);

	stream.Initialize();

	write_thread.join();
	read_thread.join();

	//yucky
	system("PAUSE");
	return 0;
}