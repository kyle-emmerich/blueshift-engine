#include "Filesystem/Puke/Version1/Reader.h"

using namespace Blueshift;
using namespace Filesystem;
using namespace Puke;

Version1::Reader::Reader(std::istream& is) {
	container = new Version1::Container;
	is.read(reinterpret_cast<char*>(container->ContainerHeader), sizeof(Version1::Container::Header::StaticLength));

	if (container->ContainerHeader->Magic.B != 'B' ||
		container->ContainerHeader->Magic.S != 'S') {
		//throw NotPukeFormatException();
	}

}