#pragma once
#include "bgfx/bgfx.h"
#include "Core/Math/Vector.h"

struct PositionOnlyVertex {
	Vector3f Position;

	static void init() {
		decl.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.end();
	}
	static bgfx::VertexDecl decl;
};

struct PositionTexCoordVertex {
	Vector3f Position;
	Vector2f TexCoord;

	static void init() {
		decl.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();
	}
	static bgfx::VertexDecl decl;
};

struct PositionTexCoordNormalTangentVertex {
	Vector3f Position;
	Vector2f TexCoord;
	Vector3f Normal;
	Vector3f Tangent;

	static void init() {
		decl.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
			.end();
	}
	static bgfx::VertexDecl decl;
};

struct PositionTexCoordNormalTangentBitangentVertex {
	Vector3f Position;
	Vector2f TexCoord;
	Vector3f Normal;
	Vector3f Tangent;
	Vector3f Bitangent;

	static void init() {
		decl.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
			.end();
	}
	static bgfx::VertexDecl decl;
};

typedef PositionTexCoordNormalTangentBitangentVertex Vertex;

inline void InitializeVertexDeclarations() {
	PositionOnlyVertex::init();
	PositionTexCoordVertex::init();
	PositionTexCoordNormalTangentVertex::init();
	PositionTexCoordNormalTangentBitangentVertex::init();
}