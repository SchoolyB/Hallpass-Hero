#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
/*The plan is to keep this file clean.*/
#ifndef UTILS_H
#define UTILS_H

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-START MACROS+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
// Booleans
#define FALSE 0
#define TRUE 1
//--------------------------------------------------------------------------------//

// Colors
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define BLUE "\x1B[34m"
#define GREEN "\x1B[32m"
#define PURPLE "\x1B[35m"
#define YELLOW "\x1B[33m"
//--------------------------------------------------------------------------------//

// handling user confirmation for yes and no
#define INPUT_IS_YES(param) (strcmp(param, "y") == 0 || strcmp(param, "Y") == 0 || \
                             strcmp(param, "yes") == 0 || strcmp(param, "Yes") == 0)

#define INPUT_IS_NO(param) (strcmp(param, "n") == 0 || strcmp(param, "N") == 0 || \
                            strcmp(param, "no") == 0 || strcmp(param, "No") == 0)
//--------------------------------------------------------------------------------//

// standard fgets() and removing the newline character at the end of the users string input
#define FGETS(param) (fgets(param, sizeof(param), stdin))

#define REMOVE_NEWLINE_CHAR(param)         \
  do                                       \
  {                                        \
    size_t len = strlen(param);            \
    if (len > 0 && param[len - 1] == '\n') \
    {                                      \
      param[len - 1] = '\0';               \
    }                                      \
  } while (0)
//--------------------------------------------------------------------------------//

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-END OF MACROS+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-START FUNCTIONS+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int error_logger(char *errorMessage, char *function, char *action)
{

  FILE *errorLog;
  errorLog = fopen("../logs/errors.log", "a");

  if (errorLog == NULL)
  {
    perror("Error opening the error log file");
    error_logger("Error opening the error log file", "error_logger", "ret1");
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
    error_logger("Incorrect action passed as argument", function, "ret1");
  }
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-END OF FUNCTIONS+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#endif
