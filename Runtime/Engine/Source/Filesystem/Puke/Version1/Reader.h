#pragma once
#include "Filesystem/Puke/Version1/Container.h"
#include <istream>
#include <exception>

namespace Blueshift {
	namespace Filesystem {
		namespace Puke {
			namespace Version1 {

				

				class Reader {
				private:
					Container* container;
					std::istream* stream;
				public:
					explicit Reader(std::istream& is);
				};

			}
		}
	}
}