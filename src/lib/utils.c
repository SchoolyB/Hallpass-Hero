/*
===============================================================================
File Name   : utils.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains several utility functions that are used
              throughout the 'C' portion of the source code. This file also
              contains the typedefs for the structs used in the program.
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "headers/c/c_files.h"
#include "headers/cpp/cpp_files.hpp"
/************************************************************************************
 * __utils_error_logger(): Logs errors errors that occur specifically in 'C' files
 * Note: For 'C++' code error logging see cpp_utils_error_logger in utils.cpp
 ************************************************************************************/
int __utils_error_logger(char *errorMessage, char *function, enum ErrorLevel level)
{

  FILE *errorLog = fopen("../logs/errors.log", "a");
  time_t currentTime;
  time(&currentTime);

  switch (level)
  {
  case MINOR:
    fprintf(errorLog, "Logged @ %s", ctime(&currentTime));
    fprintf(errorLog, "Minor Error: %s, in function call: %s()\n", errorMessage, function);
    fprintf(errorLog, "======================================================================================\n");
    fflush(errorLog);
    return 0;
    break;
  case MODERATE:
    fprintf(errorLog, "Logged @ %s", ctime(&currentTime));
    fprintf(errorLog, "Moderate Error: %s, in function call: %s()\n", errorMessage, function);
    fprintf(errorLog, "======================================================================================\n");
    fflush(errorLog);
    return 1;
  case CRITICAL:
    fprintf(errorLog, "Logged @ %s", ctime(&currentTime));
    fprintf(errorLog, "CRITICAL ERROR: %s, in function call: %s()\n", errorMessage, function);
    fprintf(errorLog, "======================================================================================\n");
    printf(RED "Critical Error occurred @ %s: For more information see logs/errors.log \n" RESET, ctime(&currentTime));
    fflush(errorLog);
    exit(1);
  default:
    break;
  }
}
/************************************************************************************
 * __utils_runtime_logger(): Logs runtime activity
 *
 ************************************************************************************/
int __utils_runtime_logger(char *action, char *functionName)
{
  if (programSettings.runtimeLoggingEnabled == FALSE)
  {
    return 0;
  }

  FILE *runtimeLog = fopen("../logs/runtime.log", "a");
  time_t currentTime;
  time(&currentTime);

  fprintf(runtimeLog, "Logged @ %s", ctime(&currentTime));

  fprintf(runtimeLog, "User Action: User %s, in function call: %s()\n", action, functionName);

  fprintf(runtimeLog, "======================================================================================\n\n");
  fflush(runtimeLog);
}

/************************************************************************************
 * __utils_operation_cancelled(): Logs when an operation is cancelled and displays
 *                               a message to the user.
 *
 ************************************************************************************/
void __utils_operation_cancelled(const char *functionName)
{
  system("clear");
  printf("%sCancelling operation.%s\n", yellow.colorCode, reset.colorCode);
  __utils_runtime_logger("Cancelled operation", functionName);
  sleep(1);
  system("clear");
}

/************************************************************************************
 * __utils_check_for_sqlite_db(): Checks if a sqlite database exists in the build
 *                                folder. If it does not then it creates the default
 *                                db.sqlite file.
 *
 * See usage in ALL C source files and the main.c file.
 ************************************************************************************/
int __utils_check_for_sqlite_db(void)
{
  int databaseFound = read_from_dir_and_check_extension("../build", ".sqlite");
  if (databaseFound == FALSE)
  {
    __utils_error_logger("Could not find database", "check_for_sqlite_db", MINOR);
    __utils_runtime_logger("Creating database", "check_for_sqlite_db");
    create_student_db_and_table();
  }
  return 0;
}

/************************************************************************************
 * __utils_check_for_bulk_loader_data_file(): Checks if a bulk data loader file exists
 *                                            in the build/data directory. If it does not
 *                                            then it creates the default active_data.json
 *                                            file.
 *
 * See usage in main.c
 ************************************************************************************/
int __utils_check_for_bulk_loader_data_file(void)
{
  int dataFileFound = read_from_dir_and_check_extension("../build/data", ".json");

  if (dataFileFound == FALSE)
  {
    __utils_error_logger("Could not find bulk data loader directory", "check_for_bulk_loader_data_file", MINOR);
    __utils_runtime_logger("Creating bulk data loader directory", "check_for_bulk_loader_data_file");
    jsonDataFile.fileNumIota = 0;
    generate_bulk_data_loader_file("data.json", "active");
    return FALSE;
  }

  else if (dataFileFound == TRUE)
  {
    __utils_runtime_logger("Bulk data loader file found", "check_for_bulk_loader_data_file");
    return TRUE;
  }
}

/************************************************************************************
 * read_from_dir_and_check_extension(): Reads from the directory and checks if the
 *                                      file has the specified extension.
 *
 * See usage in: handle_rename_db_logic() & main.c
 ************************************************************************************/
int read_from_dir_and_check_extension(const char *directoryPath, const char *extension)
{
  DIR *dir;
  struct dirent *entry;
  struct stat statbuf;

  dir = opendir(directoryPath);

  if (dir == NULL)
  {
    return -1;
  }

  // Iterate through the directory entries
  while ((entry = readdir(dir)) != NULL)
  {
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s/%s", directoryPath, entry->d_name);

    if (stat(filePath, &statbuf) == -1)
    {
      closedir(dir);
      return -1;
    }

    if (S_ISREG(statbuf.st_mode) && strstr(entry->d_name, extension) != NULL)
    {
      if (globalTrigger.isUsingBulkLoader == FALSE)
      {
        // storing the string value of the found file and file name in the programSettings struct
        strcpy(programSettings.databaseInfo.currentDBName, entry->d_name);
        return 1;
      }
      else if (globalTrigger.isUsingBulkLoader == TRUE)
      {
        // storing the string value of the found file and file path in the jsonDataFile struct whenever the function is called
        strcpy(jsonDataFile.FileName, entry->d_name);
        strcpy(jsonDataFile.FilePath, filePath);
        return 1;
      }
    }
  }

  closedir(dir);
  return 0;
}

