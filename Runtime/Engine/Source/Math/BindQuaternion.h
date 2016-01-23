#pragma once
#include "Math/Quaternion.h"
#include "Scripting/LuaState.h"

namespace Blueshift {
	namespace Math {
		Quaternion* CheckQuaternion(lua_State* L, int n);
		Quaternion* PushQuaternion(lua_State* L);

		void BindQuaternion(Scripting::LuaState* State);

	}
}