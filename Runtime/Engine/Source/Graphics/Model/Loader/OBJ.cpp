#include "Graphics/Model/Loader/OBJ.h"
#include "Core/Math/Vector.h"
#include "Core/Utility/FastParsing.h"
#include "Storage/File.h"
#include <sstream>

using namespace Blueshift;
using namespace Graphics;
using namespace Model;
using namespace Loader;
using namespace Core::Math;

OBJ::OBJ()
	: current(nullptr) {}
OBJ::~OBJ() {}

bool OBJ::Load(std::string path, std::vector<std::unique_ptr<Model::OBJMeshData>>& meshes) {
	Storage::File file(path);
	size_t length = file.GetLength();
	char* data = new char[length];
	file.Read(data, length);

	std::stringstream ss;
	ss << data;
	std::string line;

	current = new o;

	while (!std::getline(ss, line).eof()) {
		try {
			process_line(line);
		} catch (std::exception&) {
			return false;
		}
	}
	to_mesh_data(meshes);

	ss.clear();
	delete[] data;
	return true;
}

void OBJ::process_line(std::string line) {
	if (line.length() < 2) {
		return;
	}

	switch (line[0]) {
	case 'o':
	{
		//an object
		if (current != nullptr && current->verts.size() > 0) {
			objects.push_back(std::unique_ptr<o>(current));
			current->complete = true;
		}
		current = current->name.length() > 0 ? new o : current;
		current->name = line.substr(2);
		if (current->name.length() == 0) {
			current->name = "unnamed";
		}
		return;
	}
	case 'v':
	{
		//vertex-related data, switch on second char to find out what it is.
		//if there is no second char, it's a position
		if (line[1] == ' ') {
			process_v(line, 2);
		} else {
			switch (line[1]) {
			case 't':
			{
				process_vt(line, 3);
				return;
			}
			case 'n':
			{
				process_vn(line, 3);
				return;
			}
			default:
				//unsupported, potentially throw warning for debug purposes?
				return;
			}
		}
		return;
	}
	case 'f':
	{
		//face data comes as 3 vertices, but each component is indexed separately
		//in the form of p/t/n
		//so we can use getline on each token, and emit a vertex and index for each token.
		process_f(line, 2);
		break;
	}
	default:
		return;
	}
}

void OBJ::process_v(std::string line, size_t offset) {
	char* buf = const_cast<char*>(line.c_str()) + offset;
	char** pos = &buf;
	float x, y, z;
	x = (float)strtod(*pos, pos);
	y = (float)strtod(*pos, pos);
	z = (float)strtod(*pos, nullptr);
	current->p.push_back(Vector3f(x, y, z));
}

void OBJ::process_vt(std::string line, size_t offset) {
	char* buf = const_cast<char*>(line.c_str()) + offset;
	char** pos = &buf;
	float x, y;
	x = (float)strtod(*pos, pos);
	y = (float)strtod(*pos, nullptr);
	current->t.push_back(Vector2f(x, y));
}

void OBJ::process_vn(std::string line, size_t offset) {
	char* buf = const_cast<char*>(line.c_str()) + offset;
	char** pos = &buf;
	float x, y, z;
	x = (float)strtod(*pos, pos);
	y = (float)strtod(*pos, pos);
	z = (float)strtod(*pos, nullptr);
	current->n.push_back(Vector3f(x, y, z));
}

void OBJ::process_f(std::string line, size_t offset) {
	//process each triplet
	process_triplet(line, offset);
	for (int i = 1; i < 3; i++) {
		size_t start = line.find_first_of(' ', offset);
		offset = start + 1;
		process_triplet(line, offset);
	}
}

void OBJ::process_triplet(std::string line, size_t offset) {
	size_t slash = offset;
	OBJIndex idx[3];
	for (int i = 0; i < 3; i++) {
		size_t next = line.find_first_of('/', slash);
		if (next == std::string::npos) {
			next = line.size();
		}
		if (next != slash) {
			idx[i] = Core::Utility::ParseUnsignedInt(line.c_str(), slash, next - slash, next);
		} else {
			idx[i] = 0;
		}
		slash = next + 1;
	}

	OBJVertex vert;
	vert.Position = idx[0] == 0 ? Vector3f(0.0f) : current->p[idx[0] - 1];
	vert.TexCoord = idx[1] == 0 ? Vector2f(0.0f) : current->t[idx[1] - 1];
	vert.Normal = idx[2] == 0 ? Vector3f(0.0f) : current->n[idx[2] - 1];

	OBJIndex vert_idx = static_cast<OBJIndex>(current->verts.size());
	current->verts.push_back(vert);
	current->indices.push_back(vert_idx);
}

void OBJ::to_mesh_data(std::vector<std::unique_ptr<Model::OBJMeshData>>& meshes) {
	if (current != nullptr && current->verts.size() > 0) {
		current->complete = true;
		objects.push_back(std::unique_ptr<o>(current));
		current = nullptr;
	}
	for (std::unique_ptr<o>& obj : objects) {
		if (obj->complete) {
			MeshBounds bounds = {
				AABB<float>(),
				OBB<float>(),
				Sphere<float>()
			};
			meshes.push_back(std::unique_ptr<Model::OBJMeshData>(new OBJMeshData(obj->name, &obj->verts[0], obj->verts.size(), OBJVertex::Type, &obj->indices[0], obj->indices.size(), bounds)));
		}
	}
	objects.swap(std::vector<std::unique_ptr<o>>());
}