#ifndef SQLITE_H
#define SQLITE_H

#include <string>
#include <sqlite3.h>
#include <stdio.h>

class SQLite3 {
public:
  SQLite3(std::string dbName);
  void execute(std::string sql);
  ~SQLite3();
private:
  int rc;
  sqlite3 *db;
  char *err_msg;
};

#endif