/************************************************************************************
 * search_for_prefix_in_file_name(): Searches for the passed in word in the file name
 *                                   could technically be used to search for any string
 *                                   in a file name.
 *
 * See usage in: handle_rename_db_logic()
 ************************************************************************************/
int search_for_prefix_in_file_name(const char *targetWord)
{
  DIR *dir = opendir("../build/data");

  if (dir == NULL)
  {
    perror("Error opening directory");
    return 1;
  }
  struct dirent *entry;

  while ((entry = readdir(dir)) != NULL)
  {
    if (strstr(entry->d_name, targetWord) != NULL)
    {
      char fileName[256];
      char filePath[256];
      // copy the name of the entry into the fileName variable
      strcpy(fileName, entry->d_name);
      // store the value of the fileName and filePath variables in the jsonDataFile struct
      sprintf(jsonDataFile.FileName, "%s", fileName);
      sprintf(jsonDataFile.FilePath, "../build/data/%s", fileName);
      break;
    }
  }

  closedir(dir);

  return 0;
}

/************************************************************************************
 * show_current_menu(): Simply shows the current menu the user is in to prevent confusion
 ************************************************************************************/
void show_current_menu(char *str)
{
  printf("Current Menu: " BOLD "%s" RESET "\n", str);
}

/************************************************************************************
 * show_current_step(): When a feature requires multiple steps use this.
 *  Example: Step 1 of 2 name roster. Step 2 of 2 confirm name of roster.
 ************************************************************************************/
void show_current_step(char *str, int currentStep, int totalSteps)
{
  printf(BOLD "%s" RESET " Step: %d/%d\n", str, currentStep, totalSteps);
  printf("--------------------------------------------------\n");
}

void __utils_remove_newline_char(char *param)
{
  size_t len = strlen(param);
  if (len > 0 && param[len - 1] == '\n')
  {
    param[len - 1] = '\0';
  }
}

/************************************************************************************
 * __utils_clear_input_buffer(): Clears the input buffer to prevent unwanted behavior
 ************************************************************************************/
void __utils_clear_input_buffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
  {
    // Keep reading until a newline or EOF is encountered
  }
}

/************************************************************************************
 * list_all_students(): Lists all students in the student database
 ************************************************************************************/
int list_all_students(void)
{

  int result = show_all_students_in_student_db(databaseInfo.dbPath);
  if (result == 0)
  {
    int showingStudents = TRUE;
    while (showingStudents == TRUE)
    {
      puts("Press any key to continue...");
      getchar();
      showingStudents = FALSE;
      system("clear");
    }
  }
  else if (result == 1)
  {
    __utils_error_logger("Could not find students in database or table: 'students' does not exist", "list_all_students()", MODERATE);
    puts(RED "Error: Could not find students in database or table: 'students' does not exist " RESET);
    puts("May need to add a student to the database then try this action again.");
    wait_for_char_input();
  }

  return 0;
}

/************************************************************************************
 * check_for_special_chars(): Checks if the passed in string has any special
 *                            characters. If it does it returns TRUE, else FALSE
 ************************************************************************************/
int check_for_special_chars(const char *str)
{
  while (*str)
  {
    if (*str == '!' || *str == '@' || *str == '#' || *str == '$' || *str == '%' || *str == '^' || *str == '&' || *str == '*' || *str == '(' || *str == ')' || *str == '-' || *str == '_' || *str == '=' || *str == '+' || *str == '[' || *str == ']' || *str == '{' || *str == '}' || *str == '|' || *str == ';' || *str == ':' || *str == '"' || *str == '\'' || *str == '<' || *str == '>' || *str == ',' || *str == '.' || *str == '/' || *str == '?' || *str == '`' || *str == '~')
    {
      return TRUE; // Special character found
    }
    str++;
  }
  return FALSE; // No special characters found
}

/************************************************************************************
 * has_one_non_space_char(): Helper function that checks if a string has at least
 * one non-space character.
 ************************************************************************************/
int has_one_non_space_char(const char *str)
{
  while (*str)
  {
    if (*str != ' ')
    {
      return TRUE; // Non-space character found
    }
    str++;
  }
  return FALSE; // Only spaces found
}

/************************************************************************************
 * wait_for_char_input(): Simple function that waits for the user to press any key
 ************************************************************************************/
int wait_for_char_input(void)
{
  puts("Press any key to continue...");
  getchar();
  system("clear");
}

int hash_data(char *data)
{
  int hash = 0;
  int c;

  while ((c = *data++))
  {
    hash += c;
  }

  return hash;
}

int read_and_display_help_docs(const char *helpFile)
{
  FILE *f = fopen(helpFile, "r");
  if (f == NULL)
  {
    __utils_error_logger("Could not open help file", "read_and_display_help_docs", MODERATE);
    printf("%sError: Could not open help file: %s\n", red.colorCode, reset.colorCode);
    printf("Please try again.\n");
    perror("fopen");
    wait_for_char_input(); // todo remove this
    return 1;
  }

  char c;
  while ((c = fgetc(f)) != EOF)
  {
    putchar(c);
  }
  fclose(f);
  wait_for_char_input();
}
