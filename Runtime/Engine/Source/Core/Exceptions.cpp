#include "Core/Exceptions.h"
#include "Core/Engine.h"

using namespace Blueshift;

RuntimeError::RuntimeError(std::string str)
	: std::runtime_error(str) {
	Core::Engine& engine = Core::Engine::Get();
	if (engine.IsAutoLogErrorsOn()) {
		engine.Log(*this, true);
	}
}

LuaError::LuaError(std::string str)
	: std::runtime_error(str) {
	Core::Engine& engine = Core::Engine::Get();
	engine.Log(Core::LogLevel::LuaError, str);
}