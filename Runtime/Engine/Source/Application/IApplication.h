#pragma once
#include <condition_variable>
#include <mutex>

namespace Blueshift {
	namespace Application {

		enum class ApplicationType {
			Undefined,
			Client,
			Server,
			Tools
		};

		class IApplication {
		public:
			virtual void Initialize() = 0;
			virtual void InitializeRenderData() = 0;
			virtual void DestroyRenderData() = 0;
			virtual void Shutdown() = 0;

			virtual bool Update(double) = 0;

			virtual bool IsRunning() = 0;

			std::condition_variable FrameReady;
			bool IsFrameReady = false;
			std::mutex FrameMutex;
		};
	}
}