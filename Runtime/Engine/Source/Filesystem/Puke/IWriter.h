#pragma once
#include <ostream>

namespace Blueshift {
	namespace Filesystem {
		namespace Puke {

			class IWriter {
			public:
				IWriter() = delete;
				IWriter(IWriter&) = delete;
				IWriter(IWriter&&) = delete;
				IWriter(std::ostream&) {}
			};

		}
	}
}