/*
===============================================================================
File Name   : _search.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains the function used
              to search for a student in the database.
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include "../lib/headers/cpp/cpp_files.hpp"
#include "../lib/headers/c/c_files.h"

int searchingForStudent = TRUE;

/************************************************************************************
 * search_for_student(): This function is used to search for a student in the database
 *
 ************************************************************************************/
int search_for_student(void)
{
  __utils_runtime_logger("entered search for student menu", "search_for_student");
  __utils_check_for_sqlite_db();
  printf("Search for a student by name or ID...\n");
  printf("This is not case sensitive\n");
  __utils_fgets_and_remove_newline(userInput.StrInput);
  userInput.StrInput[0] = toupper(userInput.StrInput[0]);
  int result = query_student_db(userInput.StrInput);

  if (result == 0)
  {
    printf("%sSuccessfully queried student database.%s\n", green.colorCode, reset.colorCode);
  }
  else
  {
    printf("%sUnable to query student data base for search:" BOLD "%s %s%s.Please try again\n", red.colorCode, userInput.StrInput, reset.colorCode, red.colorCode);
    __utils_error_logger("Error occurred when trying to query student data base", "search_for_student", CRITICAL);
  }
  return 0;
}