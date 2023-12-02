/*
===============================================================================
File Name   : main.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains the main function for the Hallpass Hero CLI tool
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../lib/headers/utils.h"
#include "../lib/headers/main.h"
#include "../lib/headers/db.hpp"
#include "../lib/headers/c_files.h"
#include "../lib/Cuazar/lib/Cuazar.h"

static uint8_t menuInput;
uint8_t mainMenuProccess = FALSE;
char MainMenuOptions[7][50] = {
    "1. Create a new roster",
    "2. View and manage an existing roster",
    "3. Add a student to the student database",
    "4. View and manage the student database",
    "5. Search for a student",
    "6. Help",
    "7. Exit"};

int main()
{

  Init_Cuazar(clear()); // Pass NULL to disable the clear terminal function
  if (mkdir("../logs", 0777) != 0)
  {
    UTILS_ERROR_LOGGER("Could not create logs directory as it already exists", "main", MINOR);
  }

  FILE *errorLogFile = fopen("../logs/errors.log", "a");
  if (errorLogFile == NULL)
  {
    perror("Error creating errorLog file");
    return 1;
  }
  fclose(errorLogFile);

  char buffer[50];
  int mainMenuProccess = TRUE;

  while (mainMenuProccess == TRUE)
  {
    /*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-START OF MAIN MENU+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
    puts("Welcome to the Hallpass Hero main menu!");
    puts("Please select one of the options below:");

    // showing main menu options
    puts("===========================================================================================");
    for (int i = 0; i < 7; ++i)
    {
      printf("| %s %-90s\n", MainMenuOptions[i], "");
    }
    puts("===========================================================================================");
    /*Here we are handling the input that the user makes on the main menu*/
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
    menuInput = atoi(buffer);

    // to create a new roster
    if (menuInput == 1 || strcmp(buffer, "new roster") == 0)
    {
      system("clear");
      create_new_roster();
    }
    // to manage an existing roster
    else if (menuInput == 2 || strcmp(buffer, "manage roster") == 0)
    {
      system("clear");
      manage_roster();
    }
    // to add a student to the student database
    else if (menuInput == 3 || strcmp(buffer, "add student") == 0)
    {
      system("clear");
      system("clear");
      add_student_to_db();
    }
    // to view and manage the student database
    else if (menuInput == 4 || strcmp(buffer, "manage student database") == 0)
    {
      system("clear");
      manage_student_db();
    }
    // to search for a student
    else if (menuInput == 5 || strcmp(buffer, "search student") == 0)
    {
      system("clear");
      // do stuff
    }
    else if (menuInput == 6 || strcmp(buffer, "help") == 0)
    {
      system("clear");
      // do stuff
    }
    else if (menuInput == 7 || strcmp(buffer, "exit") == 0)
    {
      system("clear");
      puts("See you soon!");
      mainMenuProccess = FALSE;
    }
    else
    {
      UTILS_ERROR_LOGGER("Invalid decision made on while on main menu", "main", MINOR);
      puts("Sorry, I didn't understand that.");
      puts("Please try again");
      system("clear");
    }

    /*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-END OF MAIN MENU+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
  }
  return 0;
}
