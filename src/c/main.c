#include <stdio.h>
#include <stdlib.h>
#include "../lib/utils.h"
#include "../lib/headers/main.h"

int mainMenuProccess = FALSE;

int windowHeight;
int windowWidth;

char buffer[50];

char MainMenuOptions[7][50] = {
    // creating a roster is like creating a new class as a collection of students
    "1. Create a new roster",
    // managing a roster is like managing a class
    "2. View and manage an existing roster",
    "3. Add a student to the student database",
    "4. View and manage the student database",
    "5. Search for a student",
    "6. Help",
    "7. Exit"};

int main()
{

  int mainMenuProccess = TRUE;
  // system("clear");

  while (mainMenuProccess == TRUE)
  {
    /*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-START OF MAIN MENU+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
    puts("Welcome to the Hallpass Hero CLI tool!");
    puts("Please select one of the options below:");

    // showing main menu options
    puts("===========================================================================================");
    for (int i = 0; i < 7; ++i)
    {
      printf("| %s %-90s\n", MainMenuOptions[i], "");
    }
    puts("===========================================================================================");
    /*Here we are handling the input that the user makes on the main menu*/
    FGETS(buffer);
    REMOVE_NEWLINE_CHAR(buffer);
    int menuInput = atoi(buffer);

    // to create a new roster
    if (menuInput == 1 || strcmp(buffer, "new roster") == 0)
    {
      handle_main_menu_decision(1);
    }
    // to manage an existing roster
    else if (menuInput == 2 || strcmp(buffer, "manage roster") == 0)
    {
      handle_main_menu_decision(2);
    }
    // to add a student to the student database
    else if (menuInput == 3 || strcmp(buffer, "add student") == 0)
    {
      handle_main_menu_decision(3);
    }
    // to view and manage the student database
    else if (menuInput == 4 || strcmp(buffer, "manage student database") == 0)
    {
      handle_main_menu_decision(4);
    }
    // to search for a student
    else if (menuInput == 5 || strcmp(buffer, "search student") == 0)
    {
      handle_main_menu_decision(5);
    }
    // to exit the program
    // to help
    else if (menuInput == 6 || strcmp(buffer, "help") == 0)
    {
      handle_main_menu_decision(6);
    }
    else if (menuInput == 7 || strcmp(buffer, "exit") == 0)
    {
      puts("See you soon!");
      mainMenuProccess = FALSE;
    }
    else
    {
      puts("Sorry, I didn't understand that.");
      puts("Please try again");
      refresh_main_menu();
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
  // system("resize -s 20 90");
  system("clear");
  puts("|===========================================================================================|");
  for (int i = 0; i < 7; ++i)
  {
    printf("| %s %-90s\n", MainMenuOptions[i], "");
  }
  puts("|===========================================================================================|");
}

void handle_main_menu_decision(int num)
{
  system("clear");
  switch (num)
  {
  case 1:
    puts("You selected to create a new roster.");
    break;
  case 2:
    puts("You selected to manage an existing roster.");
    break;
  case 3:
    puts("You selected to add a student to the student database.");
    break;
  case 4:
    puts("You selected to view and manage the student database.");
    break;
  case 5:
    puts("You selected to search for a student.");
    break;
  case 6:
    puts("You selected to help.");
    break;
  case 7:
    puts("You selected to exit the program.");
    break;
  default:
    puts("Sorry, I didn't understand that.");
    puts("Please try again");
    refresh_main_menu();
    break;
  }
  puts("Is that correct?");

  FGETS(buffer);
  REMOVE_NEWLINE_CHAR(buffer);
  if (INPUT_IS_YES(buffer) && num == 1)
  {
    puts("Ok then, let's create a new roster.");
    create_new_roster();
  }
  else if (INPUT_IS_YES(buffer) && num == 2)
  {
    puts("Ok then, let's manage an existing roster.");
    // manage_roster();
  }
  else if (INPUT_IS_YES(buffer) && num == 3)
  {
    puts("Ok then, let's add a student to the student database.");
    // add_student_to_database();
  }
  else if (INPUT_IS_YES(buffer) && num == 4)
  {
    puts("Ok then, let's view and manage the student database.");
    // manage_student_database();
  }
  else if (INPUT_IS_YES(buffer) && num == 5)
  {
    puts("Ok then, let's search for a student.");
    // search_for_student();
  }
  else if (INPUT_IS_YES(buffer) && num == 6)
  {
    puts("Ok then, let's help.");
    // show_help_menu();
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

void create_new_roster()
{
  system("clear");

  int newROsterMenuIsRunning = TRUE;
  int menuWidth = 70;
  int menuHeight = 10;

  while (newROsterMenuIsRunning == TRUE)
  {
    mainMenuProccess = FALSE;
    char newRosterOptions[2][50] = {
        "1. Create a new roster",
        "2. Back to main menu"};
    puts("What would you like to do?");
    puts("|===========================================================================================");
    for (int i = 0; i < 2; ++i)
    {
      printf("| %s %-90s\n", newRosterOptions[i], "");
    }
    for (int i = 0; i < menuHeight; ++i)
    {
      printf("| %-90s\n", "");
    }
    puts("|===========================================================================================");
    FGETS(buffer);
    REMOVE_NEWLINE_CHAR(buffer);
    int menuInput = atoi(buffer);
    if (menuInput == 1 || strcmp(buffer, "new roster") == 0)
    {
      puts("You selected to create a new roster.");
      newROsterMenuIsRunning = FALSE;
      system("clear");
      puts("What would you like to name your roster?");
    }
    else if (menuInput == 2 || strcmp(buffer, "back") == 0 || strcmp(buffer, "exit") == 0)
    {
      puts("You selected to go back to the main menu.");
      refresh_main_menu();
      newROsterMenuIsRunning = FALSE;
    }
    else
    {
      puts("Sorry, I didn't understand that.");
      puts("Please try again");
      create_new_roster();
    }

    ;
  }
}