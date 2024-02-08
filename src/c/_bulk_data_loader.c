#include <stdio.h>
#include <stdlib.h>
#include "../lib/headers/c/c_files.h"
#include "../lib//utils.c"

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
      printf("%sTo cancel this operation enter 'cancel'%s\n", yellow.colorCode, reset.colorCode);
      printf("1.Bulk load data from existing file\n");
      printf("2.Bulk load data manually\n");

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
        // 1.generate a new file
        // 2. allow the user to enter as much data as they want from the ui
        // 3. store that data into a file
        break;
      }
    }
    else
    {
      __utils_runtime_logger("entered bulk data loader menu: roster", "handle_bulk_data_loader_menu");
      show_current_menu("Bulk Data Loader: Roster");
      // todo work on this later. This will prob be WAYYYY more complicated than the student database
    }
  }
}

void manually_load_student_data(void)
{
  // this function will need to allow the user to enter as much data as they want
}