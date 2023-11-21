/*
===============================================================================
File Name   : db.cpp
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains all functions that interact with the database.
              This includes creating new tables, renaming tables, and showing
              all tables in the database.
===============================================================================
*/

#include <iostream>
#include <sqlite3.h>
#include "../lib/headers/db.hpp"
#include "../lib/utils.h"
#include "../lib/utils.hpp"

/*
Allows using elements from
the 'std' namespace without
prefixing them all with ''
*/
using namespace std;

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
      cout << BOLD << (argv[i] ? argv[i] : "NULL") << RESET << "\n";
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
    string cppString(rosterName);

    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database: ", "create_new_roster_table", CppErrorLevel::CRITICAL);
    }

    char createTableSQL[200];

    sprintf(createTableSQL, "CREATE TABLE IF NOT EXISTS %s ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT)",
            rosterName);

    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);

    sqlite3_close(db);
    return 0;
  }
  //==================================================================================================
  int show_tables()
  {
    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database: ", "show_tables", CppErrorLevel::CRITICAL);
      cerr << RED "Failed to open SQLite3 database" RESET << endl;
      exit(1);
    }

    const char *selectTables = "SELECT name FROM sqlite_master WHERE type='table'";

    rc = sqlite3_exec(db, selectTables, print_table_names_callback, nullptr, nullptr);

    sqlite3_close(db);
    return 0;
  }

  /*
  =======================================================


  */
  /*logic for renaming roster tables in the db*/
  int rename_roster()
  {
    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database: ", "rename_roster", CppErrorLevel::CRITICAL);
      cerr << RED "Failed to open SQLite3 database" RESET << endl;
      exit(1);
    }

    string currentTableName;
    cout << "What table would you like to rename?" << endl;
    getline(cin, currentTableName);

    cout << "You want to rename: " << currentTableName << ". Is that correct? (y/n): " << endl;

    string answer;
    string oldName;
    string newName;
    getline(cin, answer); // Get user's input for the answer

    if (answer == "y" || answer == "Y" || answer == "yes" || answer == "Yes" || answer == "YES")
    {
      cout << YELLOW "Renaming table..." RESET << endl;
      cout << "What would you like to rename it to?" << endl;
      getline(cin, newName);

      string renameTableSQL = "ALTER TABLE " + currentTableName + " RENAME TO " + newName;

      rc = sqlite3_exec(db, renameTableSQL.c_str(), nullptr, nullptr, nullptr);
      oldName = currentTableName;
      currentTableName = newName;
    }
    else if (answer == "n" || answer == "N" || answer == "no" || answer == "No" || answer == "NO")
    {
      cout << "Exiting..." << endl;
      sqlite3_close(db);
      return 0;
    }

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      CPP_UTILS_ERROR_LOGGER("Failed to rename table: ", "rename_roster", CppErrorLevel::CRITICAL);
      cerr << RED "Failed to open SQLite3 database" RESET << endl;
      exit(1);
    }
    system("clear");

    cout << GREEN << "The " BOLD GREEN << oldName + RESET GREEN " roster has successfully been renamed to " BOLD << newName << RESET << endl;
    sqlite3_close(db);
    return 0;
  }
}

int get_table_count()
{
  sqlite3 *db;
  int rc = sqlite3_open("../build/db.sqlite", &db);

  if (rc != SQLITE_OK)
  {
    sqlite3_close(db);
    CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database when trying to get tables from database: ", "get_table_count", CppErrorLevel::MODERATE);
    cerr << RED "Failed to open SQLite3 database" RESET << endl;
    return 1;
  }

  const char *selectTables = "SELECT name FROM sqlite_master WHERE type='table'";

  rc = sqlite3_exec(db, selectTables, print_table_names_callback, nullptr, nullptr);

  sqlite3_close(db);
  return 0;
}