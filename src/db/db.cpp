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
int print_table_names_callback(void *data, int argc, char **argv, char **azColName)
{
  for (int i = 0; i < argc; i++)
  {
    // prints all tables in sqlite_master except sqlite_sequence
    if (strcmp(argv[i], "sqlite_sequence") != 0)
    {
      std::cout << BOLD << (argv[i] ? argv[i] : "NULL") << RESET << "\n";
    }
  }

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

    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      UTILS_ERROR_LOGGER("Failed to open SQLite3 database", "create_new_roster_table", MODERATE);
      sqlite3_close(db);
    }

    char createTableSQL[200];

    sprintf(createTableSQL, "CREATE TABLE IF NOT EXISTS %s ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT)",
            rosterName);

    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);

    sqlite3_close(db);
    return 0; // Indicate success
  }
  //==================================================================================================
  int show_tables()
  {
    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      UTILS_ERROR_LOGGER("Failed to open SQLite3 database", "show_tables", MODERATE);
      sqlite3_close(db);
    }

    const char *selectTables = "SELECT name FROM sqlite_master WHERE type='table'";

    rc = sqlite3_exec(db, selectTables, print_table_names_callback, nullptr, nullptr);

    sqlite3_close(db);
    return 0;
  }

  //==================================================================================================

  int rename_roster()
  {
    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      std::cerr << "Failed to open SQLite3 database: " << sqlite3_errmsg(db) << std::endl;
      sqlite3_close(db);
      return 1; // Indicate failure
    }

    std::string currentTableName;
    std::cout << "What table would you like to rename?" << std::endl;
    std::getline(std::cin, currentTableName);

    std::cout << "You want to rename: " << currentTableName << ". Is that correct? (y/n): " << std::endl;

    std::string answer;
    std::string oldName;
    std::string newName;
    std::getline(std::cin, answer); // Get user's input for the answer

    if (answer == "y" || answer == "Y" || answer == "yes" || answer == "Yes" || answer == "YES")
    {
      std::cout << YELLOW "Renaming table..." RESET << std::endl;
      std::cout << "What would you like to rename it to?" << std::endl;
      std::getline(std::cin, newName);

      std::string renameTableSQL = "ALTER TABLE " + currentTableName + " RENAME TO " + newName;

      rc = sqlite3_exec(db, renameTableSQL.c_str(), nullptr, nullptr, nullptr);
      oldName = currentTableName;
      currentTableName = newName;
    }
    else if (answer == "n" || answer == "N" || answer == "no" || answer == "No" || answer == "NO")
    {
      std::cout << "Exiting..." << std::endl;
      sqlite3_close(db);
      return 0;
    }

    if (rc != SQLITE_OK)
    {
      std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
      sqlite3_close(db);
      return 1; // Indicate failure
    }
    system("clear");

    std::cout << GREEN << "The " BOLD GREEN << oldName + RESET GREEN " roster has successfully been renamed to " BOLD << newName << RESET << std::endl;
    sqlite3_close(db);
    return 0; // Indicate success
  }
}
