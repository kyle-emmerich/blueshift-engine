#pragma once
#include "Core/IApplication.h"
#include "Core/Utility/ConfigFile.h"
#include "Core/UUID.h"
#include "Core/Exceptions.h"
#include "Core/ISubsystem.h"
#include "Core/Timing/Stopwatch.h"
#include "Core/DebugConsole.h"
#include <typeindex>

namespace Blueshift {
	const int EngineVersion[] = { 0, 0, 1 };
#if _DEBUG
	const bool EngineDebugBuild = true;
#else
	const bool EngineDebugBuild = false;
#endif

	namespace Graphics {
		class RenderSystem;
	}
	namespace Storage {
		class FileSystem;
		class File;
	}
	namespace Core {

		/*! \struct EngineParameters
			\brief Specifies parameters with which to configure the engine for the application.
		*/
		struct EngineParameters {
			/*! \brief The type of application, see enum ApplicationType.
			*/
			ApplicationType Type;
			/*! \brief The name of the application.
			*/
			std::string ApplicationName;
			/*! \brief The identity of the application, used for the write directory name. Should be filesystem friendly.
			*/
			std::string ApplicationIdentity;

			/*! \brief Whether or not the engine will log to stdout. 
			*/
			bool LogToStdOut = true;
			/*! \brief Whether or not the engine will automatically log exceptions.
			*/
			bool AutoLogErrors = true;

			/*! \brief A list of paths to search for assets on; the read directories. Can include .zip files. 
			*/
			std::vector<std::string> SearchPaths;

			/*! \brief A pointer to the application itself.
			*/
			IApplication* Application;
		};

		enum class LogLevel {
			Notice		= 0, /// Everything will be logged, even debug notices.
			Lua			= 1, /// Lua output will be logged, along with errors and warnings.
			LuaError	= 2, /// Only Lua errors will be logged, along with engine errors and warnings.
			Warning		= 3, /// Nothing from Lua will be logged, only engine errors and warnings.
			Error		= 4  /// Only engine errors will be logged, no warnings and no Lua errors or output.
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
			/*! \brief Returns the singleton instance of the engine.
			*/
			inline static Engine& Get() {
				return *_instance;
			}
			/*! \brief Destroys the singleton instance of the engine, should be called only at the end of the application.
			*/
			inline static void Destroy() {
				delete _instance;
			}
			/*! \brief Constructs the singleton instance of the engine using the parameters provided.
			*/
			Engine(EngineParameters* Parameters, int argc, char* argv[]);
			~Engine();

			/*! \brief Allocates and constructs the subsystem of type T and returns a pointer to it.
				Type registered via Engine::CreateSystem() should implement ISubsystem.
			*/
			template<typename T>
			inline T* CreateSystem() {
				T* system = new T(this);
				system->Type = typeid(T);
				subsystems[typeid(T)] = dynamic_cast<ISubsystem*>(system);
				return system;
			}
			/*! \brief Returns a pointer to the subsystem of type T. 
			*/
			template<typename T>
			inline T* GetSystem() {
				return dynamic_cast<T*>(subsystems[typeid(T)]);
			}
			/*! \brief Queries whether or not the engine has a registered subsystem of type T.
			*/
			template<typename T>
			inline bool HasSystem() const {
				return subsystems.find(typeid(T)) != subsystems.end();
			}

			/*! \brief Returns an immutable instance of the parameters used to instantiate the engine.
			*/
			const EngineParameters* GetParameters() const {
				return parameters;
			}

			/*! \brief Sets the log file to be used by the engine. 
			*/
			inline void SetLogFile(Storage::File* file) {
				log_file = file;
			}
			/*! \brief Logs a message to the log file and optionally to stdout if enabled in the parameters.
				
				\param level Denotes the severity of the log message, allowing the engine to provide filtering.
				\param message The message to be logged using \n for newlines.
			*/
			void Log(LogLevel level, std::string message);
			/*! \brief Logs a RuntimeError exception to the log file and optionally to stdout if enabled in the parameters. 
			
				\param err The RuntimeError to be logged.
				\param is_auto Used internally to filter out automatic exception logging, leave false.
			*/
			void Log(const RuntimeError& err, bool is_auto = false);
			/*! \brief Queries whether or not automatic error logging is enabled.
			*/
			bool IsAutoLogErrorsOn() const {
				return parameters->AutoLogErrors;
			}
			/*! \brief Sets whether or not automatic error logging is enabled.
			*/
			void SetAutoLogErrors(bool autolog) {
				parameters->AutoLogErrors = autolog;
			}
			/*! \brief Queries the current log filter level.
				If a message is logged at a lower level than the current log level, it will be filtered out.
			*/
			inline LogLevel GetLogLevel() const {
				return log_level;
			}
			/*! \brief Sets the current log filter level.
				If a message is logged at a lower level than the current log level, it will be filtered out.
			*/
			inline void SetLogLevel(LogLevel level) {
				log_level = level;
			}
			
			/*! \brief Returns a vector of strings of the command line arguments the engine was created with.
			*/
			inline const std::vector<std::string>& GetArgs() const {
				return init_args;
			}

			/*! \brief The engine's internal timer.
			*/
			Timing::Stopwatch Timer;
			/*! \brief The debug console used by the engine.
			*/
			Core::DebugConsole Console;
		};

	}
}