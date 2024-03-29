/*
===============================================================================
File Name   : _bulk_data_loader.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains the function used
              to handle the bulk data loader menu.
===============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include "../lib/headers/c/c_files.h"

int handle_bulk_data_loader_menu(void)
{
  int bulkDataLoaderMenuProccess = TRUE;
  while (bulkDataLoaderMenuProccess == TRUE)
  {
    if (globalTrigger.isBulkLoadingDataToStudentsTable == TRUE)
    {
      __utils_runtime_logger("entered bulk data loader menu: student database", "handle_bulk_data_loader_menu");
      show_current_menu("Bulk Data Loader: Student Database");
      printf("How would you like to load the data to the student database?\n");
      printf("Please enter the corresponding number to the action you would like to take.\n");
      printf("%sTo cancel this operation enter 'cancel'%s\n\n", yellow.colorCode, reset.colorCode);
      printf("1. Bulk load data from existing file\n");
      printf("2. Enter bulk data manually, then load it\n");
      printf("3. Main Menu\n");
      __utils_fgets_and_remove_newline(userInput.StrInput);
      bulkDataLoaderMenuProccess = FALSE;

      userInput.NumInput = atoi(userInput.StrInput);
      if (INPUT_IS_CANCEL(userInput.StrInput))
      {
        __utils_operation_cancelled("Bulk Data Loader: Student Database");
        return 0;
      }
      else if (strcmp(userInput.StrInput, "main menu") == 0 || strcmp(userInput.StrInput, "main") == 0)
      {
        __utils_runtime_logger("chose to return to main menu", "handle_bulk_data_loader_menu");
        system("clear");
        printf("Returning to main menu.\n");
        sleep(1);
        system("clear");
        handle_main_menu();
      }

      switch (userInput.NumInput)
      {
      case 1:
        // 1. check if file exists
        // 2. make sure the data in the file is formatted correctly
        // 3. load the file
        break;
      case 2:
        int DataFileFound = __utils_check_for_bulk_loader_data_file();
        switch (DataFileFound)
        {
        case FALSE: // file not found
          system("clear");
          printf("%sBulk data loader file not found. %s\n", yellow.colorCode, reset.colorCode);
          sleep(1);
          printf("%sGenerating new file...%s\n", green.colorCode, reset.colorCode);
          printf("New file successfully generated\n");
          sleep(1);
          system("clear");
          // todo do more
          break;
        case TRUE: // file and dir found
          system("clear");
          int resultOfSearch = search_for_prefix_in_file_name("active_data");
          switch (resultOfSearch)
          {
          case 0:
            printf("%sData file: " BOLD "%s %s%sfound%s\n", green.colorCode, csvDataFile.FileName, reset.colorCode, green.colorCode, reset.colorCode);
            int fileIsEmpty = check_if_data_file_is_empty();
            if (fileIsEmpty == FALSE)
            {
              system("clear");
              handle_non_empty_data_file();
            }
            else
            {
              printf("%sData file: " BOLD "%s%s%s is empty%s\n", green.colorCode, csvDataFile.FileName, reset.colorCode, green.colorCode, reset.colorCode);
              sleep(2);
              system("clear");
              __bulk_handle_data_entry();
            }
            break;
          case 1:
            break;
          default:
            __utils_error_logger("failed to search for prefix in file name", "handle_bulk_data_loader_menu", MINOR);
            break;
          }
          break;
        }
        break;
      case 3:
        __utils_runtime_logger("chose to return to main menu", "handle_bulk_data_loader_menu");
        system("clear");
        printf("Returning to main menu.\n");
        sleep(1);
        system("clear");
        handle_main_menu();
        break;
      default:
        __utils_error_logger("invalid input", "handle_bulk_data_loader_menu", MINOR);
        printf("Invalid input. Please try again.\n");
        handle_bulk_data_loader_menu();
      }
    }
    else
    {
      __utils_runtime_logger("entered bulk data loader menu: roster", "handle_bulk_data_loader_menu");
      show_current_menu("Bulk Data Loader: Roster");
      printf("WIP\n");
      printf("%s EXITING...%s\n", red.colorCode, reset.colorCode);
      exit(0);
      // todo work on this later. This will prob be WAYYYY more complicated than the student database
    }
  }
}

/************************************************************************************
 * generate_bulk_data_loader_file(): Generates a new bulk data loader .csv file
 *                                   with the file name and status passed as arguments.
 *                                    - fileStatus: "active" or "inactive"
 *
 * See usage in handle_bulk_data_loader_menu()
 ************************************************************************************/
