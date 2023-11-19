#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void show_current_menu(char *str)
{
  printf("Current Menu: " BOLD "%s" RESET "\n", str);
}

int UTILS_ERROR_LOGGER(char *errorMessage, char *function, char *action)
{

  /*Making sure to create the logs dir if
  it does not exist. For Win, Linux, Unix respectively*/
#ifdef _WIN32
  // Windows
  if (_mkdir("../logs") != 0)
#else
  // Linux/Unix
  if (mkdir("../logs", 0777) != 0)
#endif
  {
    perror("Error creating the log directory");
    return 1;
  }

  FILE *errorLog;
  errorLog = fopen("../logs/errors.log", "a");

  if (errorLog == NULL)
  {
    perror("Error opening the error log file");
    UTILS_ERROR_LOGGER("Error opening the error log file", "UTILS_ERROR_LOGGER", "ret1");
  }

  time_t currentTime;
  time(&currentTime);

  // Handles critical errors
  if (strcmp(action, "exit") == 0)
  {
    fprintf(errorLog, "Logged @ %s", ctime(&currentTime));
    fprintf(errorLog, "Critical Error: %s in function: %s()\n", errorMessage, function);
    fprintf(errorLog, "Exited program\n");
    fflush(errorLog);
    fclose(errorLog);
    exit(1);
  }
  // Handles moderate errors
  else if (strcmp(action, "ret1") == 0)
  {

    fprintf(errorLog, "Logged @ %s", ctime(&currentTime));
    fprintf(errorLog, "Moderate Error: %s in function: %s()\n", errorMessage, function);
    fflush(errorLog);
    fclose(errorLog);
    return 1;
  }
  // Handles minor errors
  else if (strcmp(action, "ret0") == 0)
  {
    fprintf(errorLog, "Logged @ %s", ctime(&currentTime));
    fprintf(errorLog, "Minor Error: %s in function: %s()\n", errorMessage, function);
    fflush(errorLog);
    fclose(errorLog);
    return 0;
  }

  else
  {
    UTILS_ERROR_LOGGER("Incorrect action passed as argument", function, "ret1");
  }
}
// Removes the newline character at the end of the user's string input
void UTILS_REMOVE_NEWLINE_CHAR(char *param)
{
  size_t len = strlen(param);
  if (len > 0 && param[len - 1] == '\n')
  {
    param[len - 1] = '\0';
  }
}