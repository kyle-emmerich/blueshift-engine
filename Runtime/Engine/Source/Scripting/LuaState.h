#pragma once
#include <string>
#include <map>
#include <memory>
#include "lua.hpp"
#include "Core/Exceptions.h"

namespace Blueshift {
	namespace Scripting {

		class LuaState {
		protected:
			lua_State* lua_state;
			int err_func = 0;
		public:
			LuaState();
			~LuaState();

			void LoadFromFile(std::string filepath);
			void LoadFromString(std::string str, std::string source = "string");
			bool ExecuteChunk();
			bool ExecuteFile(std::string filepath);
			bool ExecuteString(std::string str, std::string source = "string");

			inline lua_State* GetState() { return lua_state; }

			static std::map<lua_State*, LuaState*> states;
		};

	}
}