void generate_bulk_data_loader_file(const char *fileName, const char *fileStatus)
{
  char filePath[100];
  snprintf(filePath, sizeof(filePath), "../build/data/%s_%s", fileStatus, fileName);

  FILE *file = fopen(filePath, "a");
  if (file == NULL)
  {
    __utils_error_logger("Failed to create bulk data loader csv file", "generate_bulk_data_loader_file", CRITICAL);
    return -1;
  }

  __utils_runtime_logger("Created bulk data loader csv file", "generate_bulk_data_loader_file");

  // storing the file name and path in the csvDataFile struct on file generation
  snprintf(csvDataFile.FileName, sizeof(csvDataFile.FileName), "%s_%s", fileStatus, fileName);
  sprintf(csvDataFile.FilePath, "%s", filePath);
  fprintf(file, "FirstName,LastName,StudentID\n");
  fclose(file);
}
/************************************************************************************
 * check_if_data_file_is_empty(): Checks if the bulk data loader .csv file is empty
 *                                or not and return TRUE or FALSE accordingly.
 *
 * See usage in handle_bulk_data_loader_menu()
 ************************************************************************************/
int check_if_data_file_is_empty(void)
{
  __utils_runtime_logger("checking if bulk data loader csv file is empty", "check_if_data_file_is_empty");
  FILE *file = fopen(csvDataFile.FilePath, "r");
  if (file == NULL)
  {
    __utils_error_logger("failed to open bulk data loader csv file", "check_if_data_file_is_empty", CRITICAL);
    fclose(file);
  }
  else
  {
    __utils_runtime_logger("opened bulk data loader csv file", "check_if_data_file_is_empty");
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  if (fileSize == 0)
  {
    __utils_runtime_logger("bulk data loader csv file is empty", "check_if_data_file_is_empty");
    return TRUE;
  }
  else
  {
    __utils_runtime_logger("bulk data loader csv file is NOT empty", "check_if_data_file_is_empty");
    fclose(file);
    return FALSE;
  }
}

/************************************************************************************
 * handle_non_empty_data_file(): This function handles the menu that appears in
 *                               the event that the bulk data loader .csv file is
 *                               not empty.
 *
 * See usage in handle_bulk_data_loader_menu()
 ************************************************************************************/
int handle_non_empty_data_file(void)
{
  system("clear");
  __utils_runtime_logger("entered handle non empty data file menu", "handle_non_empty_data_file");
  printf("%sWARNING: The data file" BOLD " %s%s%s is not empty. What would you like to do?%s\n", red.colorCode, csvDataFile.FileName, reset.colorCode, red.colorCode, reset.colorCode);
  printf("The data file " BOLD "%s" RESET " contains the following data:\n", csvDataFile.FileName, reset.colorCode);
  display_data_in_non_empty_file();
  printf("Enter the corresponding number to the action you would like to take.\n");
  printf("%sTo cancel this operation enter 'cancel'%s\n\n", yellow.colorCode, reset.colorCode);
  printf("1. Create a new file\n");
  printf("2. Add to the existing file\n");
  printf("3. Overwrite the existing file\n");
  printf("4: Main Menu\n");
  __utils_fgets_and_remove_newline(userInput.StrInput);
  userInput.NumInput = atoi(userInput.StrInput);
  if (INPUT_IS_CANCEL(userInput.StrInput))
  {
    __utils_operation_cancelled("handle_non_empty_data_file");
    return 0;
  }
  else if (strcmp(userInput.StrInput, "main menu") == 0 || strcmp(userInput.StrInput, "main") == 0)
  {
    __utils_runtime_logger("chose to return to main menu", "handle_non_empty_data_file");
    system("clear");
    printf("Returning to main menu.\n");
    sleep(1);
    system("clear");
    handle_main_menu();
  }
  switch (userInput.NumInput)
  {
  case 1:
    system("clear");
    int fileExists = __utils_check_for_bulk_loader_data_file();
    if (fileExists == TRUE)
    {
      // need to change the name of the already existing file to inactive
      rename(csvDataFile.FilePath, "../build/data/inactive_data.csv");
      generate_bulk_data_loader_file("data.csv", "active");
      printf("%sNew file successfully generated%s\n", green.colorCode, reset.colorCode);
      sleep(2);
      system("clear");
      // todo need to handle what would happen in the event that there are several inactive files. Currently this will just overwrite the first "inactive" file
    }
    break;
  case 2:
    system("clear");
    printf("%sWARNING: Adding to an existing could result in a loss of data or data corruption.\n%s", red.colorCode, reset.colorCode);
    printf("Are you sure you want to add to the existing file? [y/n]\n");
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      __bulk_handle_data_entry();
    }
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      system("clear");
      __utils_operation_cancelled("handle_non_empty_data_file");
      handle_non_empty_data_file();
    }
    else
    {
      __utils_error_logger("invalid input", "handle_non_empty_data_file", MINOR);
      printf("Invalid input. Please try again.\n");
      handle_non_empty_data_file();
    }
    break;
  case 3:
    system("clear");
    overwrite_existing_data_file(csvDataFile.FilePath);
    handle_bulk_data_loader_menu();
    break;
  case 4:
    system("clear");
    printf("Returning to main menu.\n");
    sleep(1);
    system("clear");
    handle_main_menu();
    break;
  default:
    __utils_error_logger("invalid input", "handle_non_empty_data_file", MINOR);
    printf("Invalid input. Please try again.\n");
    handle_non_empty_data_file();
  }
}

