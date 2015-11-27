#include "Graphics/Model/IQM/Loader.h"
#include "Graphics/VertexDeclarations.h"
#include "Storage/FileSystem.h"

using namespace Blueshift;
using namespace Graphics;
using namespace Model;
using namespace IQM;
using namespace Core::Math;

Loader::Loader(Storage::File* file)
	: ILoader(file), meshdata(nullptr) {	
	//TODO: convert to new asset system
	iqmheader header;
	if (file->Read(&header) != sizeof(iqmheader)) {
		//TODO: throw exception
		throw 0;
	}
	if (memcmp(header.magic, IQM_MAGIC, sizeof(header.magic)) != 0) {
		//TODO: throw exception
		throw 0;
	}

	uint8_t* buffer = new uint8_t[header.filesize - sizeof(iqmheader)];
	if (file->Read(buffer, header.filesize - sizeof(iqmheader)) != header.filesize - sizeof(iqmheader)) {
		//TODO: throw exception
		throw 0;
	}

	if (header.num_meshes > 0) {
		load_meshes(header, buffer);
	}
	if (header.num_anims > 0) {
		load_animations(header, buffer);
	}
}

Loader::~Loader() {
	if (meshdata != nullptr) {
		delete meshdata;
	}
}

void Loader::load_meshes(iqmheader& header, uint8_t* buffer) {
	//iterate through vertex arrays to find out what kind of data we're dealing with
	VertexType type; type.begin();
	iqmvertexarray* vas = reinterpret_cast<iqmvertexarray*>(&buffer[header.ofs_vertexarrays]);
	bgfx::Attrib::Enum* attribs = new bgfx::Attrib::Enum[header.num_vertexarrays];
	size_t* sizes = new size_t[header.num_vertexarrays];
	size_t texcoord_count = 0;
	size_t color_count = 0;
	for (size_t i = 0; i < header.num_vertexarrays; i++) {
		iqmvertexarray& va = vas[i];
		switch (va.type) {
		case IQM_POSITION:
			attribs[i] = bgfx::Attrib::Position;
			sizes[i] = sizeof(float);
			type.add(attribs[i], va.size, bgfx::AttribType::Float);
			break;
		case IQM_NORMAL:
			attribs[i] = bgfx::Attrib::Normal;
			sizes[i] = sizeof(float);
			type.add(attribs[i], va.size, bgfx::AttribType::Float);
			break;
		case IQM_TANGENT:
			attribs[i] = bgfx::Attrib::Tangent;
			sizes[i] = sizeof(float);
			type.add(attribs[i], va.size, bgfx::AttribType::Float);
			break;
		case IQM_TEXCOORD:
			attribs[i] = (bgfx::Attrib::Enum)(bgfx::Attrib::TexCoord0 + texcoord_count++);
			sizes[i] = sizeof(float);
			type.add(attribs[i], va.size, bgfx::AttribType::Float);
			break;
		case IQM_BLENDINDEXES:
			attribs[i] = bgfx::Attrib::Indices;
			sizes[i] = sizeof(uint8_t);
			type.add(attribs[i], va.size, bgfx::AttribType::Uint8);
			break;
		case IQM_BLENDWEIGHTS:
			attribs[i] = bgfx::Attrib::Weight;
			sizes[i] = sizeof(uint8_t);
			type.add(attribs[i], va.size, bgfx::AttribType::Uint8, true);
			break;
		case IQM_COLOR:
			attribs[i] = (bgfx::Attrib::Enum)(bgfx::Attrib::Color0 + color_count++);
			sizes[i] = sizeof(uint8_t);
			type.add(attribs[i], va.size, bgfx::AttribType::Uint8, true);
			break;
		}
	}
	type.end();

	RenderData vertices = AllocateRenderData(type.getSize(header.num_vertexes));
	for (size_t i = 0; i < header.num_vertexes; i++) {
		//for each type of vertexarray we got...
		for (size_t j = 0; j < header.num_vertexarrays; j++) {
			iqmvertexarray& va = vas[j];
			//put it in the vertices buffer
			size_t dest = (i * type.getStride()) + type.getOffset(attribs[j]);
			memcpy(
				&vertices->data[dest],
				&buffer[va.offset + (i * type.getStride()) + type.getOffset(attribs[j])],
				va.size * sizes[i]
			);
		}
	}

	//for each triangle, load the index
	RenderData indices = AllocateRenderData(header.num_triangles * 3 * sizeof(uint32_t));
	iqmtriangle* triangles = reinterpret_cast<iqmtriangle*>(&buffer[header.ofs_triangles]);
	for (size_t i = 0; i < header.num_triangles; i++) {
		//this is pretty simple and straightforward
		indices->data[(i * 3) + 0] = triangles[i].vertex[0];
		indices->data[(i * 3) + 1] = triangles[i].vertex[1];
		indices->data[(i * 3) + 2] = triangles[i].vertex[2];
	}

	//now we can form our staticmeshdata
	 meshdata = new StaticMeshData<uint8_t, uint32_t>(vertices, indices, type);
	
	//each mesh will get a section
	iqmmesh* meshes = reinterpret_cast<iqmmesh*>(&buffer[header.ofs_meshes]);
	for (uint32_t i = 0; i < header.num_meshes; i++) {
		iqmmesh& mesh = meshes[i];
		Vector3f min_pos(0.0f, 0.0f, 0.0f);
		Vector3f max_pos(0.0f, 0.0f, 0.0f);

		for (uint32_t i = mesh.first_vertex; i < mesh.first_vertex + mesh.num_vertexes; i++) {
			Vector3f& pos = reinterpret_cast<Vector3f*>(vertices->data[i * type.getStride()])[0];
			
			min_pos.X = min(pos.X, min_pos.X);
			min_pos.X = min(pos.Y, min_pos.Y);
			min_pos.Z = min(pos.Z, min_pos.Z);

			max_pos.X = max(pos.X, max_pos.X);
			max_pos.X = max(pos.Y, max_pos.Y);
			max_pos.Z = max(pos.Z, max_pos.Z);
		}

		AABB<float> aabb;
		aabb.HalfWidth = (max_pos - min_pos) / 2.0f;
		aabb.Center = min_pos + aabb.HalfWidth;
		float radius = aabb.HalfWidth.Length();
		meshdata->AddSection(mesh.first_vertex, mesh.num_vertexes, mesh.first_triangle, mesh.num_triangles * 3, {
			aabb,
			radius
		});
	}

	iqmjoint* joints = reinterpret_cast<iqmjoint*>(&buffer[header.ofs_joints]);
	for (uint32_t i = 0; i < header.num_joints; i++) {
		Quaternionf* rotation = reinterpret_cast<Quaternionf*>(joints[i].rotate);
		Vector3f* position = reinterpret_cast<Vector3f*>(joints[i].translate);
		Vector3f* scale = reinterpret_cast<Vector3f*>(joints[i].scale);

		Matrix4f frame = ScaleMatrix(*scale) * QuaternionToMatrix4(*rotation) * TranslationMatrix(*position);
		Matrix4f inverse_frame = Inverse(frame);
		MeshBone* parent = nullptr;
		if (joints[i].parent >= 0) {
			parent = &meshdata->GetBone(static_cast<size_t>(joints[i].parent));
			frame = parent->frame * frame;
			inverse_frame = inverse_frame * parent->inverse_frame;
		}
		meshdata->AddBone(parent, frame, inverse_frame);
	}

	//TODO: load materials for each mesh accordingly and assign somehow? more API changes.
	//i guess that api was pretty lacking
}

void Loader::load_animations(iqmheader& header, uint8_t* buffer) {
	//TODO: load animations from iqm file; needs some API updates before this can happen reasonably.
}