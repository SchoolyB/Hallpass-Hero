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
int has_tables = FALSE;

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
      has_tables = TRUE; // Set the flag to true when a table (other than sqlite_sequence) is found
    }
  }

  return 0;
}

/************************************************************************************
 * refresh_table_count(): This helper function resets the has_tables flag to FALSE
 * Note: See function usage in the drop_table() function
 ************************************************************************************/
int refresh_table_count(void)
{
  has_tables = FALSE;
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
    // Convert the C string to a C++ string
    std::string cppString(rosterName);

    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database", "create_new_roster_table", CppErrorLevel::CRITICAL);
      return rc;
    }

    // the ? is a placeholder for the rosterName variable
    const char *createSQLTable = "CREATE TABLE IF NOT EXISTS ? (id INTEGER PRIMARY KEY AUTOINCREMENT)";
    sqlite3_stmt *statement;

    // this compiles the SQL statement into byte-code
    rc = sqlite3_prepare_v2(db, createSQLTable, -1, &statement, nullptr);
    if (rc != SQLITE_OK)
    {
      CPP_UTILS_ERROR_LOGGER("Can't prepare SQL statement", "create_new_roster_table", CppErrorLevel::CRITICAL);
      sqlite3_close(db);
      return rc;
    }

    // this binds the rosterName variable to the ? placeholder
    rc = sqlite3_bind_text(statement, 1, rosterName, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
      CPP_UTILS_ERROR_LOGGER("Can't bind SQL parameter", "create_new_roster_table", CppErrorLevel::CRITICAL);
      sqlite3_finalize(statement);
      sqlite3_close(db);
      return rc;
    }

    // this executes the statement
    rc = sqlite3_step(statement);
    if (rc != SQLITE_DONE)
    {
      CPP_UTILS_ERROR_LOGGER("Error executing SQL statement", "create_new_roster_table", CppErrorLevel::CRITICAL);
    }

    // this finalizes the statement
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return 0;
  }
  /************************************************************************************
   * show_tables(): Prints all rosters currently in the db.sqlite database
   * Note: See function usage in ../src/_create_roster.c & ../src/manage_roster.c
   ************************************************************************************/
  int show_tables(void)
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
  int rename_roster(void)
  {
    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database", "rename_roster", CppErrorLevel::CRITICAL);
      std::cerr << "Failed to open SQLite3 database" << std::endl;
      exit(1);
    }

    std::string currentTableName;
    std::cout << "What roster would you like to rename?" << std::endl;
    std::cout << "To cancel this operation, type 'cancel'." << std::endl;
    getline(std::cin, currentTableName);

    if (currentTableName == "cancel")
    {
      std::cout << "Canceling roster renaming" << std::endl;
      sqlite3_close(db);
      return 1;
    }
    else
    {
      std::cout << "You want to rename: " << currentTableName << ". Is that correct? (y/n): " << std::endl;
      std::cout << "To cancel this operation, type 'cancel'." << std::endl;
      std::string answer, newName;

      getline(std::cin, answer);

      if (answer == "y" || answer == "Y" || answer == "yes" || answer == "Yes" || answer == "YES")
      {
        std::cout << "Renaming " << currentTableName << std::endl;
        std::cout << "What would you like to rename this roster to?" << std::endl;
        std::cout << "To cancel this operation, type 'cancel'." << std::endl;
        getline(std::cin, newName);

        if (newName == "cancel")
        {
          std::cout << "Canceling roster renaming" << std::endl;
          sqlite3_close(db);
          return 1;
        }

        const char *renameSQLTable = "ALTER TABLE ? RENAME TO ?";
        sqlite3_stmt *stmt;

        rc = sqlite3_prepare_v2(db, renameSQLTable, -1, &stmt, nullptr);
        if (rc != SQLITE_OK)
        {
          CPP_UTILS_ERROR_LOGGER("Can't prepare SQL statement", "rename_roster", CppErrorLevel::CRITICAL);
          sqlite3_close(db);
          return rc;
        }

        rc = sqlite3_bind_text(stmt, 1, currentTableName.c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK)
        {
          CPP_UTILS_ERROR_LOGGER("Can't bind SQL parameter 1", "rename_roster", CppErrorLevel::CRITICAL);
          sqlite3_finalize(stmt);
          sqlite3_close(db);
          return rc;
        }

        rc = sqlite3_bind_text(stmt, 2, newName.c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK)
        {
          CPP_UTILS_ERROR_LOGGER("Can't bind SQL parameter 2", "rename_roster", CppErrorLevel::CRITICAL);
          sqlite3_finalize(stmt);
          sqlite3_close(db);
          return rc;
        }

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE)
        {
          CPP_UTILS_ERROR_LOGGER("Error executing SQL statement", "rename_roster", CppErrorLevel::CRITICAL);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        std::cout << "The roster has successfully been renamed to " << newName << std::endl;
        return 2;
      }
      else if (answer == "n" || answer == "N" || answer == "no" || answer == "No" || answer == "NO")
      {
        std::cout << "Exiting..." << std::endl;
        sqlite3_close(db);
        return 1;
      }
      else if (answer == "cancel")
      {
        std::cout << "Canceling roster renaming" << std::endl;
        sqlite3_close(db);
        return 1;
      }

      if (rc != SQLITE_OK)
      {
        CPP_UTILS_ERROR_LOGGER("Failed to rename table", "rename_roster", CppErrorLevel::CRITICAL);
        std::cerr << "Failed to rename table" << std::endl;
        exit(1);
      }
      return 0;
    }
  }

  /************************************************************************************
   * drop_table(): This helper function drops a roster table from the db.sqlite database
   * Note: See function usage in  ../src/manage_roster.c
   ************************************************************************************/
  int drop_table(void)
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
    cout << "What roster would you like to delete?" << endl;
    cout << RED << "WARNING: This action cannot be undone!" RESET << endl;
    cout << YELLOW "To cancel this operation, type 'cancel'." RESET << endl;
    getline(cin, currentTableName);
    if (currentTableName == "cancel")
    {
      system("clear");
      cout << YELLOW "Canceling roster deletion" RESET << endl;
      sqlite3_close(db);
      return 1;
    }
    else
    {
      system("clear");
      cout << "To confirm the deletion of " BOLD << currentTableName << RESET " enter 'delete'." << endl;

      cout << YELLOW "To cancel this operation, type 'cancel'." RESET << endl;
      string answer;
      getline(cin, answer); // Get user's input for the answer

      if (answer == "delete" || answer == "DELETE")
      {
        system("clear");
        cout << YELLOW "Deleting" << currentTableName << RESET << endl;
        string deleteSQLTable = "DROP TABLE " + currentTableName;

        rc = sqlite3_exec(db, deleteSQLTable.c_str(), nullptr, nullptr, nullptr);
        system("clear");
        cout << GREEN << "The roster" BOLD GREEN << currentTableName + RESET GREEN " roster has successfully been deleted" RESET << endl;
        sqlite3_close(db);
        refresh_table_count();
        return 2;
      }
      else if (answer == "cancel")
      {
        system("clear");
        cout << YELLOW "Canceling roster deletion" RESET << endl;
        sqlite3_close(db);
        return 1;
      }
      else
      {
        cout << "Please make a valid decision" << endl;
        return 1;
      }

      if (rc != SQLITE_OK)
      {
        sqlite3_close(db);
        CPP_UTILS_ERROR_LOGGER("Failed to delete roster table: ", "delete_roster", CppErrorLevel::CRITICAL);
        cerr << RED "Failed to open SQLite3 database" RESET << endl;
        exit(1);
      }
      return 0;
    }
  }
  /************************************************************************************
 * get_table_count(): This helper function returns 1 or 0 depending on if any tables
                       were found in the db.sqlite database
 * Note: See function usage in  ../src/_create_roster & ../src/manage_roster.c
 ************************************************************************************/
  int get_table_count(void)
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
}
