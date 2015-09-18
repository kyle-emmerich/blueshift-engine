#include "String.h"

String::String() {

}

String::String(const char* Buffer) {
	_internal = Buffer;
}

String::String(char* Buffer) {
	_internal = Buffer;
}

String::String(String& other) {
	_internal = other._internal;
}

String::~String() {

}

const char* String::GetBuffer() const {
	return _internal.c_str();
}

uint32_t String::GetLength() const {
	return (uint32_t)_internal.length();
}

String& String::operator= (const char Buffer[]) {
	return String(Buffer);
}

String& String::operator= (char* Buffer) {
	return String(Buffer);
}