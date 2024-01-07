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

// Declaration of C++ functions with extern "C" linkage
#ifdef __cplusplus
#include <sqlite3.h>
#include <iostream>
int __throw_error_opening_db(std::string functionName, sqlite3 *database, int param);
int __throw_error_exec_query(std::string functionName, sqlite3 *database, int param);
int __throw_error_prepare_statement(std::string functionName, sqlite3 *database, int param);
int __throw_error_statement_step(std::string functionName, sqlite3 *database, int param, sqlite3_stmt);
extern "C"
{
#include <sqlite3.h>
#endif

  // C++ function declarations

  int table_exists_callback(void *exists, int argc, char **argv, char **columnNames);
  int student_id_exists_callback(void *data, int argc, char **argv, char **azColName);
  int print_table_names_callback(void *data, int argc, char **argv, char **azColName);
  int print_table_names_callback(void *data, int argc, char **argv, char **azColName);
  int show_table_data_callback(void *data, int argc, char **argv, char **azColName);
  static int callback(void *NotUsed, int argc, char **argv, char **azColName);
  int check_if_table_exists(const char *rosterName);
  int refresh_table_count(void);
  int print_student_list_heading(void);
  int create_new_roster_table(const char *rosterName);
  int show_tables(void);
  int rename_roster(const char *oldRosterName, const char *rosterName);
  int set_col_type(const char *rosterName, const char *colName, const char *colType);
  int rename_roster_column(const char *rosterName, const char *oldColName, const char *newColName);
  int add_col_to_roster(const char *rosterName, const char *colName, const char *colType);
  int delete_col_from_roster(const char *rosterName, const char *colName);
  int check_if_col_exists(const char *rosterName, const char *colName);
  int drop_table(const char *tableName);
  int get_table_count(const char *path);
  int add_student_to_roster(const char *rosterName, const char *FirstName, const char *LastName, const char *StudentID);
  int check_if_student_id_exists(const char *ID);
  int create_student_db_and_table(void);
  int insert_student_into_db(const char *FirstName, const char *LastName, const char *StudentID);
  int show_students_in_db(const char *path);
  int query_db_for_student_name(const char *searchParam);
#ifdef __cplusplus
} // extern "C"
#endif /* DB_HPP */
#endif
