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
#include "../lib/headers/utils.h"
#include "../lib/headers/db.hpp"
#include "../lib/headers/c_files.h"

UserInput searchInput;
int searchingForStudent = TRUE;

// TODO add desc for func
int search_for_student(void)
{
  puts("Search for a student by name or ID...");
  printf("This is not case sensitive");
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(searchInput.StrInput);
  searchInput.StrInput[0] = toupper(searchInput.StrInput[0]);
  int result = query_db_for_student_name(searchInput.StrInput);

  if (result == 0)
  {
    puts(GREEN "Successfully queried student database." RESET);
  }
  else
  {
    printf(RED "Unable to query student data base for search:" BOLD "%s" RESET RED ".Please try again\n" RESET, searchInput.StrInput);
    UTILS_ERROR_LOGGER("Error occurred when trying to query student data base", "search_for_student", CRITICAL);
  }
  return 0;
}