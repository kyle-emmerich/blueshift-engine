#include <iostream>
using namespace std;

class test {
private:
	int x = 0;
public:
	void other() { x = 5; }
};

int main() {
	test t;

	test::other(&t);

	cin.get();
	return 0;
}