/************************************************************************************
 * overwrite_existing_data_file(): This function handles the menu that appears in
 *                                 the event that the user wants to overwrite the
 *                                 active bulk data loader .csv file.
 *
 * See usage in handle_non_empty_data_file()
 ************************************************************************************/
int overwrite_existing_data_file(const char *filePath)
{
  system("clear");
  printf("%sWARNING: You are about to delete all data in the file: " BOLD "%s %s\n", red.colorCode, csvDataFile.FileName, reset.colorCode);
  printf("Are you sure you want to continue?[y/n]\n");
  __utils_fgets_and_remove_newline(userInput.StrInput);
  if (INPUT_IS_YES(userInput.StrInput))
  {
    int fileCleared = clear_data_file(filePath);
    switch (fileCleared)
    {
    case 0:
      system("clear");
      printf("%sAll data in file: " BOLD "%s %s%shas been cleared%s\n", green.colorCode, csvDataFile.FileName, reset.colorCode, green.colorCode, reset.colorCode);
      sleep(2);
      printf("Returning to the bulk data loader menu...\n");
      sleep(1);
      system("clear");
      __utils_runtime_logger("cleared bulk data loader csv file", "overwrite_existing_data_file");
      return 0;
    default:
      __utils_error_logger("failed to clear bulk data loader csv file", "overwrite_existing_data_file", MINOR);
      return -1;
    }
  }
  else if (INPUT_IS_NO(userInput.StrInput))
  {
    system("clear");
    __utils_runtime_logger("chose not to overwrite bulk data loader csv file", "overwrite_existing_data_file");
    printf("Returning to the bulk data loader menu...\n");
    sleep(2);
    system("clear");
    handle_bulk_data_loader_menu();
  }
  else
  {
    __utils_error_logger("invalid input", "overwrite_existing_data_file", MINOR);
    printf("Invalid input. Please try again.\n");
    overwrite_existing_data_file(filePath);
  }
}

