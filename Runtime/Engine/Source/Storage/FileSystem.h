#pragma once
#include "physfs.h"
#include <string>
#include <vector>

#include "Core/Engine.h"
#include "Storage/File.h"
#include "Storage/AssetSystem.h"

#include "Core/ISubsystem.h"

namespace Blueshift {
	namespace Storage {

		class FileSystemNotInitiatedError : public FileError {
		public:
			FileSystemNotInitiatedError(std::string Error)
				: FileError(Formatter() << "Something went horribly wrong with the filesystem: " << Error) {}
		};

		class FileSystemSearchPathError : public FileError {
		public:
			FileSystemSearchPathError(std::string Error)
				: FileError(Error) {}
		};

		class FileSystem : public Core::ISubsystem {
		protected:
			//TODO: make dangling list thread-safe
			std::vector<File*> dangling;
			bool closing = false;

			std::vector<std::string> paths;

			friend class File;
		public:
			FileSystem(Core::Engine* engine);
			~FileSystem();

			void MountPackage(std::string PackagePath, std::string MountTo = "/", bool Prepend = true);
		
			uint64_t GetLastModifiedTime(std::string Filename);
			std::string GetRealPath(std::string Filename);

			bool FileExists(std::string Filename);
			bool PathExists(std::string Pathname);
			bool IsDirectory(std::string Path);
		};

	}
}