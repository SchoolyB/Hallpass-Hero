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
#include "../lib/headers/c_files.h"

const char *dbPath = "../build/db.sqlite";

GlobalTrigger globalTrigger = {FALSE};
ProgramSettings programSettings = {FALSE};

UserInput mainMenuInput;
uint8_t mainMenuProccess = FALSE;
char MainMenuOptions[8][50] = {
    "1. Create a new roster",
    "2. View and manage an existing roster",
    "3. Add a student to the student database",
    "4. View and manage the student database",
    "5. Search for a student",
    "6. Help",
    "7. Settings",
    "8. Exit"};

int main(void)
{

  time_t currentTime;
  time(&currentTime);

  programSettings.runtimeLoggingEnabled = TRUE;
  // Creates the student DB table on startup if it doesn't already exist
  create_student_db_and_table();
  // Creates the logs dir on startup if it doesn't already exist
  mkdir("../logs", 0777);

  // Create the errors.log file if it doesn't already exist
  FILE *errorLogFile = fopen("../logs/errors.log", "a");
  if (errorLogFile == NULL)
  {
    perror("Error creating errorLog file");
    return 1;
  }
  fclose(errorLogFile);

  // Create the runtime.log file if it doesn't already exist
  FILE *runtimeLogFile = fopen("../logs/runtime.log", "a");
  if (runtimeLogFile == NULL)
  {
    perror("Error creating runtimeLog file");
    return 1;
  }
  fprintf(runtimeLogFile, "Logged @ %s", ctime(&currentTime));
  fprintf(runtimeLogFile, "--------------------------------------------\n");
  fprintf(runtimeLogFile, "Log directory created or found\n");
  fprintf(runtimeLogFile, "Runtime logging has been enabled\n");
  fprintf(runtimeLogFile, "Error logging initialized\n");
  fprintf(runtimeLogFile, "SQLite3 database initialized\n");
  fprintf(runtimeLogFile, "======================================================================================\n\n");
  fflush(runtimeLogFile);
  fclose(runtimeLogFile);

  char buffer[50];
  int mainMenuProccess = TRUE;

  while (mainMenuProccess == TRUE)
  {
    /*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-START OF MAIN MENU+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
    system("clear");
    puts("Welcome to the Hallpass Hero main menu!");
    puts("Please select one of the options below:");

    // showing main menu options
    puts("===========================================================================================");
    for (int i = 0; i < 8; ++i)
    {
      printf("| %s %-90s\n", MainMenuOptions[i], "");
    }
    puts("===========================================================================================");
    /*Here we are handling the input that the user makes on the main menu*/
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
    mainMenuInput.NumInput = atoi(buffer);

    // to create a new roster
    if (mainMenuInput.NumInput == 1 || strcmp(buffer, "new roster") == 0)
    {
      UTILS_RUNTIME_LOGGER("User entered the create new roster menu", "main");
      system("clear");
      create_new_roster();
    }
    // to manage an existing roster
    else if (mainMenuInput.NumInput == 2 || strcmp(buffer, "manage roster") == 0)
    {
      UTILS_RUNTIME_LOGGER("User entered the manage roster menu", "main");
      system("clear");
      manage_roster();
    }
    // to add a student to the student database
    else if (mainMenuInput.NumInput == 3 || strcmp(buffer, "add student") == 0)
    {
      UTILS_RUNTIME_LOGGER("User entered the add student menu", "main");
      system("clear");
      system("clear");
      add_student_to_db();
    }
    // to view and manage the student database
    else if (mainMenuInput.NumInput == 4 || strcmp(buffer, "manage student database") == 0)
    {
      UTILS_RUNTIME_LOGGER("User entered the manage student database menu", "main");
      system("clear");
      manage_student_db();
    }
    // to search for a student
    else if (mainMenuInput.NumInput == 5 || strcmp(buffer, "search student") == 0)
    {
      UTILS_RUNTIME_LOGGER("User entered the search student menu", "main");
      system("clear");
      // do stuff
    }
    else if (mainMenuInput.NumInput == 6 || strcmp(buffer, "help") == 0)
    {
      UTILS_RUNTIME_LOGGER("User entered the main menu's help menu", "main");
      system("clear");
      // do stuff
    }
    else if (mainMenuInput.NumInput == 7 || strcmp(buffer, "settings") == 0)
    {
      UTILS_RUNTIME_LOGGER("User entered the settings menu", "main");
      system("clear");
      show_settings_menu();
    }
    else if (mainMenuInput.NumInput == 8 || strcmp(buffer, "exit") == 0)
    {
      system("clear");
      puts("See you soon!");
      UTILS_RUNTIME_LOGGER("User exited the program", "main");
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
