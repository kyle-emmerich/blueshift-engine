#include "Storage/Statement.h"
#include "Storage/Database.h"

using namespace Blueshift;
using namespace Storage;

Statement::Statement(Database& db, const std::string& sql) {
	sqlite3_prepare(db.connection, sql.c_str(), (int)sql.length() + 1, &this->statement, nullptr);
	
	int num_cols = sqlite3_column_count(this->statement);
	this->col_names.resize(num_cols);
	for (int i = 0; i < this->col_names.capacity(); i++) {
		this->col_names[i] = std::string(sqlite3_column_name(this->statement, i));
		this->col_idx[this->col_names[i]] = i;
	}
}

Statement::~Statement() {
	sqlite3_finalize(this->statement);
}

void Statement::BindInt(const std::string& identifier, int value) {
	this->BindInt(sqlite3_bind_parameter_index(this->statement, identifier.c_str()), value);
}
void Statement::BindInt(int index, int value) {
	sqlite3_bind_int(this->statement, index, value);
}

void Statement::BindInt64(const std::string& identifier, int64_t value) {
	this->BindInt64(sqlite3_bind_parameter_index(this->statement, identifier.c_str()), value);
}
void Statement::BindInt64(int index, int64_t value) {
	sqlite3_bind_int64(this->statement, index, value);
}

void Statement::BindString(const std::string& identifier, const std::string& value) {
	this->BindString(sqlite3_bind_parameter_index(this->statement, identifier.c_str()), value);
}
void Statement::BindString(int index, const std::string& value) {
	sqlite3_bind_text(this->statement, index, value.c_str(), value.length(), SQLITE_TRANSIENT);
}

void Statement::BindNull(const std::string& identifier) {
	sqlite3_bind_null(this->statement, sqlite3_bind_parameter_index(this->statement, identifier.c_str()));
}
void Statement::BindNull(int index) {
	sqlite3_bind_null(this->statement, index);
}

void Statement::BindBinary(const std::string& identifier, const void* data, size_t bytes) {
	this->BindBinary(sqlite3_bind_parameter_index(this->statement, identifier.c_str()), data, bytes);
}
void Statement::BindBinary(int index, const void* data, size_t bytes) {
	if (data == nullptr) {
		sqlite3_bind_zeroblob64(this->statement, index, bytes);
	} else {
		sqlite3_bind_blob64(this->statement, index, data, bytes, SQLITE_TRANSIENT);
	}
}

void Statement::Step() {
	int code = sqlite3_step(this->statement);
	has_row = false;
	if (code == SQLITE_ROW) {
		has_row = true;
	}
}

int Statement::GetColumnIndex(const std::string& name) {
	auto it = this->col_idx.find(name);
	if (it == this->col_idx.end()) {
		return -1;
	}
	return it->second;
}
const std::string& Statement::GetColumnName(int column) {
	static const std::string& invalid = "invalid";
	if (column < 0 || column > this->col_names.size()) {
		return invalid;
	}

	return this->col_names[column];
}


int Statement::ReadInt(int col) {
	return sqlite3_column_int(this->statement, col);
}
int Statement::ReadInt(const std::string& name) {
	return sqlite3_column_int(this->statement, this->GetColumnIndex(name));
}

int64_t Statement::ReadInt64(int col) {
	return sqlite3_column_int64(this->statement, col);
}
int64_t Statement::ReadInt64(const std::string& name) {
	return sqlite3_column_int64(this->statement, this->GetColumnIndex(name));
}

std::string Statement::ReadString(int col) {
	const unsigned char* str = sqlite3_column_text(this->statement, col);
	return std::string((const char*)str);
}
std::string Statement::ReadString(const std::string& name) {
	return ReadString(this->GetColumnIndex(name));
}

const void* Statement::ReadBinary(int col, size_t& out_bytes) {
	const void* ptr = sqlite3_column_blob(this->statement, col);
	out_bytes = sqlite3_column_bytes(this->statement, col);
	return ptr;
}
const void* Statement::ReadBinary(const std::string& name, size_t& out_bytes) {
	return this->ReadBinary(this->GetColumnIndex(name), out_bytes);
}