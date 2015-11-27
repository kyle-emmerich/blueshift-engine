#pragma once
#include "Storage/File.h"
#include "Storage/IAsset.h"
#include <atomic>

namespace Blueshift {
	namespace Storage {
		
		class ILoader {
		protected:
			Storage::File* file;
			std::atomic<uint64_t> progress;
			std::atomic<uint64_t> total_progress;

		public:
			ILoader() = delete;
			ILoader(Storage::File* f) : file(f), progress(0), total_progress(0) {}
			virtual ~ILoader() {}

			virtual void Load() = 0;

			inline virtual Storage::File* GetFile() const {
				return file;
			}
			double GetProgress() const {
				if (total_progress == 0) {
					return 0.0;
				}
				return static_cast<double>(progress) / static_cast<double>(total_progress);
			}
			virtual Storage::IAsset* GetAsset() = 0;
		};

	}
}