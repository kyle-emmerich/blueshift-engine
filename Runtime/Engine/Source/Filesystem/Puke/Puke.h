#pragma once
#include "Filesystem/Puke/IReader.h"

#include "Filesystem/Puke/Version1/Container.h"
#include "Filesystem/Puke/Version1/Reader.h"

namespace Blueshift {
	namespace Filesystem {
		namespace Puke {
			struct StubHeader {
				static const size_t StaticLength = 4;
				struct {
					uint8_t B;
					uint8_t S;
				} Magic;
				uint16_t Version;
			};

			class NotPukeFormatException : public std::runtime_error {
			public:
				NotPukeFormatException() : std::runtime_error("Not a valid .puke format file") {}
			};
			class UnsupportedPukeVersionException : public std::runtime_error {
			public:
				UnsupportedPukeVersionException() : std::runtime_error("This .puke file is of an unsupported or invalid version.") {}
			};

			inline IReader* OpenReader(std::istream& is) {
				//Mark the beginning location (we don't know if this stream
				//is just our file.)
				std::streampos beginning = is.tellg();
				//Load the stub header.
				StubHeader* header = new StubHeader;
				is >> (char*)header;
				//Seek back to the beginning.
				is.seekg(beginning);

				//Figure out the version to use.
				switch (header->Version) {
				case 1:
					return (IReader*)new Version1::Reader(is);
				default:
					throw UnsupportedPukeVersionException();
				}
			}
		}
	}
}