#include <iostream>
using namespace std;

class PlainOldData {
public:
	int x = 0;
	float y = 250567868.0f;

	PlainOldData() {
		x = 5;
		y = 290.0f;
	}
};

class NotPlainOldData {
public:
	int x = 0;
	float y = 250567868.0f;

	NotPlainOldData() {
		x = 5;
		y = 290.0f;
	}

	//this will make this not POD
	NotPlainOldData(NotPlainOldData& to_copy) {
		//........
	}

	//this also will make it not POD; a move or copy assignment operator
	NotPlainOldData& operator=(NotPlainOldData&& other) {

	}

	//this will do it too...
	virtual void vfunc() {
		//...
	}

	//but also this because of heap allocations
	char* pointer;
};

int main() {

	//so... what can you do with POD and non-POD?
	PlainOldData* pod1 = new PlainOldData;
	PlainOldData* pod2 = new PlainOldData;

	//we can memcpy pod types
	memcpy(pod1, pod2, sizeof(PlainOldData));

	//but this should freak the fuck out.
	NotPlainOldData* npod1 = new NotPlainOldData;
	NotPlainOldData* npod2 = new NotPlainOldData;

	memcpy(npod1, npod2, sizeof(NotPlainOldData));

	return 0;
}