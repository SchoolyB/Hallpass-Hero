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

//------------------------------------------ERROR HANDLING------------------------------------------//

/************************************************************************************
 * __throw_error_opening_db(): Handles errors and error logging in the event
 *                             the db cant be found.
 ************************************************************************************/
int __throw_error_opening_db(string functionName, sqlite3 *database, int param)
{
  cpp_utils_error_logger("Failed to open SQLite3 database. ", functionName.c_str(), CppErrorLevel::CRITICAL);
  cerr << RED << "CRITICAL ERROR: Failed to find/open sqlite database" << RESET << endl;
  sqlite3_close(database);
}

/************************************************************************************
 * __throw_error_exec_query(): Handles errors and error logging in the event
 *                             that the a SQL query is not successfully executed.
 ************************************************************************************/
int __throw_error_exec_query(string functionName, sqlite3 *database, int param)
{
  cpp_utils_error_logger("Failed to execute query. ", functionName.c_str(), CppErrorLevel::CRITICAL);
  cerr << RED << "CRITICAL ERROR: Failed to execute query" << sqlite3_errmsg(database) << RESET << endl;
  sqlite3_close(database);
}

/************************************************************************************
 * __throw_error_prepare_statement(): Handles errors and error logging in
 *
 *                                    that the prepare statement is not successful.
 ************************************************************************************/
int __throw_error_prepare_statement(string functionName, sqlite3 *database, int param)
{
  cpp_utils_error_logger("Failed to prepare SQL statement. ", functionName.c_str(), CppErrorLevel::CRITICAL);
  cerr << RED << "CRITICAL ERROR: Failed to prepare SQL statement" << sqlite3_errmsg(database) << RESET << endl;
  sqlite3_close(database);
}

/************************************************************************************
 * __throw_error_statement_step(): Handles errors and error logging in the event
 *                                 that the statement step is not successful.
 ************************************************************************************/
int __throw_error_statement_step(string functionName, sqlite3 *database, int param, sqlite3_stmt *statement)
{
  cpp_utils_error_logger("Failed to execute SQL statement step. ", functionName.c_str(), CppErrorLevel::CRITICAL);
  cerr << RED << "CRITICAL ERROR: Failed to execute SQL statement step: " << sqlite3_errmsg(database) << RESET << endl;
  sqlite3_close(database);
}
//------------------------------------------END ERROR HANDLING------------------------------------------//

//------------------------------------------CALLBACKS------------------------------------------//

/************************************************************************************
 * table_exists_callback(): Used to check if a table exists in the db.sqlite database
 ************************************************************************************/
int table_exists_callback(void *exists, int argc, char **argv, char **columnNames)
{
  hasTables = (argc > 0 && argv[0]);
  return 0;
}

/************************************************************************************
 * student_id_exists_callback(): Used to check if a studentID exists in the student db
 ************************************************************************************/

int student_id_exists_callback(void *data, int argc, char **argv, char **azColName)
{
  studentIDExists = (argc > 0);

  return 0;
}
/************************************************************************************
 * print_all_students_in_student_db_callback(): Used to print all students that are
 *                                              currently in the student db
 ************************************************************************************/
int print_all_students_in_student_db_callback(void *data, int argc, char **argv, char **azColName)
{
  cout << "-------------------------------------------" << endl;
  for (int i = 0; i < argc; i++)
  {
    printf(BOLD "| %-10s" RESET, argv[i] ? argv[i] : "NULL");
  }
  cout << "\n-------------------------------------------" << endl;
  return 0;
}

/************************************************************************************
 * print_all_student_data_in_roster_callback(): Used to print all the data in a roster
 ************************************************************************************/
int print_all_student_data_in_roster_callback(void *data, int argc, char **argv, char **colNames)
{
  if (argc <= 0)
  {
    // TODO need to handle this error better
    return -1; // No data, do nothing
  }

  // Display data
  printf("-------------------------------------------------------------------------------\n");
  for (int i = 0; i < argc; i++)
  {
    if (argv[i] != nullptr)
    {
      printf("%-15s | ", argv[i]);
    }
  }
  printf("\n");
  printf("-------------------------------------------------------------------------------\n");

  return 0;
}