/************************************************************************************
 * clear_data_file(): Clears the passed in  bulk data loader .csv file.
 *
 * See usage in overwrite_existing_data_file()
 ************************************************************************************/
int clear_data_file(const char *filePath)
{
  FILE *file = fopen(filePath, "w");
  if (file == NULL)
  {
    __utils_error_logger("failed to open bulk data loader csv file", "clear_data_file", MINOR);
    fclose(file);
  }
  else
  {
    fclose(file);
    return 0;
  }
}

/************************************************************************************
 * __bulk_handle_data_entry(): Handles the input for bulk student data submission
 *
 * See usage in
 ************************************************************************************/
int __bulk_handle_data_entry()
{
  system("clear");
  show_bulk_loader_current_status("Adding students to database");
  printf("Please read the following steps carefully\n");
  printf("%sTo cancel this operation enter 'cancel'%s\n\n", yellow.colorCode, reset.colorCode);

  printf("1.Enter the desired first name of the student\n");
  printf("2.Confirm the first name you just entered\n");
  printf("3.Enter the desired last name(if no last name enter 'none')\n");
  printf("4.Confirm the last name you just entered\n");
  if (programSettings.autoStudentIDGenerationEnabled == TRUE)
  {
    printf("5.Confirm the automatically generated student ID\n");
  }
  else if (programSettings.autoStudentIDGenerationEnabled == FALSE)
  {
    printf("5.Enter and confirm the desired ID for the student\n");
  }
  printf("6.Enter the word " BOLD "'next'%sto move on to the next student entry\n", reset.colorCode);
  printf("7.If you are completely finished adding students enter the word " BOLD "'done'.%s\n", reset.colorCode);
  printf("\n");
  printf("Following these steps will store all entered data into the data file\n");
  if (globalTrigger.isBulkLoadingDataToStudentsTable == TRUE)
  {
    printf("and then load the data into the student database\n");
  }
  else
  {
    printf("and then load the data into the roster\n");
  }
  printf("\n");

  printf("When you are ready to begin enter the word 'start'\n");
  __utils_fgets_and_remove_newline(userInput.StrInput);
  if (INPUT_IS_CANCEL(userInput.StrInput))
  {
    __utils_operation_cancelled("__bulk_handle_data_entry");
    return 0;
  }
  else if (strcmp(userInput.StrInput, "start") == 0)
  {
    system("cd ../src/python/ && python3 startup.py");
  }
  else
  {
    system("clear");
    printf("Invalid decision. Please try again\n");
    sleep(1);
    __bulk_handle_data_entry();
  }
}

/************************************************************************************
 * display_data_in_non_empty_file(): Displays the data in the bulk data loader .csv
 *                                   file if the file is not empty.
 *
 * See usage in handle_non_empty_data_file()
 ************************************************************************************/
int display_data_in_non_empty_file(void)
{
  __utils_runtime_logger("displaying data in non empty data file", "display_data_in_non_empty_file");
  FILE *file = fopen(csvDataFile.FilePath, "r");
  if (file == NULL)
  {
    __utils_error_logger("failed to open bulk data loader csv file", "display_data_in_non_empty_file", MINOR);
    fclose(file);
    return 1;
  }
  else
  {
    __utils_runtime_logger("opened bulk data loader csv file", "display_data_in_non_empty_file");
  }

  char line[1024];
  int lineNumber = 0;

  while (fgets(line, 1024, file))
  {
    lineNumber++;

    char *token;
    token = strtok(line, ",");
    printf("Line %d: ", lineNumber);
    while (token != NULL)
    {
      printf("%s ", token);
      token = strtok(NULL, ",");
    }
    printf("\n");
    printf("\n");
  }
  fclose(file);

  return 0;
}