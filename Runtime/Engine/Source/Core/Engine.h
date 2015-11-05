#pragma once
#include "Core/IApplication.h"
#include "Core/Utility/ConfigFile.h"
#include "Core/UUID.h"
#include "Core/Exceptions.h"
#include "Core/ISubsystem.h"
#include "Core/Timing/Stopwatch.h"
#include <typeindex>

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

			bool LogToStdOut = true;
			bool AutoLogErrors = true;

			std::vector<std::string> SearchPaths;

			IApplication* Application;
		};

		enum class LogLevel {
			Notice		= 0,
			Warning		= 1,
			Error		= 2
		};

		class Engine {
		private:
			std::map<std::type_index, ISubsystem*> subsystems;
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

			template<typename T>
			inline T* CreateSystem() {
				T* system = new T(this);
				system->Type = typeid(T);
				subsystems[typeid(T)] = system;
				return system;
			}
			template<typename T>
			inline T* GetSystem() const {
				return static_cast<T*>(&subsystems[typeid(T)]);
			}
			template<typename T>
			inline bool HasSystem() const {
				return subsystems.find(typeid(T)) != subsystems.end();
			}

			const EngineParameters* GetParameters() const {
				return parameters;
			}

			inline void SetLogFile(Storage::File& file) {
				log_file = &file;
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
			
			inline const std::vector<std::string>& GetArgs() const {
				return init_args;
			}

			Timing::Stopwatch Timer;
		};

	}
}