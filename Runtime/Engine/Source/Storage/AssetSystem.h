#pragma once
#include "Storage/ILoader.h"
#include "Storage/FileSystem.h"
#include "Core/ISubsystem.h"
#include "Core/Exceptions.h"
#include <list>

namespace Blueshift {
	namespace Storage {

		class AssetExtensionNotSupportedError : public RuntimeError {
		public:
			AssetExtensionNotSupportedError(std::string extension)
				: RuntimeError(Formatter() << "The following extension is not registered with the asset system: " << extension) {}
		};

		class AssetSystem : public Core::ISubsystem {
		private:
			struct filetype {
				filetype() : loader_size(0), create_func(nullptr), cleanup_func(nullptr) {}
				std::string extension;
				size_t loader_size;
				std::function<ILoader*(Storage::File*)> create_func;
				std::function<void(ILoader*)> cleanup_func;
			};
			std::map<std::string, filetype> filetypes;
			std::list<std::pair<std::string, ILoader*>> file_loaders;
		public:
			AssetSystem(Core::Engine*);
			virtual ~AssetSystem();

			template<typename T>
			void RegisterFileType(std::string extension) {
				filetype t; 
				t.extension = extension;
				t.loader_size = sizeof(T);
				t.create_func = [](Storage::File* f) -> ILoader* {
					T* loader = new T(f);
					return dynamic_cast<ILoader*>(loader);
				};
				t.cleanup_func = [](ILoader* l) {
					T* loader = dynamic_cast<T*>(l);
					delete loader;
					return;
				};
			}

			ILoader* ProcessFile(const std::string& path);
		};

	}
}