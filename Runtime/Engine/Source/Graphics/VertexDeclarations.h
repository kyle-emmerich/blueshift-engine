#pragma once
#include "bgfx/bgfx.h"
#include "Core/Math/Vector.h"

namespace Blueshift {
	namespace Graphics {

		struct PositionOnlyVertex {
			Blueshift::Core::Math::Vector3f Position;

			static void init() {
				decl.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.end();
			}
			static bgfx::VertexDecl decl;
		};

		struct PositionTexCoordVertex {
			Blueshift::Core::Math::Vector3f Position;
			Blueshift::Core::Math::Vector2f TexCoord;

			static void init() {
				decl.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
					.end();
			}
			static bgfx::VertexDecl decl;
		};

		struct PositionTexCoordNormalTangentVertex {
			Blueshift::Core::Math::Vector3f Position;
			Blueshift::Core::Math::Vector2f TexCoord;
			Blueshift::Core::Math::Vector3f Normal;
			Blueshift::Core::Math::Vector3f Tangent;

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

		typedef PositionTexCoordNormalTangentBitangentVertex Vertex;

		inline void InitializeVertexDeclarations() {
			PositionOnlyVertex::init();
			PositionTexCoordVertex::init();
			PositionTexCoordNormalTangentVertex::init();
			PositionTexCoordNormalTangentBitangentVertex::init();
		}

	}
}