/************************************************************************************
 * print_table_names_callback(): Used to print the names of all tables currently in
 *                               the db.sqlite database
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

//------------------------------------------END CALLBACKS------------------------------------------//

//------------------------------------------HELPER FUNCTIONS------------------------------------------//

/************************************************************************************
 * check_if_table_exists(): Checks if the passed in roster exists in the db.sqlite
 * Note: See usage in: _create_roster.c
 *                     _manage_roster.c
 *                     _add_student.c
 ************************************************************************************/
int check_if_table_exists(const char *rosterName)
{
  hasTables = FALSE;
  string cppString(rosterName);
  sqlite3 *database;
  int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);

  if (dbConnection != SQLITE_OK)
  {
    __throw_error_opening_db("check_if_table_exists", database, dbConnection);
    return -1;
  }
  string SQLTableNameExistsCheck = "SELECT name FROM sqlite_master WHERE type ='table' AND name ='" + cppString + "'";
  dbConnection = sqlite3_exec(database, SQLTableNameExistsCheck.c_str(), table_exists_callback, &hasTables, nullptr);
  if (dbConnection != SQLITE_OK)
  {
    __throw_error_exec_query("check_if_table_exists", database, dbConnection);
    return -2;
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
   * Note: See function usage in _create_roster.c
   ************************************************************************************/
  int create_new_roster_table(const char *rosterName)
  {
    // Convert the C string to a C++ string
    string cppString(rosterName);
    sqlite3 *database;

    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("create_new_roster_table", database, dbConnection);
      return -1;
    }

    string createSQLTable = "CREATE TABLE IF NOT EXISTS " + cppString + " (id INTEGER PRIMARY KEY AUTOINCREMENT, FirstName TEXT, LastName TEXT, StudentID TEXT)";
    const char *createSQLTableChar = createSQLTable.c_str();
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_exec_query("create_new_roster_table", database, dbConnection);
      return -2;
    }
    sqlite3_stmt *statement;

    dbConnection = sqlite3_prepare_v2(database, createSQLTableChar, -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("create_new_roster_table", database, dbConnection);
      return -3;
    }

    dbConnection = sqlite3_step(statement);
    sqlite3_finalize(statement);
    sqlite3_close(database);
    return 0;
  }

  /************************************************************************************
   * show_tables(): Prints all rosters currently in the db.sqlite database
   * Note: See function usage in:
   *  _create_roster.c
   *  _manage_roster.c
   *  _add_student.c
   ************************************************************************************/
  int show_tables(void)
  {
    sqlite3 *database;
    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("show_tables", database, dbConnection);
      return -1;
    }
    const char *selectSQLTables = "SELECT name FROM sqlite_master WHERE type='table' AND name LIKE 'Roster%'";

    // Print the header
    cout << "==========================================================================================" << endl;
    cout << BOLD << "Created rosters:" << RESET << endl;
    cout << "------------------------------------------------------------------------------------------" << endl;
    dbConnection = sqlite3_exec(database, selectSQLTables, print_table_names_callback, nullptr, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_exec_query("show_tables", database, dbConnection);
      return -2;
    }

    cout << "==========================================================================================" << endl;

    sqlite3_close(database);

    return 0;
  }

  //------------------------------END HELPER FUNCTIONS------------------------------------------//

  /************************************************************************************
   * check_for_data_in_roster(): Checks if the passed in roster has any data
   * Note: See function usage in _manage_roster.c
   ************************************************************************************/
  int check_for_data_in_roster(const char *rosterName)
  {
    string rosterNameString(rosterName);
    sqlite3 *database;
    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("check_for_data_in_roster", database, dbConnection);
      return -1;
    }

    string checkForDataSQL = "SELECT * FROM " + rosterNameString;
    const char *checkForDataSQLChar = checkForDataSQL.c_str();

    sqlite3_stmt *statement;
    dbConnection = sqlite3_prepare_v2(database, checkForDataSQLChar, -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("check_for_data_in_roster", database, dbConnection);
      return -3;
    }

    dbConnection = sqlite3_step(statement);
    if (dbConnection != SQLITE_ROW)
    {
      sqlite3_finalize(statement);
      sqlite3_close(database);
      return FALSE;
    }
    else
    {
      sqlite3_finalize(statement);
      sqlite3_close(database);
      return TRUE;
    }
  }

  /************************************************************************************
   * rename_roster(): Renames a roster in the db.sqlite database
   * Note: See function usage in _manage_roster.c
   ************************************************************************************/
  int rename_roster(const char *oldRosterName, const char *newRosterName)
  {
    string oldRosterNameString(oldRosterName);
    string newRosterNameString(newRosterName);
    sqlite3 *database;
    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("rename_roster", database, dbConnection);
      return -1;
    }
    string renameSQLTable = "ALTER TABLE " + oldRosterNameString + " RENAME TO " + "Roster_" + newRosterNameString;

    sqlite3_stmt *statement;

    dbConnection = sqlite3_prepare_v2(database, renameSQLTable.c_str(), -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("rename_roster", database, dbConnection);
      return -3;
    }

    dbConnection = sqlite3_step(statement);
    if (dbConnection != SQLITE_DONE)
    {
      __throw_error_statement_step("rename_roster", database, dbConnection, statement);
      return -4;
    }
    sqlite3_finalize(statement);
    sqlite3_close(database);

    return 0;
  }

  /************************************************************************************
   * add_col_to_roster(): Adds the passed in col name to the passed in roster
   * Note: See function usage in  _manage_roster.c
   ************************************************************************************/
  // todo marshall you are working on checking existing column types. you made a function called int check_col_type below. you need to use this func here as an example...
  int add_col_to_roster(const char *rosterName, const char *colName, const char *colType)
  {
    string rosterNameString(rosterName);
    sqlite3 *database;
    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("add_col_to_roster", database, dbConnection);
      return -1;
    }
    string addColToSQLTable = "ALTER TABLE " + rosterNameString + " ADD " + colName + " " + colType;
    const char *addColToSQLTableChar = addColToSQLTable.c_str();

    sqlite3_stmt *statement;
    dbConnection = sqlite3_prepare_v2(database, addColToSQLTableChar, -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("add_col_to_roster", database, dbConnection);
      return -3;
    }

    dbConnection = sqlite3_step(statement);
    if (dbConnection != SQLITE_DONE)
    {
      __throw_error_statement_step("add_col_to_roster", database, dbConnection, statement);
      return -4;
    }

    sqlite3_finalize(statement);
    sqlite3_close(database);
    return 0;
  }

  /************************************************************************************
   * delete_col_from_roster(): Deletes the passed in col name to the passed in roster
   * Note: See function usage in  _manage_roster.c
   ************************************************************************************/
  int delete_col_from_roster(const char *rosterName, const char *colName)
  {
    string rosterNameString(rosterName);
    sqlite3 *database;
    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("", database, dbConnection);
      return -1;
    }

    string deleteColFromSQLTable = "ALTER TABLE " + rosterNameString + " DROP COLUMN " + colName;
    const char *deleteColFromSQLTableChar = deleteColFromSQLTable.c_str();

    sqlite3_stmt *statement;
    dbConnection = sqlite3_prepare_v2(database, deleteColFromSQLTableChar, -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {

      __throw_error_prepare_statement("delete_col_from_roster", database, dbConnection);
      return -3;
    }

    dbConnection = sqlite3_step(statement);
    if (dbConnection != SQLITE_DONE)
    {
      __throw_error_statement_step("delete_col_from_roster", database, dbConnection, statement);
      return -4;
    }

    sqlite3_finalize(statement);
    sqlite3_close(database);
    return 0;
  }

  /************************************************************************************
   * check_if_col_exists(): Checks if the passed in col name exists in the passed
   *                        in roster.
   * Note: See function usage in  _manage_roster.c
   ************************************************************************************/
  // man idk. Shout out to ChatGPT for this function...What the fuck
  int check_if_col_exists(const char *rosterName, const char *colName)
  {
    string rosterNameString(rosterName);

    sqlite3 *database;

    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("check_if_col_exists", database, dbConnection);
      return -1;
    }

    string checkIfColExistsInSQLTable = "PRAGMA table_info(" + rosterNameString + ");";
    const char *checkIfColExistsInSQLTableChar = checkIfColExistsInSQLTable.c_str();

    sqlite3_stmt *statement;

    dbConnection = sqlite3_prepare_v2(database, checkIfColExistsInSQLTableChar, -1, &statement, nullptr);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("check_if_col_exists", database, dbConnection);
      return -3;
    }

    int colExists = FALSE;

    while (sqlite3_step(statement) == SQLITE_ROW)
    {
      // Dont ask me what the fuck is going on here.
      const char *columnName = reinterpret_cast<const char *>(sqlite3_column_text(statement, 1));
      if (strcmp(columnName, colName) == 0)
      {
        colExists = TRUE;
        break; // break out of the loop if the column is found
      }
    }
    sqlite3_finalize(statement);
    sqlite3_close(database);

    /* The ternary operator threw me off a bit.
      while debugging I noticed that the value of colExists
      was always 0. I had true and false in the wrong order.
      - Marshall Burns

      returns 1 if the column exists, 0 if it does not
   */
    return colExists ? TRUE : FALSE;
  }

  /************************************************************************************
   * check_col_type(): Checks the type of the passed in column
   * Note: See function usage in  _manage_roster.c
   ************************************************************************************/
  int check_col_type(const char *rosterName, const char *colName)
  {
    string rosterNameString(rosterName);
    string colNameString(colName);

    sqlite3 *database;
    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("check_col_type", database, dbConnection);
      return -1;
    }
    string checkForSQLColType = "PRAGMA table_info(" + rosterNameString + ")";
    const char *checkForSQLColTypeChar = checkForSQLColType.c_str();

    sqlite3_stmt *statement;

    dbConnection = sqlite3_prepare_v2(database, checkForSQLColTypeChar, -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("check_col_type", database, dbConnection);
      return -3;
    }

    // Very hacky. ChatGPT really saved my ass. I have no idea what is going on until the strcmps
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
      const char *columnName = reinterpret_cast<const char *>(sqlite3_column_text(statement, 1));

      if (columnName && colNameString == columnName)
      {
        const char *colType = reinterpret_cast<const char *>(sqlite3_column_text(statement, 2));
        if (strcmp(colType, "TEXT") == 0)
        {
          return 5;
        }
        else if (strcmp(colType, "INTEGER") == 0)
        {
          return 6;
        }
        else if (strcmp(colType, "REAL") == 0)
        {
          return 7;
        }
        else if (strcmp(colType, "BOOLEAN") == 0)
        {
          return 8;
        }
        else if (strcmp(colType, "DATE") == 0)
        {
          return 9;
        }
        break;
      }
    }

    sqlite3_finalize(statement);
    sqlite3_close(database);
  }

  /************************************************************************************
   * show_all_roster_data(): Prints all data from the passed in roster
   * Note: See function usage in  _manage_roster.c
   ************************************************************************************/
  int show_all_roster_data(const char *rosterName)
  {
    string rosterNameString(rosterName);
    sqlite3 *database;

    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("show_all_roster_data", database, dbConnection);
      return -1;
    }

    string getAllDataSQL = "SELECT * FROM " + rosterNameString;
    const char *allDataSQL = getAllDataSQL.c_str();

    if (sqlite3_exec(database, allDataSQL, print_all_student_data_in_roster_callback, 0, 0) != SQLITE_OK)
    {
      cerr << "Error executing SQL statement: " << sqlite3_errmsg(database) << endl;
      sqlite3_close(database);
      return -1;
    }

    cout << endl;
    sqlite3_close(database);
    return 0;
  }

  /************************************************************************************
   * sort_roster_col_ascending(): Sorts the passed in roster by the passed in column
   *                              in ascending order
   *
   * Note: See function usage in  _manage_roster.c
   * Note: When using this to sort BOOLEANS, this will sort the FALSE values first
   ************************************************************************************/
  int sort_roster_col_ascending(const char *rosterName, const char *colName)
  {
    string rosterNameString(rosterName);
    string colNameString(colName);
    sqlite3 *database;

    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("sort_roster_col_ascending", database, dbConnection);
      return -1;
    }
    string sortSQLColumnAscending = "SELECT * FROM " + rosterNameString + " ORDER BY " + colNameString + " ASC";

    const char *sortSQL = sortSQLColumnAscending.c_str();

    if (sqlite3_exec(database, sortSQL, print_all_student_data_in_roster_callback, 0, 0) != SQLITE_OK)
    {
      __throw_error_exec_query("sort_roster_col_ascending", database, dbConnection);
      return -2;
    }

    cout << endl;
    sqlite3_close(database);
    return 0;
  }

  /************************************************************************************
   * sort_roster_col_descending(): Sorts the passed in roster by the passed in column
   *                               in descending order
   *
   * Note: See function usage in  _manage_roster.c
   * Note: When using this to sort BOOLEANS, this will sort the TRUE values first
   ************************************************************************************/
  int sort_roster_col_descending(const char *rosterName, const char *colName)
  {
    string rosterNameString(rosterName);
    string colNameString(colName);
    sqlite3 *database;

    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("sort_roster_col_descending", database, dbConnection);
      return -1;
    }
    string sortSQLColumnDescending = "SELECT * FROM " + rosterNameString + " ORDER BY " + colNameString + " DESC";

    const char *sortSQL = sortSQLColumnDescending.c_str();

    if (sqlite3_exec(database, sortSQL, print_all_student_data_in_roster_callback, 0, 0) != SQLITE_OK)
    {
      __throw_error_exec_query("sort_roster_col_descending", database, dbConnection);
      return -2;
    }

    cout << endl;
    sqlite3_close(database);
    return 0;
  }

  /************************************************************************************
   * drop_table(): Drops a roster table from the db.sqlite database
   * Note: See function usage in  _manage_roster.c
   ************************************************************************************/
  int drop_table(const char *tableName)
  {
    string tableNameString(tableName);
    sqlite3 *database;
    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("drop_table", database, dbConnection);
      return -1;
    }

    string dropSQLTable = "DROP TABLE " + tableNameString;

    sqlite3_stmt *statement;
    dbConnection = sqlite3_prepare_v2(database, dropSQLTable.c_str(), -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("drop_table", database, dbConnection);
      return -3;
    }

    dbConnection = sqlite3_step(statement);

    if (dbConnection != SQLITE_DONE)
    {
      __throw_error_statement_step("drop_table", database, dbConnection, statement);
      return -4;
    }

    sqlite3_finalize(statement);
    sqlite3_close(database);
    return 0;
  }
  /************************************************************************************
  * get_table_count(): This helper function returns 1 or 0 depending on if any tables
                       were found in the db.sqlite database
  * Note: See function usage in  _create_roster & _manage_roster.c
  ************************************************************************************/
  int get_table_count(const char *path)
  {
    sqlite3 *database;
    int dbConnection = sqlite3_open(path, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("get_table_count", database, dbConnection);
      return -1;
    }

    const char *selectSQLTables = "SELECT name FROM sqlite_master WHERE type='table'";

    dbConnection = sqlite3_exec(database, selectSQLTables, print_table_names_callback, nullptr, nullptr);
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

    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("add_student_to_roster", database, dbConnection);
      return -1;
    }

    string addStudentToSQLTable = "INSERT INTO " + rosterNameString + " (FirstName, LastName, StudentID) VALUES (?, ?, ?)";
    const char *addStudentToSQLTableChar = addStudentToSQLTable.c_str();
    sqlite3_stmt *statement;

    dbConnection = sqlite3_prepare_v2(database, addStudentToSQLTableChar, -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      cpp_utils_error_logger("Can't prepare SQL statement", "add_student_to_roster", CppErrorLevel::CRITICAL);
      sqlite3_close(database);
      // TODO come back to this prepare statement and fix error handling should not return -1
      return -1;
    }

    sqlite3_bind_text(statement, 1, firstName, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, lastName, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, studentID, -1, SQLITE_STATIC);

    dbConnection = sqlite3_step(statement);

    if (dbConnection != SQLITE_DONE)
    {
      __throw_error_statement_step("add_student_to_roster", database, dbConnection, statement);
      return -4;
    }
    sqlite3_finalize(statement);
    sqlite3_close(database);

    return TRUE; // Success...this just makes more sense to me than returning 0. The frontend wants 1
  }

  /************************************************************************************
   *  delete_student_from_table(): Useful for deleting a student from the student db
   *                               or a roster using the studentID
   *
   * Note: See function usage in _manage_student_db.c & _manage_roster.c
   ************************************************************************************/
  int delete_student_from_table(const char *studentID, const char *tableName)
  {
    string studentIDString(studentID);
    string tableNameString(tableName);

    sqlite3 *database;
    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("delete_student_from_table", database, dbConnection);
      return -1;
    }

    string deleteStudentFromSQLTable = "DELETE FROM " + tableNameString + " WHERE StudentID = '" + studentIDString + "'";

    sqlite3_stmt *statement;
    dbConnection = sqlite3_prepare_v2(database, deleteStudentFromSQLTable.c_str(), -1, &statement, nullptr);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("delete_student_from_table", database, dbConnection);
      return -3;
    }

    // Execute the statement
    dbConnection = sqlite3_step(statement);

    if (dbConnection != SQLITE_DONE)
    {
      __throw_error_statement_step("delete_student_from_table", database, dbConnection, statement);
      return -4;
    }

    sqlite3_finalize(statement);
    sqlite3_close(database);

    return 0;
  }

  /************************************************************************************
   *  check_if_student_id_exists(): Checks if the passed in studentID exists in the
   *                                 student db or a roster
   *
   * Note: See function usage in _manage_student_db.c & _manage_roster.c
   ************************************************************************************/
  int check_if_student_id_exists(const char *ID, const char *tableName)
  {
    studentIDExists = FALSE;
    string cppString(ID);
    string tableNameString(tableName);
    sqlite3 *database;

    // this should allow me to use this function for both the student db and the roster db
    if (globalTrigger.isTriggered == TRUE)
    {
      tableNameString = "students";
    }

    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      sqlite3_close(database);
      cerr << "Failed to open SQLite3 database." << endl;
      return 1;
    }

    string SQLStudentIDExistsCheck = "SELECT * FROM " + tableNameString + " WHERE StudentID  ='" + cppString + "'";
    dbConnection = sqlite3_exec(database, SQLStudentIDExistsCheck.c_str(), student_id_exists_callback, &studentIDExists, nullptr);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_exec_query("check_if_student_id_exists", database, dbConnection);
      return -2;
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

  /******************************!EVERYTHING BELOW HERE IS FOR THE STUDENTS DATABASE!******************************/
  /******************************!EVERYTHING BELOW HERE IS FOR THE STUDENTS DATABASE!******************************/
  /******************************!EVERYTHING BELOW HERE IS FOR THE STUDENTS DATABASE!******************************/
  /******************************!EVERYTHING BELOW HERE IS FOR THE STUDENTS DATABASE!******************************/

  /************************************************************************************
   *  create_student_db_and_table(): Begins the process of adding a student to the
   *                                 db.sqlite database
   * Note: See function usage in  _add_student.c
   ************************************************************************************/
  int create_student_db_and_table()
  {

    sqlite3 *database;
    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("create_student_db_and_table", database, dbConnection);
      return -1;
    }
    const char *addStudentToSQLTable = "CREATE TABLE IF NOT EXISTS students (id INTEGER PRIMARY KEY AUTOINCREMENT, FirstName, LastName,StudentID INTEGER)";

    sqlite3_exec(database, addStudentToSQLTable, nullptr, nullptr, nullptr);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_exec_query("create_student_db_and_table", database, dbConnection);
      return -2;
    }

    sqlite3_close(database);
    return 0;
  }

  /************************************************************************************
   * insert_student_into_db(): Inserts a student into the db.sqlite database using the
   *                           data from the student struct.
   * Note: See function usage in  _add_student.c
   ************************************************************************************/
  int insert_student_into_db(const char *FirsName, const char *LastName, const char *StudentID)
  {
    sqlite3 *database;
    int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("insert_student_into_db", database, dbConnection);
      return -1;
    }

    const char *insertStudentToSQLTable = "INSERT INTO students (FirstName, LastName, StudentID) VALUES (?, ?, ?)";

    sqlite3_stmt *statement;

    dbConnection = sqlite3_prepare_v2(database, insertStudentToSQLTable, -1, &statement, nullptr);
    if (dbConnection != SQLITE_OK)
    {
      __throw_error_prepare_statement("insert_student_into_db", database, dbConnection);
      return -3;
    }

    // Bind the value
    sqlite3_bind_text(statement, 1, FirsName, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, LastName, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, StudentID, -1, SQLITE_STATIC);

    dbConnection = sqlite3_step(statement);
    if (dbConnection != SQLITE_DONE)
    {
      __throw_error_statement_step("insert_student_into_db", database, dbConnection, statement);
      return -4;
    }
    return 0;
  }

  /************************************************************************************
   * show_student_list(): Prints all students currently in the db.sqlite database
   * Note: See function usage in _add_student.c & _manage_student.c
   ************************************************************************************/
  int show_all_students_in_student_db(const char *path)
  {
    sqlite3 *database;
    int dbConnection = sqlite3_open(path, &database);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_opening_db("show_all_students_in_student_db", database, dbConnection);
      return -1;
    }
    print_student_list_heading();
    // Construct the SQL query to select all rows from the specified table
    string selectFromStudentSQLTable = "SELECT FirstName, LastName, StudentID FROM students";

    // Execute the query and invoke the callback function to print each row
    dbConnection = sqlite3_exec(database, selectFromStudentSQLTable.c_str(), print_all_students_in_student_db_callback, nullptr, nullptr);

    if (dbConnection != SQLITE_OK)
    {
      __throw_error_exec_query("show_all_students_in_student_db", database, dbConnection);
      cout << YELLOW << "It is possible that there are no students in the student database." << RESET << endl;
      cout << "Try adding a student to the student database first then try again." << endl;
      return -2;
    }

    sqlite3_close(database);
    return 0;
  }
}

/************************************************************************************
 *  query_student_db(): Takes in a string from the frontend and queries
 *                                      the db.sqlite database for a match
 * Note: See function usage in  _manage_student.c & _search.c
 ************************************************************************************/
int query_student_db(const char *searchParam)
{
  sqlite3 *database;
  sqlite3_stmt *statement;
  int dbConnection = sqlite3_open(programSettings.databaseInfo.dbPath, &database);

  if (dbConnection != SQLITE_OK)
  {
    __throw_error_opening_db("query_student_db", database, dbConnection);
    return -1;
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
    __throw_error_prepare_statement("query_student_db", database, dbConnection);
    return -3;
  }
  sqlite3_close(database);
  return 0;
}
