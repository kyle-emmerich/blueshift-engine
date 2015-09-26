#include "Core/Utility/ConfigFile.h"
#include <fstream>
#include <cctype>
#include <string>
#include <algorithm>

inline std::string trim(const std::string &s)
{
	auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) {return std::isspace(c); });
	auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) {return std::isspace(c); }).base();
	return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

using namespace Blueshift;
using namespace Core;
using namespace Utility;

using std::string;
using std::ifstream;
using std::stringstream;

ConfigFile::ConfigFile(std::string path) {
	ReadFile(path);
}

bool ConfigFile::ReadStream(std::istream& stream) {
	initialized = false;
	current_section.clear();
	options.clear();

	std::string buf;
	while (!stream.eof()) {
		std::getline(stream, buf);
		read_line(buf);
	}
	initialized = true;
	return true;
}

bool ConfigFile::ReadFile(std::string path) {
	ifstream stream(path.c_str());
	bool result = false;
	initialized = false;
	if (stream.is_open()) {
		result = ReadStream(stream);
	}
	stream.close();

	return result;
}

bool ConfigFile::ReadString(std::string data) {
	return ReadStream(stringstream(data));
}

void ConfigFile::read_line(std::string line) {
	std::string name;
	std::string value;
	int splitter_pos;

	if (line.length() == 0 ||
		line[0] == '#') {
		return;
	}

	if (line[0] == '[') {
		current_section = trim(line.substr(1, line.find(']') - 1));
		return;
	}

	splitter_pos = line.find('=');
	name = trim(line.substr(0, splitter_pos));
	value = trim(line.substr(splitter_pos + 1));

	options[current_section + '/' + name] = value;
}