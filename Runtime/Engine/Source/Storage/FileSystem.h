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

		/*! \class FileSystem
			\brief Subsystem that manages access to the virtual filesystem.
		
			The FileSystem subsystem is responsible for managing all file access in the engine. It uses physfs to provide a safe virtual
			filesystem with write access limited to a single directory and read access limited to specified folders. Read directories can
			be mounted from both filesystem paths and .zip archives.

			The subsystem has two built-in read paths: Assets and Scripts.
		*/
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

			/*! \brief Mounts a directory or .zip archive to the read search path.

				\param PackagePath The real filesystem path to the directory or archive to be mounted. Unless working directory is changed, this should be relative to the Blueshift executable.
				\param MountTo The virtual filesystem path to mount this package to. For example, if "/package" is supplied and file "bacon.png" exists in the package, it should be accessed at "/package/bacon.png".
				\param Prepend Whether or not to prepend to the search path. Almost always leave this true; this means that this package will override any earlier packages (such as the built-ins.)
			*/
			void MountPackage(std::string PackagePath, std::string MountTo = "/", bool Prepend = true);
		
			/*!	\brief Retrieves the last modified time of a file as 64-bit seconds since Jan. 1st, 1970.
				
				\param A filename in the virtual filesystem.
			*/
			uint64_t GetLastModifiedTime(std::string Filename);
			/*!	\brief Retrieves the real path of a file in the virtual filesystem.

				\param A filename in the virtual filesystem. Can be a directory.
			*/
			std::string GetRealPath(std::string Filename);

			/*! \brief Queries whether or not the given filename exists in the virtual filesystem.
			*/
			bool FileExists(std::string Filename);
			/*! \brief Queries whether or not the given path exists in the virtual filesystem.
			*/
			bool PathExists(std::string Pathname);
			/*!	\brief Queries whether or not the given path in the virtual filesystem is a directory or a file.
				
				\returns True if Path is a directory, false if Path is a file.
			*/
			bool IsDirectory(std::string Path);
		};

	}
}