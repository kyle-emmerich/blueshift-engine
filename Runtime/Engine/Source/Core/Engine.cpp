#include "Core/Engine.h"
#include "bgfx/bgfx.h"
#include "Graphics/VertexDeclarations.h"
#include "Platform/PlatformSpecific.h"
#include "Graphics/RenderSystem.h"
#include "Storage/FileSystem.h"
#include <vector>
#include <iostream>
#include <ctime>

using namespace Blueshift;
using namespace Blueshift::Core;
using namespace Blueshift::Graphics;
using namespace Blueshift::Platform;

Engine* Engine::_instance = 0;

Engine::Engine(EngineParameters* Parameters, int argc, char* argv[]) {
	Engine::_instance = this;

	parameters = Parameters;
	
	for (int i = 0; i < argc; i++) {
		init_args.push_back(std::string(argv[i]));
	}
	if (argc == 0) {
		init_args.push_back("");
	}
}

Engine::~Engine() {
	for (auto it = subsystems.begin(); it != subsystems.end(); it++) {
		delete it->second;
	}
	if (log_file != nullptr) {
		delete log_file;
	}
	delete parameters;
}

void Engine::Log(LogLevel level, std::string message) {
	std::stringstream ss; 
	
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);
	char buf[80];
	std::strftime(buf, sizeof(buf), "%F %T", now);

	ss << "[" << buf << "] ";
	switch (level) {
	case LogLevel::Warning:
		ss << "Warning: ";
		break;
	case LogLevel::Error:
		ss << "Error! ";
		break;
	default:
		ss << "Notice: ";
		break;
	}
	ss << message << std::endl;

	std::string final_msg = ss.str();

	if (parameters->LogToStdOut) {
		std::cout << final_msg;
	}
	if (log_file != nullptr) {
		log_file->Write(final_msg);
	}
}

void Engine::Log(const RuntimeError& err, bool is_auto) {
	if (!is_auto && parameters->AutoLogErrors) {
		return;
	}
	Log(LogLevel::Error, err.what());
}