#pragma once
#include "bgfx/bgfx.h"
#include "Core/Math/Vector.h"

namespace Blueshift {
	namespace Graphics {

		typedef bgfx::VertexDecl VertexType;

		struct BaseVertex {};

		struct SimpleVertex : public BaseVertex {
			struct {
				float X, Y, Z;
			} Position;
			struct {
				float X, Y, Z;
			} Normal;

			static void init() {
				Type.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
					.end();
			}
			static VertexType Type;
		};

		struct OBJVertex : public BaseVertex {
			struct {
				float X, Y, Z;
			} Position;
			struct {
				float X, Y;
			} TexCoord;
			struct {
				float X, Y, Z;
			} Normal;

			static void init() {
				Type.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
					.end();
			}
			static VertexType Type;
		};

		inline void InitializeVertexDeclarations() {
			SimpleVertex::init();
			OBJVertex::init();
		}
	}
}