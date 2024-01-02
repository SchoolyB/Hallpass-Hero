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
  int table_exists_callback(void *exists, int argc, char **argv, char **columnNames);
  int student_id_exists_callback(void *data, int argc, char **argv, char **azColName);
  static int callback(void *NotUsed, int argc, char **argv, char **azColName);
  int print_table_names_callback(void *data, int argc, char **argv, char **azColName);
  int check_if_table_exists(const char *rosterName);
  int create_new_roster_table(const char *rosterName);
  int show_tables(void);
  int rename_roster(const char *oldRosterName, const char *rosterName);
  int get_table_count(const char *path);
  int drop_table(const char *tableName);
  int check_if_student_id_exists(const char *ID);
  int create_student_db_and_table(void);
  int insert_student_into_db(const char *FirstName, const char *LastName, const char *StudentID);
  int show_students_in_db(const char *path);
  int query_db_for_student_name(const char *searchParam);
  int add_student_to_roster(const char *rosterName, const char *FirstName, const char *LastName, const char *StudentID);
#ifdef __cplusplus
} // extern "C"
#endif /* DB_HPP */
#endif
