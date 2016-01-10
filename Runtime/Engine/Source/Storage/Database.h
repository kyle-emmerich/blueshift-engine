#pragma once
#include "Core/Core.h"
#include "Storage/sqlite3/sqlite3.h"

namespace Blueshift {
	namespace Storage {

		class DatabaseConnectionError : public RuntimeError {
		public:
			DatabaseConnectionError(std::string error_msg)
				: RuntimeError(error_msg) {}
		};

		class DatabaseSaveError : public RuntimeError {
		public:
			DatabaseSaveError(std::string error_msg)
				: RuntimeError(error_msg) {}
		};

		class FileSystem;
		class Statement;
		class Database {
		private:
			sqlite3* connection = nullptr;

			friend class Statement;
		public:
			enum class OpenMode {
				ReadOnly,
				ReadWrite,
				ReadWriteCreate
			};
			//Opens a database at filename, using the OpenMode to determine read/write access and whether
			//or not to create the database if it does not exist. Defaults to ReadWriteCreate.
			 Database(std::string filename, Database::OpenMode = Database::OpenMode::ReadWriteCreate);
			//Creates an in-memory database that is readable and writable.
			 Database();
			//Closes the database connection.
			~Database();

			//Saves the database to a file; note that this is not normally necessary, and actually just perform
			//a backup. Useful for saving memory databases.
			void SaveTo(std::string filename);

			//Creates a prepared statement using the provided SQL that can then have arguments bound to it and executed.
			//A prepared statement may be executed more than once!

		};


	}
}