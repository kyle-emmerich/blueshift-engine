#pragma once
#include <exception>
#include <stdexcept>
#include <sstream>

namespace Blueshift {

	class LogicError : public std::logic_error {
	public:
		LogicError(std::string str) : std::logic_error(str) {}
	};

	class Formatter {
	private:
		std::stringstream ss;
	public:
		Formatter() {}
		Formatter(const Formatter&) = delete;
		Formatter(const Formatter&&) = delete;
		~Formatter() {}

		template<typename T>
		Formatter& operator << (const T& val) {
			ss << val;
			return *this;
		}

		operator std::string() const { return ss.str(); }
	};

	class RuntimeError : public std::runtime_error {
	public:
		RuntimeError(std::string str);
	};


}