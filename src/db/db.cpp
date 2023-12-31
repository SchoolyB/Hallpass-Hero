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
#include "../lib/headers/utils.h"
#include "../lib/headers/utils.hpp"

using namespace std;

int hasTables = FALSE;
int studentIDExists = FALSE;
const char *dbPath = dbPath;

int __throw_error_opening_db(string functionName, sqlite3 *database, int param)
{
  CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database. ", functionName.c_str(), CppErrorLevel::CRITICAL);
  cerr << RED << "CRITICAL ERROR: Failed to find/open sqlite database" << RESET << endl;
  sqlite3_close(database);
  return -1;
}

int __throw_error_exec_query(string functionName, sqlite3 *database, int param)
{
  CPP_UTILS_ERROR_LOGGER("Failed to execute query ", functionName.c_str(), CppErrorLevel::CRITICAL);
  cerr << RED << "CRITICAL ERROR: Failed to execute query" << RESET << endl;
  sqlite3_close(database);
  return -2;
}

int __throw_error_prepare_statement(string functionName, sqlite3 *database, int param)
{
  CPP_UTILS_ERROR_LOGGER("Failed to prepare SQL statement. ", functionName.c_str(), CppErrorLevel::CRITICAL);
  cerr << RED << "CRITICAL ERROR: Failed to prepare SQL statement" << RESET << endl;
  sqlite3_close(database);
  return -3;
}

int __throw_error_statement_step(string functionName, sqlite3 *database, int param, sqlite3_stmt *statement)
{
  CPP_UTILS_ERROR_LOGGER("Failed to execute SQL statement step. ", functionName.c_str(), CppErrorLevel::CRITICAL);
  cerr << RED << "Error executing SQL statement: " << sqlite3_errmsg(database) << endl;
  sqlite3_finalize(statement);
  sqlite3_close(database);
  return -4;
}

int table_exists_callback(void *exists, int argc, char **argv, char **columnNames)
{
  hasTables = (argc > 0 && argv[0]);
  return 0;
}

int student_id_exists_callback(void *data, int argc, char **argv, char **azColName)
{
  studentIDExists = (argc > 0);

  return 0;
}
/************************************************************************************
 * print_student_info_callback(): Callback function for sqlite3_exec()
 * Note: This callback function is used to print the names of all students
 ************************************************************************************/
int print_student_info_callback(void *data, int argc, char **argv, char **azColName)
{
  cout << "-------------------------------------------" << endl;
  for (int i = 0; i < argc; i++)
  {
    printf(BOLD "| %-10s" RESET, argv[i] ? argv[i] : "NULL");
  }
  cout << "\n-------------------------------------------" << endl;

  // printf("\n");
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
      hasTables = TRUE; // Set the flag to true when a table (other than sqlite_sequence) is found
    }
  }

  return 0;
}

// todo come back to this and use it when showing all of a roster's data
int show_table_data_callback(void *data, int argc, char **argv, char **azColName)
{
  for (int i = 0; i < argc; i++)
  {
    printf(BOLD "| %-10s" RESET, argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int callback(void *notUsed, int argc, char **argv, char **azColName)
{
  // Print the table names directly
  for (int i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], "sqlite_sequence") != 0)
    {
      cout << BOLD << (argv[i] ? argv[i] : "NULL") << RESET << "\n";
      hasTables = TRUE; // Set the flag to true when a table (other than sqlite_sequence) is found
    }
  }
  return 0;
}

/************************************************************************************
 * check_if_table_exists(): Simply checks if the passed in string is an existing table
 * Note:
 ************************************************************************************/
