#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/headers/utils.h"
#include "../lib/headers/c_files.h"

UserInput settingsInput;
int settingsMenuRunning = TRUE;

int show_settings_menu(void)
{
  while (settingsMenuRunning == TRUE)
  {
    show_current_menu("Settings Menu");
    puts("Enter the number of the option you would like to select:");
    puts("1. Enable/Disable Runtime Logging");
    puts("2. Main Menu");
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(settingsInput.StrInput);
    settingsInput.NumInput = atoi(settingsInput.StrInput);
    settingsMenuRunning = FALSE;
  }
  switch (settingsInput.NumInput)
  {
  case 1:
    __utils_runtime_logger("User entered the enable/disable runtime logging menu", "settings");
    handle_runtime_logging_logic();
    break;
  case 2:
    system("clear");
    puts("Returning to main menu...");
    sleep(1);
    system("clear");
    return 0;
    break;
  default:
    system("clear");
    puts("Invalid input");
    sleep(1);
    system("clear");
    puts("Please try again.");
    show_settings_menu();
  }
}
int handle_runtime_logging_logic(void)
{
  if (programSettings.runtimeLoggingEnabled == TRUE)
  {
    system("clear");
    show_current_step("Enable/Disable Runtime Logging", 1, 1);
    printf("Currently runtime logging is " BOLD GREEN "enabled" RESET "\n");
    puts("Would you like to disable runtime logging?[y/n]");
    puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(settingsInput.StrInput);
    if (INPUT_IS_YES(settingsInput.StrInput))
    {
      system("clear");
      puts("Enabling runtime logging...");
      programSettings.runtimeLoggingEnabled = FALSE;
      puts(RED "Runtime logging has been disabled" RESET);
      sleep(2);
      system("clear");
      time_t currentTime;
      time(&currentTime);

      FILE *runtimeLogFile = fopen("../logs/runtime.log", "a");
      if (runtimeLogFile == NULL)
      {
        perror("Error creating runtimeLog file");
        return 1;
      }
      fprintf(runtimeLogFile, "Logged @ %s", ctime(&currentTime));
      fprintf(runtimeLogFile, "Runtime logging has been disabled\n");
      fprintf(runtimeLogFile, "======================================================================================\n\n");
      fflush(runtimeLogFile);
      fclose(runtimeLogFile);
      return 0;
    }
    if (INPUT_IS_NO(settingsInput.StrInput || INPUT_IS_CANCEL(settingsInput.StrInput)))
    {
      system("clear");
      puts("Returning to settings menu...");
      sleep(1);
      system("clear");
      settingsMenuRunning = TRUE;
    }
  }
  else if (programSettings.runtimeLoggingEnabled == FALSE)
  {
    system("clear");
    show_current_step("Enable/Disable Runtime Logging", 1, 1);
    printf("Currently runtime logging is " BOLD RED "disabled" RESET "\n");
    puts("Would you like to enable runtime logging?[y/n]");
    puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(settingsInput.StrInput);
    if (INPUT_IS_YES(settingsInput.StrInput))
    {
      system("clear");
      puts("Enabling runtime logging...");
      programSettings.runtimeLoggingEnabled = TRUE;
      puts(GREEN "Runtime logging has been enabled" RESET);
      sleep(2);
      system("clear");

      time_t currentTime;
      time(&currentTime);

      // Create the runtime.log file if it doesn't already exist
      FILE *runtimeLogFile = fopen("../logs/runtime.log", "a");
      if (runtimeLogFile == NULL)
      {
        perror("Error creating runtimeLog file");
        return 1;
      }
      fprintf(runtimeLogFile, "Logged @ %s", ctime(&currentTime));
      fprintf(runtimeLogFile, "Runtime logging has been enabled\n");
      fprintf(runtimeLogFile, "======================================================================================\n\n");
      fflush(runtimeLogFile);
      fclose(runtimeLogFile);

      return 0;
    }
    else if (INPUT_IS_NO(settingsInput.StrInput))
    {
      system("clear");
      puts("Returning to settings menu...");
      sleep(1);
      system("clear");
      settingsMenuRunning = TRUE;
    }
  }
  else
  {
    system("clear");
    system("Please enter a valid input");
    sleep(1);
    system("clear");
    handle_runtime_logging_logic();
  }
}