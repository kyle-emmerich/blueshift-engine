#pragma once
#include <map>
#include <string>
#include <sstream>

namespace Blueshift {
	namespace Core {
		namespace Utility {

			class ConfigFile {
			private:
				bool initialized = false;
				std::map<std::string, std::string> options;

				std::string current_section;
				void read_line(std::string line);
			public:
				ConfigFile() = default;
				explicit ConfigFile(std::string path);

				inline bool IsInitialized() { return initialized; }

				bool ReadStream(std::istream& stream);
				bool ReadFile(std::string path);
				bool ReadString(std::string data);

				inline std::string Get(const std::string& section, const std::string& name) const {
					auto it = options.find(section + '/' + name);
					if (it == options.end()) {
						throw 0;
					}
					return it->second;
				}

				template<typename T>
				inline const T Get(const std::string& section, const std::string& name, const T&& default = 0) const {
					T rv;
					std::stringstream ss;
					try {
						ss << Get(section, name);
						ss >> rv;
					} catch (...) {
						rv = default;
					}
					return rv;
				}

				template<>
				inline const bool Get(const std::string& section, const std::string& name, const bool&& default) const {
					try {
						std::string val = Get(section, name);
						if (val == "true" || val == "1") {
							return true;
						}
						else if (val == "false" || val == "0") {
							return false;
						}
					} catch (...) {
						return default;
					}
					return default;
				}

				bool Has(const std::string& section, const std::string& name) const {
					auto it = options.find(section + '/' + name);
					return it != options.end();
				}
			};

		}
	}
}