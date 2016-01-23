#include "Core/Math/BindQuaternion.h"

using namespace Blueshift;
using namespace Core;
using namespace Math;

Quaternion* Blueshift::Core::Math::CheckQuaternion(lua_State* L, int n) {
	const void* ud = lua_topointer(L, n);
	luaL_argcheck(L, ud != nullptr, n, "'Quaternion' expected");
	return (Quaternion*)ud;
}

Quaternion* Blueshift::Core::Math::PushQuaternion(lua_State* L) {
	Quaternion* q = new Quaternion;
	lua_pushlightuserdata(L, q);
	return q;
}
static int __new(lua_State* L) {
	PushQuaternion(L);
	return 1;
}
static int __delete(lua_State* L) {
	const Quaternion* q = reinterpret_cast<const Quaternion*>(lua_topointer(L, 1));
	delete q;
	return 0;
}

static int __get(lua_State* L) {
	Quaternion* quat = CheckQuaternion(L, 1);
	int idx = luaL_checkint(L, 2);

	lua_pushnumber(L, quat->data[idx - 1]);

	return 1;
}
static int __set(lua_State* L) {
	Quaternion* quat = CheckQuaternion(L, 1);
	int idx = luaL_checkint(L, 2);
	float num = static_cast<float>(luaL_checknumber(L, 3));

	quat->data[idx - 1] = num;

	return 0;
}

void Blueshift::Core::Math::BindQuaternion(Scripting::LuaState* State) {
	lua_State* L = State->GetState();

	luaL_Reg Quaternion_meta[] = {
		{ "__new__", __new },
		{ "__delete__", __delete },
		{ "__get__", __get },
		{ "__set__", __set },
		

		{ nullptr, nullptr }
	};

	luaL_openlib(L, "Quaternion_internals", Quaternion_meta, 0);
	State->ExecuteFile("Blueshift/Quaternion.lua");
}