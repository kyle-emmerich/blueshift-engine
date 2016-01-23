#include "Math/BindMatrix.h"
#include "Math/BindVector.h"

using namespace Blueshift;
using namespace Math;

Math::Matrix4* Blueshift::Math::CheckMatrix4(lua_State* L, int n) {
	const void* ud = lua_topointer(L, n);
	luaL_argcheck(L, ud != nullptr, n, "'Matrix4' expected");
	return (Matrix4*)ud;
}

void* Blueshift::Math::CheckMatrix4OrVector4(lua_State* L, int n, bool& is_matrix) {
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

Matrix4* Blueshift::Math::PushMatrix4(lua_State* L) {
	Matrix4* m = new Matrix4;
	lua_pushlightuserdata(L, m);
	return m;
}

static int __new(lua_State* L) {
	PushMatrix4(L);
	return 1;
}
static int __delete(lua_State* L) {
	const Matrix4* m = reinterpret_cast<const Matrix4*>(lua_topointer(L, 1));
	delete m;
	return 0;
}

static int __get(lua_State* L) {
	Matrix4* mat = CheckMatrix4(L, 1);
	int i = luaL_checkint(L, 2);
	
	Vector4* vec = &(mat->vec[i - 1]);
	lua_pushlightuserdata(L, vec);

	return 1;
}
static int __set(lua_State* L) {
	Matrix4* mat = CheckMatrix4(L, 1);
	int i = luaL_checkint(L, 2);
	Vector4* vec = CheckVector4(L, 3);

	mat->vec[i - 1] = *vec;

	return 0;
}

static int Matrix4_Transpose(lua_State* L) {
	Matrix4* a = CheckMatrix4(L, 1);
	Transpose(*a);
	return 0;
}

static int Matrix4_Inverse(lua_State* L) {
	Matrix4* a = CheckMatrix4(L, 1);
	Matrix4* b = PushMatrix4(L);
	*b = Invert(*a);

	return 1;
}

static int Matrix4_Invert(lua_State* L) {
	Matrix4* a = CheckMatrix4(L, 1);
	Invert(*a);

	return 0;
}

static int Matrix4_FromAxisAngle(lua_State* L) {
	Vector4* axis = CheckVector4(L, 1);
	float angle = (float)luaL_checknumber(L, 2);

	Matrix4* m = PushMatrix4(L);
	*m = RotationMatrix(*axis, angle);

	return 1;
}

void Blueshift::Math::BindMatrix4(Scripting::LuaState* State) {
	lua_State* L = State->GetState();

	luaL_Reg Matrix4_meta[] = {
		{ "__new__", __new },
		{ "__delete__", __delete },
		{ "__get__", __get },
		{ "__set__", __set },

		{ "__transpose__", Matrix4_Transpose },
		{ "__inverse__", Matrix4_Inverse },
		{ "__invert__", Matrix4_Invert },
		{ "__axisangle__", Matrix4_FromAxisAngle },

		{ nullptr, nullptr }
	};

	luaL_openlib(L, "Matrix4_internals", Matrix4_meta, 0);
	State->ExecuteFile("Blueshift/Matrix4.lua");
}