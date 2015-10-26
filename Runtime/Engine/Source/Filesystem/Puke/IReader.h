#pragma once
#include <istream>

namespace Blueshift {
	namespace Filesystem {
		namespace Puke {

			class IReader {
			public:
				IReader() = delete;
				IReader(IReader&) = delete;
				IReader(IReader&&) = delete;
				IReader(std::istream&) {}
			};

		}
	}
}