#pragma once
#include "bgfx/bgfx.h"
#include "Core/Math/Vector.h"

namespace Blueshift {
	namespace Graphics {

		struct BaseVertex {};

		struct SimpleVertex : public BaseVertex {
			Blueshift::Core::Math::Vector3f Position;
			Blueshift::Core::Math::Vector3f Normal;

			SimpleVertex(Blueshift::Core::Math::Vector3f position, Blueshift::Core::Math::Vector3f normal)
				: Position(position), Normal(normal) {}

			static void init() {
				decl.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
					.end();
			}
			static bgfx::VertexDecl decl;
		};

		struct OBJVertex : public BaseVertex {
			Blueshift::Core::Math::Vector3f Position;
			Blueshift::Core::Math::Vector2f TexCoord;
			Blueshift::Core::Math::Vector3f Normal;

			static void init() {
				decl.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
					.end();
			}
			static bgfx::VertexDecl decl;
		};

		struct StaticVertex : public BaseVertex {
			Blueshift::Core::Math::Vector3f Position;
			Blueshift::Core::Math::Vector2f TexCoord;
			Blueshift::Core::Math::Vector3f Normal;
			Blueshift::Core::Math::Vector3f Tangent;
			Blueshift::Core::Math::Vector3f Bitangent;

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

		struct SkeletalVertex : public BaseVertex {
			Blueshift::Core::Math::Vector3f Position;
			Blueshift::Core::Math::Vector2f TexCoord;
			Blueshift::Core::Math::Vector3f Normal;
			Blueshift::Core::Math::Vector3f Tangent;
			Blueshift::Core::Math::Vector3f Bitangent;
			uint16_t Indices[8];
			double Weights[8];

			static void init() {
				decl.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Indices, 8, bgfx::AttribType::Int16)
					.add(bgfx::Attrib::Weight, 8, bgfx::AttribType::Float)
					.end();
			}
			static bgfx::VertexDecl decl;
		};

		inline void InitializeVertexDeclarations() {
			SimpleVertex::init();
			OBJVertex::init();
			StaticVertex::init();
			SkeletalVertex::init();
		}
	}
}