#include "sqlite3.h"

SQLite3::SQLite3(std::string dbName): err_msg{0} {
  rc = sqlite3_open(dbName.c_str(), &db);
  if (rc != SQLITE_OK) {
      throw "Cannot open database.";
  }

}

SQLite3::~SQLite3() {
  sqlite3_close(db);
}

void SQLite3::execute(std::string sql) {
  rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);

  if (rc != SQLITE_OK ) {

      throw "SQL error.\n";

      sqlite3_free(err_msg);
  }
}
