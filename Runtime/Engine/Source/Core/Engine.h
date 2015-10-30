#pragma once
#include "Core/IApplication.h"
#include "Core/Utility/ConfigFile.h"
#include "Core/UUID.h"
#include "Core/Exceptions.h"


namespace Blueshift {
	namespace Graphics {
		class RenderSystem;
	}
	namespace Storage {
		class FileSystem;
		class File;
	}
	namespace Core {

		
		struct EngineParameters {
			ApplicationType Type;
			std::string ApplicationName;
			std::string ApplicationIdentity;

			std::string LogFilePath = "log.txt";
			bool LogToStdOut = true;
			bool AutoLogErrors = true;

			std::vector<std::string> SearchPaths;

			IApplication* Application;

			struct {
				bool Rendering;
				bool Input;
				bool Audio;
				bool Physics;
				bool Networking;
				bool AI;
				bool API;
				bool Storage;
				bool Database;
			} Subsystems;
		};

		enum class LogLevel {
			Notice		= 0,
			Warning		= 1,
			Error		= 2
		};

		class Engine {
		private:
			Graphics::RenderSystem* render = nullptr;
			Storage::FileSystem* filesys = nullptr;

			EngineParameters* parameters;
			std::vector<std::string> init_args;

			Storage::File* log_file;
			LogLevel log_level;
		
			bool stop = false;

			static Engine* _instance;
		public:
			inline static Engine& Get() {
				return *_instance;
			}
			inline static void Destroy() {
				delete _instance;
			}
			Engine(EngineParameters* Parameters, int argc, char* argv[]);
			~Engine();

			Graphics::RenderSystem& GetRenderSystem() const {
				return *render;
			}
			Storage::FileSystem& GetFileSystem() const {
				return *filesys;
			}

			const EngineParameters* GetParameters() const {
				return parameters;
			}

			void Log(LogLevel level, std::string message);
			void Log(const RuntimeError& err, bool is_auto = false);
			bool IsAutoLogErrorsOn() const {
				return parameters->AutoLogErrors;
			}
			void SetAutoLogErrors(bool autolog) {
				parameters->AutoLogErrors = autolog;
			}
			inline LogLevel GetLogLevel() const {
				return log_level;
			}
			inline void SetLogLevel(LogLevel level) {
				log_level = level;
			}
			
		};

	}
}