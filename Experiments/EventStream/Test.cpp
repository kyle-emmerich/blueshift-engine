#include "EventStream.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;

struct TestEvent {
	int x;
};

void write(EventStream<TestEvent>* stream) {
	int base = 0;
	while (true) {
		//keep writing events til the end of time
		for (int i = 0; i < 5; i++) {
			TestEvent* ev = stream->Lock();
			ev->x = base + i;
			cout << "Writing " << ev->x << std::endl;
			stream->Unlock();
		}
		base += 5;

		//guess we can lighten the cpu load though
		std::this_thread::sleep_for(1000ms);
	}
}

void read(EventStream<TestEvent>* stream) {
	while (true) {
		for (int i = 0; i < 5; i++) {
			TestEvent* ev = stream->Read();
			if (ev) {
				cout << "Read " << ev->x << endl;
			}
		}

		std::this_thread::sleep_for(900ms);
	}
}

int main(int argc, char* argv[]) {
	EventStream<TestEvent> stream(25);

	thread write_thread(write, &stream);
	thread read_thread(read, &stream);


	write_thread.join();
	read_thread.join();

	//yucky
	system("PAUSE");
	return 0;
}