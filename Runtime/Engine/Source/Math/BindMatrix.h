#pragma once
#include "Math/Matrix4.h"
#include "Math/Vector4.h"
#include "Scripting/LuaState.h"

namespace Blueshift {
	namespace Math {

		void BindMatrix4(Scripting::LuaState* State);

		Matrix4* CheckMatrix4(lua_State* L, int n);
		void* CheckMatrix4OrVector4(lua_State* L, int n, bool& is_matrix);
		Matrix4* PushMatrix4(lua_State* L);

	}
}