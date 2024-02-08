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
#include "../lib/headers/c/c_files.h"

UserInput userInput = {FALSE};
GlobalTrigger globalTrigger = {FALSE};
ProgramSettings programSettings = {FALSE};
DatabaseInfo databaseInfo = {FALSE};
ThrowAways throwAways = {FALSE};
Roster roster = {FALSE};
JSONDataFile jsonDataFile = {FALSE};
Color red = {RED};
Color green = {GREEN};
Color yellow = {YELLOW};
Color reset = {RESET};

// default db path can be changed in settings menu
char dbPath[40];
uint8_t mainMenuProccess = FALSE;
int handle_main_menu(void);

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
  char dbPath[40] = "../build/db.sqlite";
  programSettings.databaseInfo.dbPath = dbPath;
  globalTrigger.isAddingToStudentsTable = TRUE;
  /*Runs a check on start up to see if a sqlite file is already found
  in the build folder. If a file is not then create the default db.sqlite file*/
  __utils_check_for_sqlite_db();
  /*This appends the current database name to the dbPath variable
  so on startup the db name is already set if it already exists
  in the build folder, and if it doesn't then it will be created
  and the db name will be set to the default db.sqlite
   */
  sprintf(dbPath, "../build/%s", programSettings.databaseInfo.currentDBName);

  // Creates the logs dir on startup if it doesn't already exist
  mkdir("../logs", 0777);

  mkdir("../build/data", 0777);
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
  fprintf(runtimeLogFile, "Settings config file created or found\n");
  fprintf(runtimeLogFile, "SQLite3 database created or found\n");
  fprintf(runtimeLogFile, "======================================================================================\n\n");
  fflush(runtimeLogFile);
  fclose(runtimeLogFile);

  int mainMenuProccess = TRUE;
  while (mainMenuProccess == TRUE)
  {
    handle_main_menu();
    /*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-START OF MAIN MENU+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

    /*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-END OF MAIN MENU+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
  }
  return 0;
}

int handle_main_menu(void)
{
  system("clear");
  printf("Welcome to the Hallpass Hero main menu!\n");
  printf("Please select one of the options below:\n");

  // showing main menu options
  printf("===========================================================================================\n");
  for (int i = 0; i < 8; ++i)
  {
    printf("| %s %-90s\n", MainMenuOptions[i], "");
  }

  /*Here we are handling the input that the user makes on the main menu*/
  __utils_fgets_and_remove_newline(userInput.StrInput);
  userInput.NumInput = atoi(userInput.StrInput);

  // check for the config file and load it
  check_and_load_config();

  // to create a new roster
  if (userInput.NumInput == 1 || strcmp(userInput.StrInput, "new roster") == 0)
  {
    __utils_runtime_logger("entered the create new roster menu", "main");
    system("clear");
    create_new_roster();
  }
  // to manage an existing roster
  else if (userInput.NumInput == 2 || strcmp(userInput.StrInput, "manage roster") == 0)
  {
    __utils_runtime_logger("entered the manage roster menu", "main");
    system("clear");
    show_manage_roster_menu();
  }
  // to add a student to the student database
  else if (userInput.NumInput == 3 || strcmp(userInput.StrInput, "add student") == 0)
  {
    __utils_runtime_logger("entered the add student menu", "main");
    system("clear");
    system("clear");
    add_student_to_student_table();
  }
  // to view and manage the student database
  else if (userInput.NumInput == 4 || strcmp(userInput.StrInput, "manage student database") == 0)
  {
    __utils_runtime_logger("entered the manage student database menu", "main");
    system("clear");
    manage_student_db();
  }
  // to search for a student
  else if (userInput.NumInput == 5 || strcmp(userInput.StrInput, "search student") == 0)
  {
    __utils_runtime_logger("entered the search student menu", "main");
    system("clear");
    // do stuff
  }
  else if (userInput.NumInput == 6 || strcmp(userInput.StrInput, "help") == 0)
  {
    __utils_runtime_logger("entered the main menu's help menu", "main");
    system("clear");
    read_and_display_help_docs("../src/help/_help_main_menu.txt");
    // do stuff
  }
  // to enter the settings menu
  else if (userInput.NumInput == 7 || strcmp(userInput.StrInput, "settings") == 0)
  {
    __utils_runtime_logger("entered the settings menu", "main");
    system("clear");
    show_settings_menu();
  }
  else if (userInput.NumInput == 8 || strcmp(userInput.StrInput, "exit") == 0)
  {
    system("clear");
    printf("See you soon!");
    __utils_runtime_logger("exited the program", "main");
    exit(0);
  }
  else
  {
    __utils_error_logger("Invalid decision made on while on main menu", "main", MINOR);
    printf("Sorry, I didn't understand that.");
    printf("Please try again");
    system("clear");
  }
}
