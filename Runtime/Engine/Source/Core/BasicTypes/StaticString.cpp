#include "StaticString.h"

using namespace Blueshift;
using namespace Blueshift::Core::BasicTypes;

StaticString::StaticString() {

}

StaticString::StaticString(const char* Buffer) {
	_internal = Buffer;
}

StaticString::StaticString(char* Buffer) {
	_internal = Buffer;
}

StaticString::StaticString(StaticString& other) {
	_internal = other._internal;
}

StaticString::~StaticString() {

}

const char* StaticString::GetBuffer() const {
	return _internal.c_str();
}

uint32_t StaticString::GetLength() const {
	return (uint32_t)_internal.length();
}

StaticString& StaticString::operator= (const char Buffer[]) {
	return StaticString(Buffer);
}
//todo: fix these, bad stack allocations
StaticString& StaticString::operator= (char* Buffer) {
	return StaticString(Buffer);
}