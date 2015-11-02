#include "Storage/FileSystem.h"
#include <algorithm>
#include <sys/stat.h>

using namespace Blueshift;
using namespace Storage;

FileSystem::FileSystem(std::string BasePath, std::string Identity, const std::vector<std::string>& SearchPaths) {
	int success = PHYSFS_init(BasePath.c_str());
	if (success == 0) {
		throw FileSystemNotInitiatedError(PHYSFS_getLastError());
	}
	
	std::string write_dir = PHYSFS_getUserDir();
	if (0 == PHYSFS_setWriteDir(write_dir.c_str())) {
		throw FileError(Formatter() << "Could not set write directory to \"" << write_dir <<"\": " << PHYSFS_getLastError());
	}
	if (PHYSFS_exists(Identity.c_str()) == 0) {
		PHYSFS_mkdir(Identity.c_str());
	}
	write_dir += "/" + Identity;
	if (0 == PHYSFS_setWriteDir(write_dir.c_str())) {
		throw FileError(Formatter() << "Could not set write directory to \"" << write_dir << "\": " << PHYSFS_getLastError());
	}

	std::string base = PHYSFS_getBaseDir();
	base += "..";
	MountPackage(base.c_str());
	std::string assets = PHYSFS_getBaseDir();
	assets += "../Assets";
	MountPackage(assets.c_str());

	for (auto path : SearchPaths) {
		MountPackage(path);
	}
}

FileSystem::~FileSystem() {
	//try to close all our dangling files now
	closing = true;
	for (auto file : dangling) {
		delete file;
	}
	dangling.clear();
	PHYSFS_deinit();
}

void FileSystem::MountPackage(std::string PackagePath, std::string MountTo, bool Prepend) {
	if (0 == PHYSFS_mount(PackagePath.c_str(), MountTo.c_str(), Prepend ? 0 : 1)) {
		throw FileSystemSearchPathError(PHYSFS_getLastError());
	}
	paths.push_back(PackagePath);
}
uint64_t FileSystem::GetLastModifiedTime(std::string Filename) {
	return PHYSFS_getLastModTime(Filename.c_str());
}
std::string FileSystem::GetRealPath(std::string Filename) {
	return std::string(PHYSFS_getRealDir(Filename.c_str()));
}