#include <stdio.h>
#include <stdlib.h>
#include "../lib/utils.h"
#include "../lib/headers/main.h"

int isRunning = FALSE;

int windowHeight;
int windowWidth;

char buffer[50];

int main()
{

  int isRunning = TRUE;
  // system("clear");

  while (isRunning == TRUE)
  {
    /*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-START OF MAIN MENU+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
    puts("Welcome to the Hallpass Hero CLI tool!");
    puts("Please select one of the options below:");

    char options[6][50] = {
        // creating a roster is like creating a new class as a collection of students
        "1. Create a new roster",
        // managing a roster is like managing a class
        "2. View and manage an existing roster",
        "3. Add a student to the student database",
        "4. View and manage the student database",
        "5. Search for a student",
        "6. Exit\n"};
    // showing main menu options
    for (int i = 0; i < 6; ++i)
    {
      printf("%s\n", options[i]);
    }

    /*Here we are handling the input that the user makes on the main menu*/
    FGETS(buffer);
    int menuInput = atoi(buffer);

    // to create a new roster
    if (menuInput == 1)
    {
      system("clear");
      puts("You have chosen tot create a new roster.");
      puts("Is that correct?");
      FGETS(buffer);
      REMOVE_NEWLINE_CHAR(buffer);
      if (INPUT_IS_YES(buffer))
      {
        puts("Great! Let's get started.");
        // do stuff
      }
      else if (INPUT_IS_NO(buffer))
      {
        puts("No problem. Let's try again.");
        refresh_main_menu();
      }
      else
      {
        puts("Sorry, I didn't understand that.");
        puts("Please try again");
        refresh_main_menu();
      }
    }
    // to manage an existing roster
    else if (menuInput == 2)
    {
      system("clear");
      puts("You have chosen to manage an existing roster.");
      puts("Is that correct?");
      FGETS(buffer);
      REMOVE_NEWLINE_CHAR(buffer);
      if (INPUT_IS_YES(buffer))
      {
        puts("Great! Let's get started.");
        // do stuff
      }
      else if (INPUT_IS_NO(buffer))
      {
        puts("No problem. Let's try again.");
        refresh_main_menu();
      }
      else
      {
        puts("Sorry, I didn't understand that.");
        puts("Please try again");
        refresh_main_menu();
      }
    }
    // to add a student to the student database
    else if (menuInput == 3)
    {
      system("clear");
      puts("You have chosen to add a student to the student database.");
      puts("Is that correct?");
      FGETS(buffer);
      REMOVE_NEWLINE_CHAR(buffer);
      if (INPUT_IS_YES(buffer))
      {
        puts("Great! Let's get started.");
        // do stuff
      }
      else if (INPUT_IS_NO(buffer))
      {
        puts("No problem. Let's try again.");
        refresh_main_menu();
      }
      else
      {
        puts("Sorry, I didn't understand that.");
        puts("Please try again");
        refresh_main_menu();
      }
    }
    // to view and manage the student database
    else if (menuInput == 4)
    {
      system("clear");
      puts("You have chosen manage the student database.");
      puts("Is that correct?");
      FGETS(buffer);
      REMOVE_NEWLINE_CHAR(buffer);
      if (INPUT_IS_YES(buffer))
      {
        puts("Great! Let's get started.");
        // do stuff
      }
      else if (INPUT_IS_NO(buffer))
      {
        puts("No problem. Let's try again.");
        refresh_main_menu();
      }
      else
      {
        puts("Sorry, I didn't understand that.");
        puts("Please try again");
        refresh_main_menu();
      }
    }
    // to search for a specific student
    else if (menuInput == 5)
    {
      system("clear");
      puts("You have chosen to seach for a student.");
      puts("Is that correct?");
      FGETS(buffer);
      REMOVE_NEWLINE_CHAR(buffer);
      if (INPUT_IS_YES(buffer))
      {
        puts("Great! Let's get started.");
        // do stuff
      }
      else if (INPUT_IS_NO(buffer))
      {
        puts("No problem. Let's try again.");
        refresh_main_menu();
      }
      else
      {
        puts("Sorry, I didn't understand that.");
        puts("Please try again");
        refresh_main_menu();
      }
    }
    // exit the program from the main menu
    else if (menuInput == 6)
    {
      system("clear");
      puts("You have chosen to exit Hallpass Hero.");
      puts("Is that correct?");
      FGETS(buffer);
      REMOVE_NEWLINE_CHAR(buffer);
      if (INPUT_IS_YES(buffer))
      {
        puts("Sorry to see you go.");
        isRunning = FALSE;
      }
      else if (INPUT_IS_NO(buffer))
      {
        puts("No problem. Let's try again.");
        refresh_main_menu();
      }
      else
      {
        puts("Sorry, I didn't understand that.");
        puts("Please try again");
        refresh_main_menu();
      }
    }
    /*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-END OF MAIN MENU+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
  }

  return 0;
}

// this refreshes the ui
void refresh_main_menu()
{
  // windowHeight = 20;
  // windowWidth = 90;
  system("clear");
  // puts("|===========================================================================================|");
  // for (int i = 0; i < windowHeight; ++i)
  // {
  //   printf("| %-90s|\n", "");
  // }
  // puts("|===========================================================================================|");
}