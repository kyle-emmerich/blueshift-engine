#pragma once
#include "Core/Math/Math.h"
#include "Scripting/LuaState.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			void BindVector4(Scripting::LuaState* State);

			Vector4* CheckVector4(lua_State* L, int n);
			Vector4* PushVector4(lua_State* L);
		}
	}
}