#include "Storage/File.h"
#include "Storage/FileSystem.h"
#include "Core/Engine.h"

using namespace Blueshift;
using namespace Storage;

File::File(std::string Path, FileOpenMode Mode, size_t BufferSize)
	: path(Path), mode(Mode) {
	switch (Mode) {
	case FileOpenMode::Read:
		file = PHYSFS_openRead(Path.c_str());
		break;
	case FileOpenMode::Write:
		file = PHYSFS_openWrite(Path.c_str());
		break;
	case FileOpenMode::Append:
		file = PHYSFS_openAppend(Path.c_str());
	}

	open = (file != nullptr);
	if (!open) {
		throw FileOpenError(path, std::string(PHYSFS_getLastError()));
	}

	if (BufferSize > 0) {
		PHYSFS_setBuffer(file, BufferSize);
	}
}

File::~File() {
	if (dangling) {
		//whoa there, we can't close just yet.
		//since this instance is going away, make a copy
		//on the heap that just stores it for the dangling list.
		File* dangle_copy = new File(file);
		Core::Engine::Get().GetSystem<Storage::FileSystem>()->dangling.push_back(dangle_copy);
	}
	if (file != nullptr) {
		Close();
	}
}

size_t File::GetLength() {
	return PHYSFS_fileLength(file);
}

size_t File::write_bytes(const uint8_t* Data, size_t Size, size_t Count) {
	ensure_writable();
	return static_cast<size_t>(PHYSFS_write(
		file, 
		Data, 
		static_cast<PHYSFS_uint32>(Size), 
		static_cast<PHYSFS_uint32>(Count))) * Size;
}

size_t File::read_bytes(uint8_t* Data, size_t Size, size_t Count) {
	ensure_readable();
	return static_cast<size_t>(PHYSFS_read(
		file, 
		Data, 
		static_cast<PHYSFS_uint32>(Size),
		static_cast<PHYSFS_uint32>(Count))) * Size;
}

void File::Seek(size_t pos) {
	PHYSFS_seek(file, pos);
}

size_t File::Tell() const {
	int64_t pos = PHYSFS_tell(file);
	if (pos != -1) {
		return static_cast<size_t>(pos);
	}
	//TODO: throw exception???
	return 0;
}

bool File::Close() {
	FileSystem* filesys = Core::Engine::Get().GetSystem<Storage::FileSystem>();
	int success = PHYSFS_close(file);
	if (success != 0) {
		if (dangling && !filesys->closing) {
			//remove ourself from the dangling list
			for (auto it = filesys->dangling.begin(); it != filesys->dangling.end(); it++) {
				if ((*it)->file == this->file) {
					dangling = false;
					filesys->dangling.erase(it);
					break;
				}
			}
		}

		open = false;
		file = nullptr;
		return true;
	} else {
		//we're now dangling
		open = true;
		if (!dangling && !filesys->closing) {
			dangling = true;
			filesys->dangling.push_back(this);
		}
		return false;
	}
}