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

bool has_tables = false;

/************************************************************************************
 * callback(): Callback function for sqlite3_exec()
 * Note: This callback function is used to print the results of a query
 ************************************************************************************/

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
/************************************************************************************
 * print_table_names_callback(): Callback function for sqlite3_exec()
 * Note: This callback function is used to print the names of all tables
 ************************************************************************************/
int print_table_names_callback(void *data, int argc, char **argv, char **azColName)
{
  for (int i = 0; i < argc; i++)
  {
    // Exclude sqlite_sequence from the printed tables
    if (strcmp(argv[i], "sqlite_sequence") != 0)
    {
      cout << BOLD << (argv[i] ? argv[i] : "NULL") << RESET << "\n";
      has_tables = true; // Set the flag to true when a table (other than sqlite_sequence) is found
    }
  }

  return 0;
}
extern "C"
{
  /************************************************************************************
   * create_new_roster_table(): Creates a new table in the db.sqlite database
   * Note: See function usage in ../src/_create_roster.c
   ************************************************************************************/
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

    char createSQLTable[200];

    sprintf(createSQLTable, "CREATE TABLE IF NOT EXISTS %s ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT)",
            rosterName);

    // nullptr takes the place of a callback function
    rc = sqlite3_exec(db, createSQLTable, nullptr, nullptr, nullptr);

    sqlite3_close(db);
    return 0;
  }

  /************************************************************************************
   * show_tables(): Prints all rosters currently in the db.sqlite database
   * Note: See function usage in ../src/_create_roster.c & ../src/manage_roster.c
   ************************************************************************************/
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

    const char *selectSQLTables = "SELECT name FROM sqlite_master WHERE type='table'";

    // nullptr takes the place of a callback function
    rc = sqlite3_exec(db, selectSQLTables, nullptr, nullptr, nullptr);

    sqlite3_close(db);
    int tableCountResult = get_table_count();
    return tableCountResult;
  }

  /************************************************************************************
   * rename_roster(): Renames a roster in the db.sqlite database
   * Note: See function usage in ../src/manage_roster.c
   ************************************************************************************/
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
    cout << "What roster would you like to rename?" << endl;
    cout << YELLOW "To cancel this operation, type 'cancel' and press enter" RESET << endl;
    getline(cin, currentTableName);
    if (currentTableName == "cancel")
    {
      system("clear");
      cout << YELLOW "Canceling roster renaming" RESET << endl;
      sqlite3_close(db);
      return 1;
    }
    else
    {

      cout << "You want to rename: " << currentTableName << ". Is that correct? (y/n): " << endl;

      string answer;
      string oldName;
      string newName;
      getline(cin, answer); // Get user's input for the answer

      if (answer == "y" || answer == "Y" || answer == "yes" || answer == "Yes" || answer == "YES")
      {
        cout << YELLOW "Renaming table..." RESET << endl;
        cout << "What would you like to rename it to?" << endl;
        cout << "To cancel this operation, type 'cancel'." << endl;
        getline(cin, newName);

        string renameSQLTable = "ALTER TABLE " + currentTableName + " RENAME TO " + newName;

        rc = sqlite3_exec(db, renameSQLTable.c_str(), nullptr, nullptr, nullptr);
        oldName = currentTableName;
        currentTableName = newName;
      }
      else if (answer == "n" || answer == "N" || answer == "no" || answer == "No" || answer == "NO")
      {
        system("clear");
        cout << "Exiting..." << endl;
        sqlite3_close(db);
        return 0;
      }
      else if (answer == "cancel")
      {
        system("clear");
        cout << YELLOW "Canceling roster renaming" RESET << endl;
        sqlite3_close(db);
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
}

/************************************************************************************
 * get_table_count(): This helper function returns 1 or 0 depending on if any tables
                       were found in the db.sqlite database
 * Note: See function usage in  ../src/_create_roster & ../src/manage_roster.c
 ************************************************************************************/
int get_table_count()
{
  sqlite3 *db;
  int rc = sqlite3_open("../build/db.sqlite", &db);

  if (rc != SQLITE_OK)
  {
    sqlite3_close(db);
    CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database. ", "get_table_count", CppErrorLevel::CRITICAL);
    cerr << RED "CRITICAL ERROR: Failed to find/open database" RESET << endl;
    cout << "Exiting program" << endl;
    sleep(1);
    exit(1);
  }

  const char *selectSQLTables = "SELECT name FROM sqlite_master WHERE type='table'";

  rc = sqlite3_exec(db, selectSQLTables, print_table_names_callback, nullptr, nullptr);
  sqlite3_close(db);

  // Checking if any tables were found
  if (has_tables)
  {
    return TRUE; // Tables found
  }
  else
  {
    return FALSE; // No tables found
  }
}

/************************************************************************************
 * drop_table(): This helper function drops a roster table from the db.sqlite database
 * Note: See function usage in  ../src/manage_roster.c
 ************************************************************************************/
int drop_table(const char *tableName)
{
  sqlite3 *db;
  int rc = sqlite3_open("../build/db.sqlite", &db);

  if (rc != SQLITE_OK)
  {
    CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database. ", "get_table_count", CppErrorLevel::CRITICAL);
    cerr << RED "CRITICAL ERROR: Failed to find/open database" RESET << endl;
    cout << "Exiting program" << endl;
    sleep(1);
    exit(1);
  }

  char dropSQLTable[200];
  sprintf(dropSQLTable, "DROP TABLE IF EXISTS %s", tableName);

  rc = sqlite3_exec(db, dropSQLTable, nullptr, nullptr, nullptr);

  if (rc != SQLITE_OK)
  {
    CPP_UTILS_ERROR_LOGGER("Failed to drop roster table.", "drop_table", CppErrorLevel::MINOR);
    cerr << RED "ERROR: Failed to drop roster table table" RESET << endl;
    return 1;
  }
  sqlite3_close(db);
  return 0;
}