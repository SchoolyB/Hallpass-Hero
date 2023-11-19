#include <iostream>
#include <sqlite3.h>
#include "../lib/utils.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  int i;
  for (i = 0; i < argc; i++)
  {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

extern "C"
{

  int create_new_roster_table(const char *rosterName)
  {

    /*
      This takes in a string when its called from a C source file
      this line converts the C string into a C++ string
    */
    std::string cppString(rosterName);

    // print what the C++ string got from the C source file
    // TODO remove after debugging
    std::cout << "The CPP function received string: " << rosterName << std::endl;

    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      std::cerr << "Failed to open SQLite3 database: " << sqlite3_errmsg(db) << std::endl;
      sqlite3_close(db);
      return 1; // Indicate failure
    }

    char createTableSQL[200]; // Adjust the size accordingly

    sprintf(createTableSQL, "CREATE TABLE IF NOT EXISTS %s ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "name TEXT NOT NULL,"
                            "age INTEGER);",
            rosterName);

    // Uncomment the following line if you have a callback function defined
    // rc = sqlite3_exec(db, createTableSQL, callback, nullptr, nullptr);

    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr); // No callback for now

    if (rc != SQLITE_OK)
    {
      std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
      sqlite3_close(db);
      return 4; // Indicate failure
    }

    std::cout << "Database opened successfully" << std::endl;

    sqlite3_close(db);
    return 0; // Indicate success
  }
}
