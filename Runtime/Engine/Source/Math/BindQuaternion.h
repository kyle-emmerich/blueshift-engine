#pragma once
#include "Core/Math/Math.h"
#include "Scripting/LuaState.h"

namespace Blueshift {
	namespace Core {
		namespace Math {
			Quaternion* CheckQuaternion(lua_State* L, int n);
			Quaternion* PushQuaternion(lua_State* L);

			void BindQuaternion(Scripting::LuaState* State);

		}
	}
}