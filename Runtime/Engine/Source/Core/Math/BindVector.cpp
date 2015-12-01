#include "Core/Math/BindVector.h"

using namespace Blueshift;
using namespace Core;
using namespace Math;

static Vector4* CheckVector4(lua_State* L, int n) {
	const void* ud = lua_topointer(L, n);
	luaL_argcheck(L, ud != NULL, n, "'Vector4' expected");
	return (Vector4*)ud;
}
static void* CheckVector4OrMatrix4(lua_State* L, int n, bool& is_matrix) {
	void* ud = lua_touserdata(L, n);
	if (lua_getmetatable(L, n)) {
		lua_getfield(L, LUA_REGISTRYINDEX, "Vector4");
		lua_getfield(L, LUA_REGISTRYINDEX, "Matrix4");
		if (lua_rawequal(L, -2, -3)) {
			is_matrix = false;
			lua_pop(L, 3);
			return ud;
		} else if (lua_rawequal(L, -1, -3)) {
			is_matrix = true;
			lua_pop(L, 1);
			return ud;
		}
	} 
	luaL_argcheck(L, ud != NULL, n, "'Vector4' or 'Matrix4' expected");
	return nullptr;
}
static Vector4* PushVector4(lua_State* L, float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) {
	Vector4* rv = reinterpret_cast<Vector4*>(lua_newuserdata(L, sizeof(Vector4)));
	luaL_getmetatable(L, "Vector4");
	lua_setmetatable(L, -2);

	rv->X = static_cast<float>(x);
	rv->Y = static_cast<float>(y);
	rv->Z = static_cast<float>(z);
	rv->W = static_cast<float>(w);

	return rv;
}

static int __new(lua_State* L) {
	Vector4* v = new Vector4;
	lua_pushlightuserdata(L, v);
	return 1;
}
static int __delete(lua_State* L) {
	const Vector4* v = reinterpret_cast<const Vector4*>(lua_topointer(L, 1));
	delete v;
	return 0;
}


static int __get(lua_State* L) {
	Vector4* vec = CheckVector4(L, 1);
	int idx = luaL_checkint(L, 2);

	lua_pushnumber(L, vec->data[idx]);

	return 1;
}
static int __set(lua_State* L) {
	Vector4* vec = CheckVector4(L, 1);
	int idx = luaL_checkint(L, 2);
	float num = static_cast<float>(luaL_checknumber(L, 3));

	vec->data[idx] = num;

	return 0;
}

static int __add(lua_State* L) {
	Vector4* a = CheckVector4(L, 1);
	if (lua_isuserdata(L, 2)) {
		Vector4* b = CheckVector4(L, 2);
		Vector4* rv = PushVector4(L);
		AddInplace(*a, *b, *rv);
		return 1;
	} else if (lua_isnumber(L, 2)) {
		float s = static_cast<float>(luaL_checknumber(L, 2));
		Vector4* rv = PushVector4(L);
		AddInplace(*a, s, *rv);
		return 1;
	}
	return 1;
}

static int __sub(lua_State* L) {
	Vector4* a = CheckVector4(L, 1);
	if (lua_isuserdata(L, 2)) {
		Vector4* b = CheckVector4(L, 2);
		Vector4* rv = PushVector4(L);
		SubInplace(*a, *b, *rv);
		return 1;
	} else if (lua_isnumber(L, 2)) {
		float s = static_cast<float>(luaL_checknumber(L, 2));
		Vector4* rv = PushVector4(L);
		SubInplace(*a, s, *rv);
		return 1;
	}
	return 1;
}

static int __mul(lua_State* L) {
	Vector4* a = CheckVector4(L, 1);
	if (lua_isuserdata(L, 2)) {
		Vector4* b = CheckVector4(L, 2);
		Vector4* rv = PushVector4(L);
		MulInplace(*a, *b, *rv);
		return 1;
	} else if (lua_isnumber(L, 2)) {
		float s = static_cast<float>(luaL_checknumber(L, 2));
		Vector4* rv = PushVector4(L);
		MulInplace(*a, s, *rv);
		return 1;
	}
	return 1;
}

static int __div(lua_State* L) {
	Vector4* a = CheckVector4(L, 1);
	if (lua_isuserdata(L, 2)) {
		Vector4* b = CheckVector4(L, 2);
		Vector4* rv = PushVector4(L);
		DivInplace(*a, *b, *rv);
		return 1;
	} else if (lua_isnumber(L, 2)) {
		float s = static_cast<float>(luaL_checknumber(L, 2));
		Vector4* rv = PushVector4(L);
		DivInplace(*a, s, *rv);
		return 1;
	}
	return 1;
}

static int Vector4_Dot(lua_State* L) {
	Vector4* a = CheckVector4(L, 1);
	Vector4* b = CheckVector4(L, 2);
	bool with_w = lua_toboolean(L, 3) != 0;

	float res = DotProduct(*a, *b, with_w);
	lua_pushnumber(L, res);

	return 1;
}

static int Vector4_Cross(lua_State* L) {
	Vector4* a = CheckVector4(L, 1);
	Vector4* b = CheckVector4(L, 2);
	Vector4* rv = PushVector4(L);

	*rv = CrossProduct(*a, *b);

	return 1;
}

static int Vector4_Lerp(lua_State* L) {
	Vector4* a = CheckVector4(L, 1);
	Vector4* b = CheckVector4(L, 2);
	float m = static_cast<float>(luaL_checknumber(L, 3));
	Vector4* rv = PushVector4(L);

	LerpInplace(*a, *b, m, *rv);

	return 1;
}

static int Vector4_Length(lua_State* L) {
	Vector4* a = CheckVector4(L, 1);
	bool with_w = lua_toboolean(L, 2) != 0;
	
	lua_pushnumber(L, Length(*a, with_w));

	return 1;
}

static int Vector4_LengthSq(lua_State* L) {
	Vector4* a = CheckVector4(L, 1);
	bool with_w = lua_toboolean(L, 2) != 0;

	lua_pushnumber(L, Length(*a, with_w));

	return 1;
}

void Blueshift::Core::Math::BindVector4(Scripting::LuaState* State) {
	lua_State* L = State->GetState();

	luaL_Reg Vector4_meta[] = {
		{ "__new__", __new },
		{ "__delete__", __delete },
		{ "__get__", __get },
		{ "__set__", __set },

		{ "__add__", __add },
		{ "__sub__", __sub },
		{ "__mul__", __mul },
		{ "__div__", __div },

		{ "__dot__", Vector4_Dot },
		{ "__cross__", Vector4_Cross },
		{ "__lerp__", Vector4_Lerp },

		{ "__length__", Vector4_Length },
		{ "__length_sq__", Vector4_LengthSq },

		{ nullptr, nullptr }
	};

	luaL_openlib(L, "Vector4_internals", Vector4_meta, 0);
	State->ExecuteFile("Blueshift/Vector4.lua");
}