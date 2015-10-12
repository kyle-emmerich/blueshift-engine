#pragma once

namespace Blueshift {
	namespace Core {

		enum class ApplicationType {
			Undefined,
			Client,
			Server,
			Tools
		};

		struct EngineParameters;

		class IApplication {
		public:
			virtual void Initialize() = 0;
			virtual void Shutdown() = 0;

			virtual bool Update(double) = 0;

			virtual bool IsRunning() = 0;
		};
	}
}