// TODO add usage locations to comment ^^^.
int check_if_table_exists(const char *rosterName)
{
  hasTables = FALSE;
  string cppString(rosterName);
  sqlite3 *database;
  int dbConnection = sqlite3_open(dbPath, &database);

  if (dbConnection != SQLITE_OK)
  {
    __throw_error_opening_db("check_if_table_exists", database, dbConnection);
  }
  string SQLTableNameExistsCheck = "SELECT name FROM sqlite_master WHERE type ='table' AND name ='" + cppString + "'";
  dbConnection = sqlite3_exec(database, SQLTableNameExistsCheck.c_str(), table_exists_callback, &hasTables, nullptr);
  if (dbConnection != SQLITE_OK)
  {
    __throw_error_exec_query("check_if_table_exists", database, dbConnection);
  }

  if (hasTables)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/************************************************************************************
 * refresh_table_count(): This helper function resets the hasTables flag to FALSE
 * Note: See function usage in the drop_table() function
 ************************************************************************************/
// todo do I need this??? cant i just the 'hasTables' reset variable value whenever this function is called?
int refresh_table_count(void)
{
  hasTables = FALSE;
  return 0;
}
/*Simple helper that prints a heading when showing the student list*/
int print_student_list_heading(void)
{
  printf("%-15s %-15s %-15s\n", "First Name", "Last Name", "Student ID");
  return 0;
}
extern "C"
{
#include <sqlite3.h>
  /************************************************************************************
   * create_new_roster_table(): Creates a new table in the db.sqlite database
   * Note: See function usage in ../src/_create_roster.c
   ************************************************************************************/
  int create_new_roster_table(const char *rosterName)
  {
    // Convert the C string to a C++ string
    string cppString(rosterName);
    sqlite3 *database;

    int dbConnection = sqlite3_open(dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("create_new_roster_table", database, dbConnection);
    }

    string createSQLTable = "CREATE TABLE IF NOT EXISTS " + cppString + " (id INTEGER PRIMARY KEY AUTOINCREMENT, FirstName TEXT, LastName TEXT, StudentID TEXT)";
    const char *createSQLTableChar = createSQLTable.c_str();
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_exec_query("create_new_roster_table", database, dbConnection);
    }
    sqlite3_stmt *statement;

    dbConnection = sqlite3_prepare_v2(database, createSQLTableChar, -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("create_new_roster_table", database, dbConnection);
    }

    dbConnection = sqlite3_step(statement);
    sqlite3_finalize(statement);
    sqlite3_close(database);
    return 0;
  }

  /************************************************************************************
   * show_tables(): Prints all rosters currently in the db.sqlite database
   * Note: See function usage in ../src/_create_roster.c & ../src/_manage_roster.c
   ************************************************************************************/
  int show_tables(void)
  {
    sqlite3 *database;
    int dbConnection = sqlite3_open(dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("show_tables", database, dbConnection);
    }
    const char *selectSQLTables = "SELECT name FROM sqlite_master WHERE type='table' AND name LIKE 'Roster%'";

    // Print the header
    cout << "==========================================================================================" << endl;
    cout << BOLD << "Created rosters:" << RESET << endl;
    cout << "------------------------------------------------------------------------------------------" << endl;
    dbConnection = sqlite3_exec(database, selectSQLTables, callback, nullptr, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_exec_query("show_tables", database, dbConnection);
    }

    cout << "==========================================================================================" << endl;

    sqlite3_close(database);

    return 0;

    // If you want to get the count of tables, you can call your get_table_count function here
    // int tableCountResult = get_table_count(dbPath);
    // return tableCountResult;
  }
  /************************************************************************************
   * rename_roster(): Renames a roster in the db.sqlite database
   * Note: See function usage in ../src/_manage_roster.c
   ************************************************************************************/
  int rename_roster(const char *oldRosterName, const char *newRosterName)
  {
    string oldRosterNameString(oldRosterName);
    string newRosterNameString(newRosterName);
    sqlite3 *database;
    int dbConnection = sqlite3_open(dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("rename_roster", database, dbConnection);
    }
    string renameSQLTable = "ALTER TABLE " + oldRosterNameString + " RENAME TO " + "Roster_" + newRosterNameString;

    sqlite3_stmt *statement;

    dbConnection = sqlite3_prepare_v2(database, renameSQLTable.c_str(), -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("rename_roster", database, dbConnection);
    }

    dbConnection = sqlite3_step(statement);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_statement_step("rename_roster", database, dbConnection, statement);
    }
    sqlite3_finalize(statement);
    sqlite3_close(database);

    return 0;
  }

  /************************************************************************************
   * add_col_to_roster(): Adds the passed in col name to the passed in roster
   * Note: See function usage in  ../src/_manage_roster.c
   ************************************************************************************/
  int add_col_to_roster(const char *rosterName, const char *colName, const char *colType)
  {
    string rosterNameString(rosterName);
    sqlite3 *database;
    int dbConnection = sqlite3_open(dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("add_col_to_roster", database, dbConnection);
    }
    string addColToSQLTable = "ALTER TABLE " + rosterNameString + " ADD " + colName + " " + colType;
    const char *addColToSQLTableChar = addColToSQLTable.c_str();

    sqlite3_stmt *statement;
    dbConnection = sqlite3_prepare_v2(database, addColToSQLTableChar, -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("add_col_to_roster", database, dbConnection);
    }

    dbConnection = sqlite3_step(statement);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_statement_step("add_col_to_roster", database, dbConnection, statement);
    }

    sqlite3_finalize(statement);
    sqlite3_close(database);
    return 0;
  }

  int delete_col_from_roster(const char *rosterName, const char *colName)
  {
    string rosterNameString(rosterName);
    sqlite3 *database;
    int dbConnection = sqlite3_open(dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("", database, dbConnection);
    }

    string deleteColFromSQLTable = "ALTER TABLE " + rosterNameString + " DROP COLUMN " + colName;
    const char *deleteColFromSQLTableChar = deleteColFromSQLTable.c_str();

    sqlite3_stmt *statement;
    dbConnection = sqlite3_prepare_v2(database, deleteColFromSQLTableChar, -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {

      __throw_error_prepare_statement("delete_col_from_roster", database, dbConnection);
    }

    dbConnection = sqlite3_step(statement);
    if (dbConnection != SQLITE_DONE)
    {
      __throw_error_statement_step("delete_col_from_roster", database, dbConnection, statement);
    }

    sqlite3_finalize(statement);
    sqlite3_close(database);
    return 0;
  }

  // man idk. Shout out to ChatGPT for this function...What the fuck

  int check_if_col_exists(const char *rosterName, const char *colName)
  {
    string rosterNameString(rosterName);

    sqlite3 *database;

    int dbConnection = sqlite3_open(dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("check_if_col_exists", database, dbConnection);
    }

    string checkIfColExistsInSQLTable = "PRAGMA table_info(" + rosterNameString + ");";
    const char *checkIfColExistsInSQLTableChar = checkIfColExistsInSQLTable.c_str();

    sqlite3_stmt *statement;

    dbConnection = sqlite3_prepare_v2(database, checkIfColExistsInSQLTableChar, -1, &statement, nullptr);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("check_if_col_exists", database, dbConnection);
    }

    bool colExists = false;

    while (sqlite3_step(statement) == SQLITE_ROW)
    {
      const char *columnName = reinterpret_cast<const char *>(sqlite3_column_text(statement, 1));
      if (strcmp(columnName, colName) == 0)
      {
        colExists = true;
        break; // break out of the loop if the column is found
      }
    }
    sqlite3_finalize(statement);
    sqlite3_close(database);

    return colExists ? 0 : 1;
  }

  /************************************************************************************
   * drop_table(): Drops a roster table from the db.sqlite database
   * Note: See function usage in  ../src/_manage_roster.c
   ************************************************************************************/
  int drop_table(const char *tableName)
  {
    string tableNameString(tableName);
    sqlite3 *database;
    int dbConnection = sqlite3_open(dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("drop_table", database, dbConnection);
    }

    string dropSQLTable = "DROP TABLE " + tableNameString;

    sqlite3_stmt *statement;
    dbConnection = sqlite3_prepare_v2(database, dropSQLTable.c_str(), -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("drop_table", database, dbConnection);
    }

    dbConnection = sqlite3_step(statement);

    if (dbConnection != SQLITE_DONE)
    {
      __throw_error_statement_step("drop_table", database, dbConnection, statement);
    }

    sqlite3_finalize(statement);
    sqlite3_close(database);
    return 0;
  }
  /************************************************************************************
  * get_table_count(): This helper function returns 1 or 0 depending on if any tables
                       were found in the db.sqlite database
  * Note: See function usage in  ../src/_create_roster & ../src/_manage_roster.c
  ************************************************************************************/
  int get_table_count(const char *path)
  {
    sqlite3 *database;
    int dbConnection = sqlite3_open(path, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("get_table_count", database, dbConnection);
    }

    const char *selectSQLTables = "SELECT name FROM sqlite_master WHERE type='table'";

    dbConnection = sqlite3_exec(database, selectSQLTables, callback, nullptr, nullptr);
    sqlite3_close(database);

    // Checking if any tables were found
    if (hasTables)
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }

  int add_student_to_roster(const char *rosterName, const char *firstName, const char *lastName, const char *studentID)
  {
    string rosterNameString(rosterName);

    sqlite3 *database;

    int dbConnection = sqlite3_open(dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("add_student_to_roster", database, dbConnection);
    }

    string addStudentToSQLTable = "INSERT INTO " + rosterNameString + " (FirstName, LastName, StudentID) VALUES (?, ?, ?)";
    const char *addStudentToSQLTableChar = addStudentToSQLTable.c_str();
    sqlite3_stmt *statement;

    dbConnection = sqlite3_prepare_v2(database, addStudentToSQLTableChar, -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      CPP_UTILS_ERROR_LOGGER("Can't prepare SQL statement", "add_student_to_roster", CppErrorLevel::CRITICAL);
      sqlite3_close(database);
      return -1;
    }

    sqlite3_bind_text(statement, 1, firstName, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, lastName, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, studentID, -1, SQLITE_STATIC);

    dbConnection = sqlite3_step(statement);

    sqlite3_finalize(statement);
    sqlite3_close(database);

    if (dbConnection != SQLITE_DONE)
    {
      // Handle the error
      CPP_UTILS_ERROR_LOGGER("Execution failed: ", "add_student_to_roster", CppErrorLevel::CRITICAL);
      return -2;
    }

    return TRUE; // Success...this just makes more sense to me than returning 0. The frontend wants 1
  }
  /***************!EVERYTHING BELOW HERE IS FOR THE STUDENTS DATABASE!***************/
  /***************!EVERYTHING BELOW HERE IS FOR THE STUDENTS DATABASE!***************/
  /***************!EVERYTHING BELOW HERE IS FOR THE STUDENTS DATABASE!***************/
  /***************!EVERYTHING BELOW HERE IS FOR THE STUDENTS DATABASE!***************/

  int check_if_student_id_exists(const char *ID)
  {
    studentIDExists = FALSE;
    string cppString(ID);

    sqlite3 *database;
    int dbConnection = sqlite3_open(dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      sqlite3_close(database);
      cerr << "Failed to open SQLite3 database." << endl;
      return 1;
    }

    string SQLStudentIDExistsCheck = "SELECT * FROM students WHERE StudentID  ='" + cppString + "'";
    dbConnection = sqlite3_exec(database, SQLStudentIDExistsCheck.c_str(), student_id_exists_callback, &studentIDExists, nullptr);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_exec_query("check_if_student_id_exists", database, dbConnection);
    }

    if (studentIDExists)
    {
      return TRUE;
    }
    else
    {

      return FALSE;
    }
  }

  /************************************************************************************
   *  create_student_db_and_table();: Begins the process of adding a student to the db.sqlite database
   * Note: See function usage in  ../src/_add_student.c
   * Note: THis function relies on the following helper functions:
   ************************************************************************************/
  int create_student_db_and_table()
  {

    sqlite3 *database;
    int dbConnection = sqlite3_open(dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      sqlite3_close(database);
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database: ", "add_student_to_student_db", CppErrorLevel::CRITICAL);
      cerr << RED "Failed to open SQLite3 database" RESET << endl;
      return 1;
    }
    const char *addStudentToSQLTable = "CREATE TABLE IF NOT EXISTS students (id INTEGER PRIMARY KEY AUTOINCREMENT, FirstName, LastName,StudentID INTEGER)";

    sqlite3_exec(database, addStudentToSQLTable, nullptr, nullptr, nullptr);

    if (dbConnection != SQLITE_OK)
    {
      sqlite3_close(database);
      CPP_UTILS_ERROR_LOGGER("Failed to add student to database: ", "add_student_to_student_db", CppErrorLevel::CRITICAL);
      cerr << RED "Failed to add student to database" RESET << endl;
      return 1;
    }

    sqlite3_close(database);
    return 0;
  }

  /************************************************************************************
   *  insert_student_into_db();: Inserts a student into the db.sqlite database using the data from the student struct
   * Note: See function usage in  ../src/_add_student.c
   ************************************************************************************/
  int insert_student_into_db(const char *FirsName, const char *LastName, const char *StudentID)
  {
    sqlite3 *database;
    int dbConnection = sqlite3_open(dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      sqlite3_close(database);
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database: ", "insert_student_into_db", CppErrorLevel::CRITICAL);
      cerr << RED "Failed to open SQLite3 database" RESET << endl;
      return 1;
    }

    const char *insertStudentToSQLTable = "INSERT INTO students (FirstName, LastName, StudentID) VALUES (?, ?, ?)";

    sqlite3_stmt *statement;

    dbConnection = sqlite3_prepare_v2(database, insertStudentToSQLTable, -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      CPP_UTILS_ERROR_LOGGER("Can't prepare SQL statement", "insert_student_into_db", CppErrorLevel::CRITICAL);
      sqlite3_close(database);
      return 1;
    }

    // Bind the value
    sqlite3_bind_text(statement, 1, FirsName, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, LastName, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, StudentID, -1, SQLITE_STATIC);

    dbConnection = sqlite3_step(statement);
    return 0;
  }

  /************************************************************************************
   * show_student_list(): Prints all students currently in the db.sqlite database
   * Note: See function usage in ../src/_add_student.c & ../src/_manage_student.c
   ************************************************************************************/
  int show_students_in_db(const char *path)
  {
    sqlite3 *database;
    int dbConnection = sqlite3_open(path, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("show_students_in_db", database, dbConnection);
    }
    print_student_list_heading();
    // Construct the SQL query to select all rows from the specified table
    string selectFromStudentSQLTable = "SELECT FirstName, LastName, StudentID FROM students";

    // Execute the query and invoke the callback function to print each row
    dbConnection = sqlite3_exec(database, selectFromStudentSQLTable.c_str(), print_student_info_callback, nullptr, nullptr);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_exec_query("show_students_in_db", database, dbConnection);
    }

    // Close the database
    sqlite3_close(database);
    return 0;
  }
}

