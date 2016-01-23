#pragma once
#include "Math/Vector4.h"
#include "Scripting/LuaState.h"

namespace Blueshift {
	namespace Math {

		void BindVector4(Scripting::LuaState* State);

		Vector4* CheckVector4(lua_State* L, int n);
		Vector4* PushVector4(lua_State* L);
	}
}