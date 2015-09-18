#pragma once
#include <string>

#include "Core/BasicTypes/BaseString.h"

class StaticString : public BaseString {
private:
	//temporary
	//TODO: Finish this!
	std::string _internal;
public:
	StaticString();
	StaticString(const char* Buffer);
	StaticString(char* Buffer);
	StaticString(StaticString&);

	~StaticString();

	const char* GetBuffer() const;
	uint32_t GetLength() const;

	StaticString& operator= (const char Buffer[]);
	StaticString& operator= (char* Buffer);
};