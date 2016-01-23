#include "Core/Exceptions.h"

using namespace Blueshift;

RuntimeError::RuntimeError(std::string str)
	: std::runtime_error(str) {
}

LuaError::LuaError(std::string str)
	: RuntimeError(str) {}