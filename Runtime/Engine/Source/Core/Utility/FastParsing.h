#pragma once
#include <inttypes.h>

namespace Blueshift {
	namespace Core {
		namespace Utility {

			inline uint32_t ParseUnsignedInt(const char* str, size_t offset, size_t length, size_t& after) {
				uint32_t accum = 0;
				for (size_t i = offset; i < offset + length; i++) {
					char at = str[i];
					if (at >= 48 && at <= 58) {
						uint32_t digit = static_cast<uint32_t>(at - 48);
						accum = (accum * 10) + digit;
					} else {
						after = i + 1;
						return accum;
					}
				}
				return accum;
			}

			//Takes a char* buffer and length with offset, returns position that is 1 after the parsed value.
			inline float ParseFloat(const char* str, size_t offset, size_t length, size_t& after) {
				float accum = 0.0f;
				bool found_numbers = false;
				bool past_decimal = false;
				float sign = 1.0f;

				char at = str[offset];
				uint8_t digit;
				uint32_t pos = 10;

				if (at >= 48 && at <= 58) {
					found_numbers = true;
				}
				for (size_t i = offset; i < offset + length; i++) {
					at = str[i];
					if (!found_numbers) {
						if (i == offset + length - 1) {
							after = 0;
							return 0.0f;
						}
						char next = str[i + 1];
						if (next >= 48 && next <= 58) {
							found_numbers = true;
							if (at == '-') {
								sign = -1.0f;
							}
						}
					} else {
						if (at == '.') {
							past_decimal = true;
						} else if (at == ' ' || at == ',' || at < 48 || at > 58) {
							after = i + 1;
							break;
						} else {
							//convert ascii char to number
							digit = static_cast<uint8_t>(at - 48);
							if (!past_decimal) {
								//before decimal, shift accumulated value up 10
								accum = (accum * 10.0f) + static_cast<float>(digit);
							} else {
								//after decimal, add as fractional value
								accum += digit == 0 ? 0 : static_cast<float>(digit) / static_cast<float>(pos);
								pos *= 10;
								if (pos > 10000000)
									break;
							}
						}
					}
				}
				return sign * accum;
			}

		}
	}
}