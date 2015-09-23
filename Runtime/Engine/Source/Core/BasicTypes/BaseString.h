#pragma once

namespace Blueshift {
	namespace Core {
		namespace BasicTypes {

			class BaseString {
			public:
				virtual const char* GetBuffer() const = 0;
				virtual uint32_t GetLength() const = 0;
				virtual BaseString& operator= (const char[]) = 0;
				virtual BaseString& operator= (char* Buffer) = 0;
			};

		}
	}
}