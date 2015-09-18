#pragma once
#include <string>

#include "Core/BasicTypes/BaseString.h"

class String : public BaseString {
private:
	//temporary
	//TODO: Finish this!
	std::string _internal;
public:
	String();
	String(const char* Buffer);
	String(char* Buffer);
	String(String&);

	~String();

	const char* GetBuffer() const;
	uint32_t GetLength() const;

	String& operator= (const char Buffer[]);
	String& operator= (char* Buffer);
};