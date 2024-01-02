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

/*
Allows using elements from
the 'std' namespace without
prefixing them all with ''
*/
using namespace std;
int hasTables = FALSE;
int studentIDExists = FALSE;

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
  sqlite3 *db;
  int rc = sqlite3_open("../build/db.sqlite", &db);

  if (rc != SQLITE_OK)
  {
    sqlite3_close(db);
    cerr << "Failed to open SQLite3 database." << endl;
    return -1;
  }

  string SQLTableNameExistsCheck = "SELECT name FROM sqlite_master WHERE type ='table' AND name ='" + cppString + "'";
  rc = sqlite3_exec(db, SQLTableNameExistsCheck.c_str(), table_exists_callback, &hasTables, nullptr);

  if (rc != SQLITE_OK)
  {
    cerr << "SQLite error: " << sqlite3_errmsg(db) << endl;
    sqlite3_close(db);
    return -1;
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
int refresh_table_count(void)
{
  hasTables = FALSE;
  return 0;
}
/*Simple helper that prints a heading when showing the student list*/
int print_student_list_heading()
{
  printf("%-15s %-15s %-15s\n", "First Name", "Last Name", "Student ID");
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
    string cppString(rosterName);

    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database", "create_new_roster_table", CppErrorLevel::CRITICAL);
      return 1;
    }

    // Manually concatenate the table name into the SQL string
    string createSQLTable = "CREATE TABLE IF NOT EXISTS " + cppString + " (id INTEGER PRIMARY KEY AUTOINCREMENT, FirstName TEXT, LastName TEXT, StudentID TEXT)";
    const char *createSQLTableChar = createSQLTable.c_str();

    sqlite3_stmt *statement;

    // Compile the SQL statement into byte-code
    rc = sqlite3_prepare_v2(db, createSQLTableChar, -1, &statement, nullptr);
    if (rc != SQLITE_OK)
    {
      CPP_UTILS_ERROR_LOGGER("Can't prepare SQL statement", "create_new_roster_table", CppErrorLevel::CRITICAL);
      sqlite3_close(db);
      return 1;
    }

    // Execute the statement
    rc = sqlite3_step(statement);
    if (rc != SQLITE_DONE)
    {
      CPP_UTILS_ERROR_LOGGER("Error executing SQL statement", "create_new_roster_table", CppErrorLevel::CRITICAL);
      sqlite3_finalize(statement);
      sqlite3_close(db);
      return 1;
    }

    // Finalize the statement
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return 0;
  }

  /************************************************************************************
   * show_tables(): Prints all rosters currently in the db.sqlite database
   * Note: See function usage in ../src/_create_roster.c & ../src/_manage_roster.c
   ************************************************************************************/
  int show_tables()
  {
    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      std::cerr << "Failed to open SQLite3 database" << std::endl;
      return -1;
    }

    const char *selectSQLTables = "SELECT name FROM sqlite_master WHERE type='table' AND name LIKE 'Roster%'";

    // Print the header
    cout << "==========================================================================================" << endl;
    cout << BOLD << "Created rosters:" << RESET << endl;
    cout << "------------------------------------------------------------------------------------------" << endl;
    rc = sqlite3_exec(db, selectSQLTables, callback, nullptr, nullptr);

    if (rc != SQLITE_OK)
    {
      std::cerr << "Failed to execute query" << std::endl;
    }

    cout << "==========================================================================================" << endl;

    sqlite3_close(db);

    // If you want to get the count of tables, you can call your get_table_count function here
    // int tableCountResult = get_table_count("../build/db.sqlite");
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
    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);
    if (rc != SQLITE_OK)
    {
      cerr << "Failed to open SQLite3 database" << endl;
      return 1;
    }
    // Manually concatenate the table name into the SQL string also adds the Roster_ prefix to the new table name
    string renameSQLTable = "ALTER TABLE " + oldRosterNameString + " RENAME TO " + "Roster_" + newRosterNameString;

    sqlite3_stmt *statement;
    rc = sqlite3_prepare_v2(db, renameSQLTable.c_str(), -1, &statement, nullptr);
    if (rc != SQLITE_OK)
    {
      cerr << "Can't prepare SQL statement: " << sqlite3_errmsg(db) << endl;
      sqlite3_close(db);
      return 1;
    }

    rc = sqlite3_step(statement);
    if (rc != SQLITE_DONE)
    {
      cerr << "Error executing SQL statement: " << sqlite3_errmsg(db) << endl;
      sqlite3_finalize(statement);
      sqlite3_close(db);
      return 1;
    }

    sqlite3_finalize(statement);
    sqlite3_close(db);

    return 0;
  }

  /************************************************************************************
   * drop_table(): Drops a roster table from the db.sqlite database
   * Note: See function usage in  ../src/_manage_roster.c
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
      CPP_UTILS_SLEEP(1);
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
        cout << YELLOW "Deleting " << currentTableName << RESET << endl;
        string deleteSQLTable = "DROP TABLE " + currentTableName;

        rc = sqlite3_exec(db, deleteSQLTable.c_str(), nullptr, nullptr, nullptr);
        system("clear");
        cout << GREEN << "The " BOLD GREEN << currentTableName + RESET GREEN " roster has successfully been deleted" RESET << endl;
        sqlite3_close(db);
        refresh_table_count();
        return 2;
      }
      else if (answer == "cancel")
      {
        system("clear");
        cout << YELLOW "Canceling roster deletion" RESET << endl;
        sqlite3_close(db);
        CPP_UTILS_SLEEP(1);
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
  * Note: See function usage in  ../src/_create_roster & ../src/_manage_roster.c
  ************************************************************************************/
  int get_table_count(const char *path)
  {
    sqlite3 *db;
    int rc = sqlite3_open(path, &db);

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

    rc = sqlite3_exec(db, selectSQLTables, callback, nullptr, nullptr);
    sqlite3_close(db);

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

    sqlite3 *db;

    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database. ", "add_student_to_roster", CppErrorLevel::CRITICAL);
      cerr << RED "CRITICAL ERROR: Failed to find/open database" RESET << endl;
      cout << "Exiting program" << endl;
      sleep(1);
      exit(1);
    }

    string addStudentToSQLTable = "INSERT INTO " + rosterNameString + " (FirstName, LastName, StudentID) VALUES (?, ?, ?)";
    const char *addStudentToSQLTableChar = addStudentToSQLTable.c_str();
    sqlite3_stmt *statement;

    rc = sqlite3_prepare_v2(db, addStudentToSQLTableChar, -1, &statement, nullptr);
    if (rc != SQLITE_OK)
    {
      CPP_UTILS_ERROR_LOGGER("Can't prepare SQL statement", "add_student_to_roster", CppErrorLevel::CRITICAL);
      sqlite3_close(db);
      return -1;
    }

    sqlite3_bind_text(statement, 1, firstName, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, lastName, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, studentID, -1, SQLITE_STATIC);

    rc = sqlite3_step(statement);

    sqlite3_finalize(statement);
    sqlite3_close(db);

    if (rc != SQLITE_DONE)
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

    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      cerr << "Failed to open SQLite3 database." << endl;
      return 1;
    }

    string SQLStudentIDExistsCheck = "SELECT * FROM students WHERE StudentID  ='" + cppString + "'";
    rc = sqlite3_exec(db, SQLStudentIDExistsCheck.c_str(), student_id_exists_callback, &studentIDExists, nullptr);

    if (rc != SQLITE_OK)
    {
      cerr << "SQLite error: " << sqlite3_errmsg(db) << endl;
      sqlite3_close(db);
      return 1;
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

  int get_student_db_row_count(const char *path)
  {
    sqlite3 *db;
    int rc = sqlite3_open(path, &db);

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database. ", "get_student_db_row_count", CppErrorLevel::CRITICAL);
      cerr << RED "CRITICAL ERROR: Failed to find/open database" RESET << endl;
      cout << "Exiting program" << endl;
      sleep(1);
      exit(1);
    }
    return 0;
  }

  /************************************************************************************
   *  create_student_db_and_table();: Begins the process of adding a student to the db.sqlite database
   * Note: See function usage in  ../src/_add_student.c
   * Note: THis function relies on the following helper functions:
   ************************************************************************************/
  int create_student_db_and_table()
  {

    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database: ", "add_student_to_student_db", CppErrorLevel::CRITICAL);
      cerr << RED "Failed to open SQLite3 database" RESET << endl;
      return 1;
    }
    const char *addStudentToSQLTable = "CREATE TABLE IF NOT EXISTS students (id INTEGER PRIMARY KEY AUTOINCREMENT, FirstName, LastName,StudentID INTEGER)";

    sqlite3_exec(db, addStudentToSQLTable, nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      CPP_UTILS_ERROR_LOGGER("Failed to add student to database: ", "add_student_to_student_db", CppErrorLevel::CRITICAL);
      cerr << RED "Failed to add student to database" RESET << endl;
      return 1;
    }

    sqlite3_close(db);
    return 0;
  }

  /************************************************************************************
   *  insert_student_into_db();: Inserts a student into the db.sqlite database using the data from the student struct
   * Note: See function usage in  ../src/_add_student.c
   ************************************************************************************/
  int insert_student_into_db(const char *FirsName, const char *LastName, const char *StudentID)
  {
    sqlite3 *db;
    int rc = sqlite3_open("../build/db.sqlite", &db);

    if (rc != SQLITE_OK)
    {
      sqlite3_close(db);
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database: ", "insert_student_into_db", CppErrorLevel::CRITICAL);
      cerr << RED "Failed to open SQLite3 database" RESET << endl;
      return 1;
    }

    const char *insertStudentToSQLTable = "INSERT INTO students (FirstName, LastName, StudentID) VALUES (?, ?, ?)";

    sqlite3_stmt *statement;

    rc = sqlite3_prepare_v2(db, insertStudentToSQLTable, -1, &statement, nullptr);
    if (rc != SQLITE_OK)
    {
      CPP_UTILS_ERROR_LOGGER("Can't prepare SQL statement", "insert_student_into_db", CppErrorLevel::CRITICAL);
      sqlite3_close(db);
      return 1;
    }

    // Bind the value
    sqlite3_bind_text(statement, 1, FirsName, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, LastName, -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, StudentID, -1, SQLITE_STATIC);

    rc = sqlite3_step(statement);
    return 0;
  }

  /************************************************************************************
   * show_student_list(): Prints all students currently in the db.sqlite database
   * Note: See function usage in ../src/_add_student.c & ../src/_manage_student.c
   ************************************************************************************/
  int show_students_in_db(const char *path)
  {
    sqlite3 *db;
    int rc = sqlite3_open(path, &db);

    if (rc != SQLITE_OK)
    {
      cerr << RED "Failed to open SQLite3 database. Error: " RESET << sqlite3_errmsg(db) << endl;
      CPP_UTILS_ERROR_LOGGER("Failed to open SQLite3 database. ", "show_students_in_db", CppErrorLevel::CRITICAL);
      sqlite3_close(db);
      exit(1);
    }
    print_student_list_heading();
    // Construct the SQL query to select all rows from the specified table
    string selectFromStudentSQLTable = "SELECT FirstName, LastName, StudentID FROM students";

    // Execute the query and invoke the callback function to print each row
    rc = sqlite3_exec(db, selectFromStudentSQLTable.c_str(), print_student_info_callback, nullptr, nullptr);

    if (rc != SQLITE_OK)
    {
      return 1;
    }

    // Close the database
    sqlite3_close(db);
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
  sqlite3 *db;
  sqlite3_stmt *statement;
  int rc = sqlite3_open("../build/db.sqlite", &db);

  if (rc != SQLITE_OK)
  {
    cerr << "Failed to open SQLite3 database. Error: " << sqlite3_errmsg(db) << endl;
    sqlite3_close(db);
    return -1;
  }

  const char *sql = "SELECT * FROM students WHERE FirstName = ? OR LastName = ? OR StudentID = ?";

  if (sqlite3_prepare_v2(db, sql, -1, &statement, NULL) == SQLITE_OK)
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
    cerr << "Failed to prepare the statement: " << sqlite3_errmsg(db) << endl;
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);

  return 0;
}
