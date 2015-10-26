#pragma once
#include <inttypes.h>
#include <random>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <string>

namespace Blueshift {
	namespace Core {

		struct UUID {
			uint64_t Low;
			uint64_t High;

			explicit UUID() {
				Low = distribution(generator);
				High = distribution(generator);
			}
			explicit UUID(uint64_t low, uint64_t high)
				: Low(low), High(high) {}

			bool operator==(UUID& other) {
				return Low == other.Low && High == other.High;
			}

			inline friend std::ostream& operator<<(std::ostream& os, const UUID& uuid) {
				os << std::setfill('0') << std::setw(16) << std::setbase(16) << uuid.Low;
				os << "-";
				os << std::setfill('0') << std::setw(16) << std::setbase(16) << uuid.High;
				os << std::flush;
				return os;
			}
		private:
			static std::mt19937_64 generator;
			static std::uniform_int_distribution<uint64_t> distribution;
		};

	
	}
}