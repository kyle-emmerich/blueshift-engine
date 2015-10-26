#pragma once
#include <inttypes.h>
#include "Core/UUID.h"

namespace Blueshift {
	namespace Filesystem {
		namespace Puke {
			namespace Version1 {

#pragma pack(push, PUKE_CONTAINER, 4)
				struct Container {
					struct Header {
						static const size_t StaticLength = 8;
						struct {
							uint8_t B;
							uint8_t S;
						} Magic;
						uint16_t Version;
						uint16_t NumEntries;
						uint16_t Unused0000;

						struct {
							uint32_t Offset;
							uint32_t Length;
						}*EntryPositions;
					};
					struct EntryHeader {
						Core::UUID ID;
						uint64_t BodyOffset;
						uint64_t BodyLength;
						struct {
							uint64_t Low;
							uint64_t High;
						} Hash;
						uint32_t ContentRevision;

						uint8_t FormatType;
						uint8_t Format;
						uint8_t FormatVersion;
						uint8_t CompressionFlags;
						uint16_t FormatFlags;

						uint16_t PathLength;
						char* Path;
					};
					Header* ContainerHeader;
					EntryHeader* Entries;
				};
#pragma pack(pop, PUKE_CONTAINER)

			}
		}
	}
}