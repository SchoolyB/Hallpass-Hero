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
  int get_table_count(void);
  int drop_table(void);
  int create_student_db_and_table(void);

#ifdef __cplusplus
} // extern "C"
#endif /* DB_HPP */
#endif
