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
#include "../lib/headers/utils.h"
#include "../lib/headers/db.hpp"
#include "../lib/headers/c_files.h"

static char buffer[50];
int searchingForStudent = TRUE;

// TODO add desc for func
int search_for_student()
{

  char buffer[50];
  puts("Search for a student by name or ID...");
  printf("This is not case sensitive");
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
  buffer[0] = toupper(buffer[0]);
  int result = query_db_for_student_name(buffer);

  if (result == 0)
  {
    puts(GREEN "Successfully queried student database." RESET);
  }
  else
  {
    printf(RED "Unable to query student data base for search:" BOLD "%s" RESET RED ".Please try again\n" RESET);
    UTILS_ERROR_LOGGER("Error occurred when trying to query student data base", "search_for_student", CRITICAL);
  }
}