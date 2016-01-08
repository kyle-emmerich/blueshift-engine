#pragma once
#include "physfs.h"
#include "Core/Exceptions.h"
#include <vector>
#include <string>

namespace Blueshift {
	namespace Storage {

		const size_t MAX_STRING_BUFFER_SIZE = 256 * 1024;

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

		class FileNoNullTerminatorFound : public FileError {
		public:
			FileNoNullTerminatorFound(std::string Path)
				: FileError(Formatter() << "Tried to read too many bytes from file \"" << Path << "\" while searching for a null terminator in a string. Are you sure there is one?") {}
		};

		/*!	\class File
			\brief Represents an opened file in the virtual filesystem managed by FileSystem.

			This interface is much like a stream and uses physfs underneath. You can read or write any
			POD type, or you can write plain bytes (uint8_t.)

			The file will be closed when the destructor is called, so if a file is only needed for a short
			time, it should be allocated on the stack and allowed to go out of scope.	

			\sa FileSystem
		*/
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
			/*! \brief Opens a file according to the supplied mode, and optionally allows a buffer size to be set.

				This internally uses physfs, so the path should be relative to the root of the virtual filesystem.
				That means that the first file found in any of the mounted directories will be used; the built-in
				directories (Assets, Scripts) have the lowest priority so that mounted directories/archives may
				override them.

				\param Path A path to a file in the virtual filesystem
				\param Mode The mode to open with; keep in mind that files can only be written to the write directory, see FileSystem for more info.
				\param BufferSize The size of the buffer in bytes, 0 for unbuffered mode. When buffering is enabled on a file, read operations will come from
								  the buffer until it is exhausted, and then the buffer will be refilled. This can be useful for .zip archives because decompression
								  happens while buffering, so the CPU cost can be offset a little.
			*/
			File(std::string Path, FileOpenMode Mode = FileOpenMode::Read, size_t BufferSize = 0);
			/*!	\brief Closes the file. 
				
				If the file is unable to be closed, it will be considered dangling and will be checked into the FileSystem to be handled later.
			*/
			~File();

			/*! \brief Returns the length of the file in bytes; may not be accurate!
				
				This method will *attempt* to read the length of the file, but it may fail to do so correctly if the file is in a streamed
				archive or if another process/thread is writing to it. Be careful, or just don't use it at all if it can be helped.
			*/
			size_t GetLength();

			/*!	\brief Writes a single object of type T to the file and advances the internal pointer; T must be a plain-old-data type.
			*/
			template<typename T>
			inline size_t Write(const T* Data) {
				ensure_writable();
				return write_bytes((const uint8_t*)Data, sizeof(T));
			}
			/*!	\brief Writes multiple objects of type T to the file and advances the internal pointer; T must be a plain-old-data type.
			*/
			template<typename T>
			inline size_t Write(const T* Data, size_t Size) {
				ensure_writable();
				return write_bytes((const uint8_t*)Data, Size);
			}
			/*!	\brief Writes a vector of objects of type T to the file and advances the internal pointer; T must be a plain-old-data type.
			*/
			template<typename T>
			inline size_t Write(const std::vector<T>& Data) {
				ensure_writable();
				if (Data.size() == 0) {
					throw LogicError("Cannot write 0 items to a file.");
				}
				return write_bytes((const uint8_t*)&Data[0], sizeof(T) * Data.size());
			}
			/*! \brief Writes a string to the file and advances the internal pointer.
			*/
			inline size_t Write(std::string str) {
				ensure_writable();
				return Write<char>(str.c_str(), str.size());
			}

			/*!	\brief Reads a single object of type T from the file and advances the internal pointer; T must be a plain-old-data type.
			*/
			template<typename T>
			inline size_t Read(T* Data) {
				ensure_readable();
				return read_bytes((uint8_t*)Data, sizeof(T));
			}
			/*! \brief Reads multiple objects of type T from the file and advances the internal pointer; T must be a plain-old-data type.
			*/
			template<typename T>
			inline size_t Read(T* Data, size_t Size) {
				ensure_readable();
				return read_bytes((uint8_t*)Data, Size);
			}
			/*! \brief Reads into a vector of objects of type T from the file and advances the internal pointer; T must be a plain-old-data type.
			*/
			template<typename T>
			inline size_t Read(std::vector<T>& Data, size_t Count) {
				ensure_readable();
				if (Count == 0) {
					throw LogicError("Cannot read 0 items from a file.");
				}
				Data.resize(Count);
				return read_bytes((uint8_t*)&Data[0], sizeof(T), Count);
			}
			/*!	\brief Reads a string of known length from the file and advances the internal pointer.
			*/
			inline size_t Read(std::string& str, size_t Length) {
				ensure_readable();
				str.resize(Length);
				return Read<char>(&str[0], Length);
			}
			/*! \brief Reads a null-terminated string from the file and advances the internal pointer; use other variant if length is known.

				This method will only read up to 256kb or to the end of the file before throwing a FileNoNullTerminatorFound exception. If it's not
				certain that a null-terminator exists, it's better to use the variant that takes a known length.
			
			*/
			inline size_t Read(std::string& str) {
				ensure_readable();
				//now we get to do things the slower way.
				size_t i = 0;
				size_t buffer_size = 256;
				char* buffer = (char*)malloc(buffer_size);
				bool null_found = false;

				//definitely don't read more than 256kb, that'd be crazy.
				while (buffer_size < MAX_STRING_BUFFER_SIZE) {
					//read some
					size_t bytes_read = read_bytes((uint8_t*)&buffer[i], 256, 1);
					if (bytes_read == 0) {
						throw FileNoNullTerminatorFound(path);
					}
					//find a null-terminator
					null_found = false;
					for (i; i < buffer_size; i++) {
						if (buffer[i] == '\0') {
							buffer_size = i;
							null_found = true;
							break;
						}
					}
					//if no null-terminator is found, reallocate and try some more
					if (!null_found) {
						buffer_size += 256;
						buffer = (char*)realloc(buffer, buffer_size);
					}
				}

				if (buffer_size > MAX_STRING_BUFFER_SIZE) {
					throw FileNoNullTerminatorFound(path);
				}

			}

			/*! \brief Seeks to a given position in bytes in the file.
			*/
			void Seek(size_t pos);
			/*!	\brief Returns the current internal pointer position in bytes.
			*/
			size_t Tell() const;

			/*! \brief Returns true if the internal pointer is at the end of the file.
			*/
			inline bool IsAtEOF() const {
				return PHYSFS_eof(file) != 0;
			}

			/*! \brief Returns the virtual filesystem path the file was opened at.
			*/
			inline std::string GetPath() const {
				return path;
			}

			/*! \brief Explicitly closes the file; not necessary since the destructor will do it for you.
			*/
			bool Close();
		};

	}
}