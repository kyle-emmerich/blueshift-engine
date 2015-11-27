#include "Storage/AssetSystem.h"

using namespace Blueshift;
using namespace Storage;

AssetSystem::AssetSystem(Core::Engine* engine) {

}

AssetSystem::~AssetSystem() {
	//Make sure we clean up everything left
	for (auto loader : file_loaders) {
		filetype& type = filetypes[loader.first];
		ILoader* ptr = loader.second;
		type.cleanup_func(ptr);
	}
	filetypes.clear();
	file_loaders.clear();
}

ILoader* AssetSystem::ProcessFile(const std::string& path) {
	//now to figure out the extension, yay...
	std::string extension;
	std::size_t ext_pos = path.rfind('.');
	std::size_t dir_pos = path.rfind('/'); //don't worry, physfs ensures we can use / on any platform

	if (ext_pos < dir_pos && dir_pos != std::string::npos) {
		//it's an extensionless file in a hidden directory, like
		//foo/.bar/readme
		//take the whole name
		extension.append(path.begin() + dir_pos + 1, path.end());
	} else {
		//normal old file extension
		extension.append(path.begin() + ext_pos + 1, path.end());
	}
	
	ILoader* loader = nullptr;
	if (filetypes.find(extension) != filetypes.end()) {
		Storage::File* file = new Storage::File(path);
		filetype& type = filetypes[extension];
		loader = type.create_func(file);
		file_loaders.push_back(std::make_pair(extension, loader));
	} else {
		throw AssetExtensionNotSupportedError(extension);
	}
	return loader;
}

