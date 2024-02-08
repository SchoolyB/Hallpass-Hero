#include <stdio.h>
#include <stdlib.h>
#include "../lib/headers/c/c_files.h"

int handle_bulk_data_loader_menu(void)
{
  globalTrigger.isUsingBulkLoader = TRUE;
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
      printf("1.Bulk load data from existing file\n");
      printf("2.Enter bulk data manually, then load it\n");

      __utils_fgets_and_remove_newline(userInput.StrInput);
      bulkDataLoaderMenuProccess = FALSE;

      userInput.NumInput = atoi(userInput.StrInput);
      if (INPUT_IS_CANCEL(userInput.StrInput))
      {
        __utils_operation_cancelled("Bulk Data Loader: Student Database");
        return 0;
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
          printf("%sBulk data loader file not found. New file generated%s\n", yellow.colorCode, reset.colorCode);
          sleep(2);
          system("clear");
          // todo do more
          break;
        case TRUE: // file and dir found
          system("clear");
          printf("%sData file: " BOLD "%s %s%sfound%s\n", green.colorCode, jsonDataFile.FileName, reset.colorCode, green.colorCode, reset.colorCode);
          int fileIsEmpty = check_if_data_file_is_empty();
          if (fileIsEmpty == FALSE)
          {
            system("clear");
            handle_non_empty_data_file();
          }
          else
          {
            system("clear");
            printf("%sData file is empty%s\n", green.colorCode, reset.colorCode);
            sleep(2);
            system("clear");
            // todo add data...
          }
          break;
        }
        // 1.generate a new file
        // 2. allow the user to enter as much data as they want from the ui
        // 3. store that data into a file
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

void manually_load_student_data(void)
{
  // this function will need to allow the user to enter as much data as they want
}

void generate_bulk_data_loader_file(const char *fileName, int num)
{
  char filePath[100];
  snprintf(filePath, sizeof(filePath), "../build/data/%d_%s", num, fileName);

  FILE *file = fopen(filePath, "a");
  if (file == NULL)
  {
    __utils_error_logger("Failed to create bulk data loader json file", "generate_bulk_data_loader_file", CRITICAL);
    return -1;
  }

  __utils_runtime_logger("Created bulk data loader json file", "generate_bulk_data_loader_file");

  // storing the file name and path in the jsonDataFile struct on file generation
  snprintf(jsonDataFile.FileName, sizeof(jsonDataFile.FileName), "%d_%s", num, fileName);
  sprintf(jsonDataFile.FilePath, "%s", filePath);

  fclose(file);
}
/************************************************************************************
 * check_if_data_file_is_empty(): Checks if the bulk data loader .json file is empty
 *                                or not and return TRUE or FALSE accordingly.
 *
 * See usage in handle_bulk_data_loader_menu()
 ************************************************************************************/
int check_if_data_file_is_empty()
{
  __utils_runtime_logger("checking if bulk data loader json file is empty", "check_if_data_file_is_empty");
  FILE *file = fopen(jsonDataFile.FilePath, "r");
  if (file == NULL)
  {
    __utils_error_logger("failed to open bulk data loader json file", "check_if_data_file_is_empty", CRITICAL);
    fclose(file);
  }
  else
  {
    __utils_runtime_logger("opened bulk data loader json file", "check_if_data_file_is_empty");
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  if (fileSize == 0)
  {
    __utils_runtime_logger("bulk data loader json file is empty", "check_if_data_file_is_empty");
    return TRUE;
  }
  else
  {
    __utils_runtime_logger("bulk data loader json file is NOT empty", "check_if_data_file_is_empty");
    fclose(file);
    return FALSE;
  }
}

int handle_non_empty_data_file()
{
  system("clear");
  __utils_runtime_logger("entered handle non empty data file menu", "handle_non_empty_data_file");
  printf("%sWARNING: The data file" BOLD " %s%s%s is not empty. What would you like to do?%s\n", red.colorCode, jsonDataFile.FileName, reset.colorCode, red.colorCode, reset.colorCode);
  printf("Enter the corresponding number to the action you would like to take.\n");
  printf("%sTo cancel this operation enter 'cancel'%s\n\n", yellow.colorCode, reset.colorCode);
  printf("1. Create a new file\n");
  printf("2. Add to the existing file\n");
  printf("3. Overwrite the existing file\n");

  __utils_fgets_and_remove_newline(userInput.StrInput);
  userInput.NumInput = atoi(userInput.StrInput);

  switch (userInput.NumInput)
  {
  case 1:
    system("clear");
    int fileExists = __utils_check_for_bulk_loader_data_file();
    if (fileExists == TRUE)
    {
      generate_bulk_data_loader_file("data.json", jsonDataFile.fileNumIota++);
      printf("%sNew file successfully generated%s\n", green.colorCode, reset.colorCode);
      printf("Data file now set to: " BOLD "%s%s\n", jsonDataFile.FileName, reset.colorCode);
      printf("Data file path now set to: " BOLD "%s%s\n", jsonDataFile.FilePath, reset.colorCode);
      // sleep(3);
    }
    break;
  case 2:
    system("clear");
    // todo add to the existing file
    break;
  case 3:
    system("clear");
    // todo overwrite the existing file
    overwrite_existing_data_file(jsonDataFile.FilePath);
    handle_bulk_data_loader_menu();
    break;
  }
}

int overwrite_existing_data_file(const char *filePath)
{
  system("clear");
  printf("%sWARNING: You are about to delete all data in the file: " BOLD "%s %s\n", red.colorCode, jsonDataFile.FileName, reset.colorCode);
  printf("Are you sure you want to continue?[y/n]\n");
  __utils_fgets_and_remove_newline(userInput.StrInput);
  if (INPUT_IS_YES(userInput.StrInput))
  {
    int fileCleared = clear_data_file(filePath);
    switch (fileCleared)
    {
    case 0:
      system("clear");
      printf("%sAll data in file: " BOLD "%s %s%shas been cleared%s\n", green.colorCode, jsonDataFile.FileName, reset.colorCode, green.colorCode, reset.colorCode);
      sleep(2);
      printf("Returning to the bulk data loader menu...\n");
      sleep(1);
      system("clear");
      __utils_runtime_logger("cleared bulk data loader json file", "overwrite_existing_data_file");
      return 0;
    default:
      __utils_error_logger("failed to clear bulk data loader json file", "overwrite_existing_data_file", MINOR);
      return -1;
    }
  }
  else if (INPUT_IS_NO(userInput.StrInput))
  {
    __utils_operation_cancelled("Overwriting existing data file");
    return 0;
  }
  else
  {
    __utils_error_logger("invalid input", "overwrite_existing_data_file", MINOR);
    printf("Invalid input. Please try again.\n");
    overwrite_existing_data_file(filePath);
  }
}

int clear_data_file(const char *filePath)
{
  FILE *file = fopen(filePath, "w");
  if (file == NULL)
  {
    __utils_error_logger("failed to open bulk data loader json file", "clear_data_file", MINOR);
    fclose(file);
  }
  else
  {
    fclose(file);
    return 0;
  }
}
