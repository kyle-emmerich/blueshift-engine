#include "Storage/Database.h"
#include "Storage/File.h"

using namespace Blueshift;
using namespace Storage;

Database::Database(std::string filename, Database::OpenMode mode) {
	int flags = SQLITE_OPEN_FULLMUTEX;
	switch (mode) {
	case Database::OpenMode::ReadWriteCreate:
		flags |= SQLITE_OPEN_CREATE;
	case Database::OpenMode::ReadWrite:
		flags |= SQLITE_OPEN_READWRITE;
		break;
	case Database::OpenMode::ReadOnly:
		flags |= SQLITE_OPEN_READONLY;
		break;
	}
	if (SQLITE_OK != sqlite3_open_v2(filename.c_str(), &this->connection, flags, nullptr)) {
		const char* msg = sqlite3_errmsg(this->connection);
		sqlite3_close(this->connection);
		this->connection = nullptr;
		throw DatabaseConnectionError(Formatter() << "Error connecting to database: " << msg);
	}
}

Database::Database() {
	int flags = SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_MEMORY;
	if (SQLITE_OK != sqlite3_open_v2(":memory:", &this->connection, flags, nullptr)) {
		const char* msg = sqlite3_errmsg(this->connection);
		sqlite3_close(this->connection);
		this->connection = nullptr;
		throw DatabaseConnectionError(Formatter() << "Error creating in-memory database: " << msg);
	}
}

Database::~Database() {
	if (connection) {
		sqlite3_close_v2(connection);
	}
}

void Database::SaveTo(std::string filename) {
	sqlite3* file;
	sqlite3_backup* backup;

	if (SQLITE_OK == sqlite3_open(filename.c_str(), &file)) {
		backup = sqlite3_backup_init(file, "main", this->connection, "main");
		if (backup) {
			sqlite3_backup_step(backup, -1);
			sqlite3_backup_finish(backup);
		} else {
			throw DatabaseSaveError(Formatter() << "Error saving database to file: " << sqlite3_errmsg(file));
		}
	} else {
		throw DatabaseSaveError(Formatter() << "Error saving database to file: " << sqlite3_errmsg(file));
	}
}