#include <iostream>
#include <sqlite3.h>
#include "../lib/utils.h"

extern "C"
{
  int create_new_roster_table()
  {
    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    char *freeMessage;
    // Add your SQLite operations here, if needed
    // exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &message);

    if (rc != SQLITE_OK)
    {
      UTILS_ERROR_LOGGER("Error opening SQLite3 database:", "create_new_roster_table", "exit");
      sqlite3_free(freeMessage);
    }
    else
      std::cout << "Database opened Successfully" << std::endl;

    sqlite3_close(db);
    return 0;
  }
}
