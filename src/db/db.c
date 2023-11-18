#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "../lib/utils.h"
/*creates the db in the build dir if one doesn't exist*/
int init_db()
{
  int binDirExists = FALSE;
  sqlite3 *db;
  int rc = sqlite3_open("../build/db.sqlite", &db);
  if (rc != SQLITE_OK)
  {
    UTILS_ERROR_LOGGER("Cannot open database.", "init_db", "exit");
  }
  else
  {
  }
}
