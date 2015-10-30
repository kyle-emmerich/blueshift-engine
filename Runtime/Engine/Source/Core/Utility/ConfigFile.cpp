#include "Storage/File.h"
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
	Storage::File file(path);
	size_t length = file.GetLength();
	char* data = new char[length];
	file.Read(data, length);

	std::stringstream ss;
	ss << data;
	std::string line;
	while (!std::getline(ss, line).eof()) {
		read_line(line);
	}

	ss.clear();
	delete[] data;
}

void ConfigFile::read_line(std::string line) {
	std::string name;
	std::string value;
	size_t splitter_pos;

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