#pragma once
#include "physfs.h"
#include "Core/Exceptions.h"
#include <vector>
#include <string>

namespace Blueshift {
	namespace Storage {

		enum class FileOpenMode {
			Read = 0,
			Write,
			Append
		};

		class FileError : public RuntimeError {
		public:
			FileError(std::string str) : RuntimeError(str) {}
		};

		class FileOpenError : public FileError {
		public:
			FileOpenError(std::string Path, std::string Error)
				: FileError(Formatter() << "Could not open file \"" << Path << "\": " << Error) {}
		};
		class FileNotReadableError : public FileError {
		public:
			FileNotReadableError(std::string Path, std::string Error)
				: FileError(Formatter() << "Could not read from file \""<<Path<<"\": " << Error) {}
		};
		class FileNotWritableError : public FileError {
		public:
			FileNotWritableError(std::string Path, std::string Error)
				: FileError(Formatter() << "Could not write to file \"" << Path << "\": " << Error) {}
		};

		class File {
		private:
			std::string path;
			FileOpenMode mode;
			bool open = false;
			bool dangling = false;

			PHYSFS_File* file = nullptr;

			inline void ensure_writable() {
				if (!open) {
					throw FileNotWritableError(path, "the file is not open.");
				}
				if (mode != FileOpenMode::Write && mode != FileOpenMode::Append) {
					throw FileNotWritableError(path, "the file was not opened for writing/appending.");
				}
			}
			inline void ensure_readable() {
				if (!open) {
					throw FileNotReadableError(path, "the file is not open.");
				}
				if (mode != FileOpenMode::Read) {
					throw FileNotWritableError(path, "the file was not opened for reading.");
				}
			}

			File(PHYSFS_File* f) : file(f) {
				dangling = true;
				open = true;
			}

			size_t write_bytes(const uint8_t* Data, size_t Size, size_t Count = 1);
			size_t read_bytes(uint8_t* Data, size_t Size, size_t Count = 1);
		public:
			File(std::string Path, FileOpenMode Mode = FileOpenMode::Read, size_t BufferSize = 0);
			~File();

			size_t GetLength();

			
			template<typename T>
			inline size_t Write(const T* Data) {
				ensure_writable();
				return write_bytes((const uint8_t*)Data, sizeof(T));
			}
			template<typename T>
			inline size_t Write(const T* Data, size_t Size) {
				ensure_writable();
				return write_bytes((const uint8_t*)Data, Size);
			}
			template<typename T>
			inline size_t Write(const std::vector<T>& Data) {
				ensure_writable();
				if (Data.size() == 0) {
					throw LogicError("Cannot write 0 items to a file.");
				}
				return write_bytes((const uint8_t*)&Data[0], sizeof(T) * Data.size());
			}
			inline size_t Write(std::string str) {
				ensure_writable();
				return Write<char>(str.c_str(), str.size());
			}

			template<typename T>
			inline size_t Read(T* Data) {
				ensure_readable();
				return read_bytes((uint8_t*)Data, sizeof(T));
			}
			template<typename T>
			inline size_t Read(T* Data, size_t Size) {
				ensure_readable();
				return read_bytes((uint8_t*)Data, Size);
			}
			template<typename T>
			inline size_t Read(std::vector<T>& Data, size_t Count) {
				ensure_readable();
				if (Count == 0) {
					throw LogicError("Cannot read 0 items from a file.");
				}
				Data.resize(Count);
				return read_bytes((uint8_t*)&Data[0], sizeof(T), Count);
			}
			inline size_t Read(std::string& str, size_t Count) {
				ensure_readable();
				str.resize(Count);
				return Read<char>(&str[0], Count);
			}

			inline std::string GetPath() const {
				return path;
			}

			bool Close();
		};

	}
}