/************************************************************************************
 *  query_db_for_student_name(): Takes in a string from the frontend and queries
 *                                      the db.sqlite database for a match
 * Note: See function usage in  ../src/_manage_student.c & ../src/_search.c
 ************************************************************************************/
int query_db_for_student_name(const char *searchParam)
{
  sqlite3 *database;
  sqlite3_stmt *statement;
  int dbConnection = sqlite3_open(dbPath, &database);

  if (dbConnection != SQLITE_OK)
  {
    __throw_error_opening_db("query_db_for_student_name", database, dbConnection);
  }

  const char *sql = "SELECT * FROM students WHERE FirstName = ? OR LastName = ? OR StudentID = ?";

  if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) == SQLITE_OK)
  {
    sqlite3_bind_text(statement, 1, searchParam, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, searchParam, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, searchParam, -1, SQLITE_STATIC);

    while (sqlite3_step(statement) == SQLITE_ROW)
    {
      system("clear");
      sleep(1);
      cout << BOLD << "First Name: " << RESET << sqlite3_column_text(statement, 1) << endl;
      cout << BOLD << "Last Name: " << RESET << sqlite3_column_text(statement, 2) << endl;
      cout << BOLD << "Student ID: " << RESET << sqlite3_column_text(statement, 3) << endl;
      cout << "-------------------------------------------" << endl;
    }

    sqlite3_finalize(statement);
    return 0;
  }
  else
  {
    cerr << "Failed to prepare the statement: " << sqlite3_errmsg(database) << endl;
    sqlite3_close(database);
    return -1;
  }

  sqlite3_close(database);

  return 0;
}
