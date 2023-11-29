/*
===============================================================================
File Name   : db.hpp
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This header file contains all the declarations for the functions
              that interact with the database
-------------------------------------------------------------------------------
Notes       : This uses extern "C" linkage to allow the C++ functions to be
              called from C code.
===============================================================================
*/

#ifndef DB_HPP
#define DB_HPP

#include <sqlite3.h>

// Declaration of C++ functions with extern "C" linkage
#ifdef __cplusplus
extern "C"
{
#endif

  // C++ function declarations
  static int callback(void *NotUsed, int argc, char **argv, char **azColName);
  int print_table_names_callback(void *data, int argc, char **argv, char **azColName);
  int create_new_roster_table(const char *rosterName);
  int show_tables(void);
  int rename_roster(void);
  int get_table_count(const char *path);
  int drop_table(void);
  int create_student_db_and_table(void);
  int insert_student_into_db(const char *FirstName, const char *LastName, const char *StudentID);
  int show_students_in_db(const char *path, const char *tableName);
#ifdef __cplusplus
} // extern "C"
#endif /* DB_HPP */
#endif
