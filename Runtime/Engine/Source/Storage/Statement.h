#pragma once
#include "Core/Core.h"
#include "sqlite3/sqlite3.h"
#include <unordered_map>

namespace Blueshift {
	namespace Storage {

		class Database;
		class Statement {
		private:
			sqlite3_stmt* statement;
			std::vector<std::string> col_names;
			std::unordered_map<std::string, int> col_idx;

			bool has_row = false;
		public:
			 Statement(Database& db, const std::string& sql);
			~Statement();

			void BindInt(const std::string& identifier, int value);
			void BindInt(int index, int value);

			void BindInt64(const std::string& identifier, int64_t value);
			void BindInt64(int index, int64_t value);

			void BindString(const std::string& identifier, const std::string& value);
			void BindString(int index, const std::string& value);

			void BindNull(const std::string& identifier);
			void BindNull(int index);

			void BindBinary(const std::string& identifier, const void* data, size_t bytes);
			void BindBinary(int index, const void* data, size_t bytes);

			void Step();

			int GetColumnIndex(const std::string& name);
			const std::string& GetColumnName(int column);

			inline bool HasRow() {
				return this->has_row;
			}
			
			int ReadInt(int col);
			int ReadInt(const std::string& name);
			int64_t ReadInt64(int col);
			int64_t ReadInt64(const std::string& name);
			std::string ReadString(int col);
			std::string ReadString(const std::string& name);
			const void* ReadBinary(int col, size_t& out_bytes);
			const void* ReadBinary(const std::string& name, size_t& out_bytes);
		};

	}
}