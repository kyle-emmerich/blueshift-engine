#include "Scripting/LuaState.h"
#include "Core/Engine.h"
#include "Storage/FileSystem.h"
#include "Core/Math/Math.h"
#include "Core/Math/Bindings.h"

extern "C" {
	#include "luajit.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

using namespace Blueshift;
using namespace Scripting;

std::map<lua_State*, LuaState*> LuaState::states;

static int traceback(lua_State* L);
static void* allocator(void* ud, void* ptr, size_t osize, size_t nsize);

LuaState::LuaState() {
	lua_state = luaL_newstate();
	states[lua_state] = this;
	luaL_openlibs(lua_state);

	lua_getfield(lua_state, LUA_GLOBALSINDEX, "package");
	lua_getfield(lua_state, -1, "loaders");
	lua_remove(lua_state, -2);
	lua_pushcfunction(lua_state, [](lua_State* L) -> int {
		Storage::FileSystem* fs = Core::Engine::Get().GetSystem<Storage::FileSystem>();

		std::string filename;
		std::string module;
		const char* name = luaL_checkstring(L, 1);
		module = luaL_gsub(L, name, ".", "/");

		if (fs->IsDirectory(module)) {
			filename = module + "/init.lua";
		} else {
			filename = module + ".lua";
		}

		if (!fs->FileExists(filename)) {
			//not found here!
			lua_pushfstring(L, "\n\tno file %s in Scripts", filename.c_str());
			return 1;
		}
		
		try {
			LuaState* state = LuaState::states[L];
			state->LoadFromFile(filename);
			
		} catch (...) {}
		return 1;
	});
	lua_rawseti(lua_state, -2, 2);

	lua_register(lua_state, "print", [](lua_State* L) -> int {
		int n = lua_gettop(L);
		
		lua_getglobal(L, "tostring");
		Formatter formatter;
		for (int i = 1; i <= n; i++) {
			const char* s;
			lua_pushvalue(L, -1);
			lua_pushvalue(L, i);
			lua_call(L, 1, 1);
			s = lua_tostring(L, -1);
			if (s == nullptr) {
				return luaL_error(L, "tostring must return a string to print");
			}
			if (i > 1) {
				formatter << "\t";
			}
			formatter << s;
			lua_pop(L, 1);
		}
		Core::Engine::Get().Log(Core::LogLevel::Lua, formatter);
		return 0;
	});
	lua_pushcfunction(lua_state, traceback);
	err_func = lua_gettop(lua_state);

	this->ExecuteFile("Blueshift/Setup.lua");
	Core::Math::BindVector4(this);
	Core::Math::BindMatrix4(this);
	Core::Math::BindQuaternion(this);
}

LuaState::~LuaState() {
	lua_close(lua_state);
}

void LuaState::LoadFromFile(std::string filepath) {
	Storage::File* file = new Storage::File(filepath);
	std::string str;
	file->Read(str, file->GetLength());
	delete file;
	LoadFromString(str, filepath);
}

void LuaState::LoadFromString(std::string str, std::string source) {
	int result = luaL_loadbuffer(lua_state, str.c_str(), str.length(), source.c_str());
	if (result != 0) {
		Formatter formatter;
		if (result == LUA_ERRSYNTAX) {
			if (lua_isstring(lua_state, -1)) {
				formatter << lua_tostring(lua_state, -1);
			} else {
				formatter << "unknown syntax error";
			}
		} else if (result == LUA_ERRMEM) {
			formatter << "out of memory";
		} else {
			formatter << "unknown error";
		}

		lua_gc(lua_state, LUA_GCCOLLECT, 0);

		throw LuaError(formatter);
	}
}

bool LuaState::ExecuteChunk() {
	
	int result = lua_pcall(lua_state, 0, 0, err_func);
	if (result != 0) {
		lua_gc(lua_state, LUA_GCCOLLECT, 0);
		return false;
	}
	return true;
}

bool LuaState::ExecuteFile(std::string filepath) {
	LoadFromFile(filepath);
	return ExecuteChunk();
}

bool LuaState::ExecuteString(std::string str, std::string source) {
	LoadFromString(str, source);
	return ExecuteChunk();
}

static int traceback(lua_State* L) {
	if (!lua_isstring(L, 1)) {
		return 1;
	}
	lua_getfield(L, LUA_GLOBALSINDEX, "debug");
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return 1;
	}
	lua_getfield(L, -1, "traceback");
	if (!lua_isfunction(L, -1)) {
		lua_pop(L, 2);
		return 1;
	}
	lua_pushvalue(L, 1);
	lua_pushinteger(L, 2);
	lua_call(L, 2, 1);

	//now our stack should have the string
	const char* trace = lua_tostring(L, 1);
	Core::Engine::Get().Log(Core::LogLevel::LuaError, Formatter() << trace);
	lua_pop(L, 1);
	return 1;
}

static void* allocator(void* ud, void* ptr, size_t osize, size_t nsize) {
	if (nsize == 0) {
//#ifdef _WIN32
		_aligned_free(ptr);
//#endif
		//TODO: support other platforms?
		return nullptr;
	} else {
//#ifdef _WIN32
		if (osize == LUA_TUSERDATA) {
			return _aligned_realloc(ptr, nsize, 16);
		} else {
			return realloc(ptr, nsize);
		}
//#endif
	}
}