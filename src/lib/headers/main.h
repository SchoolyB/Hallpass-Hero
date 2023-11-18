#ifndef MAIN_H
#define MAIN_H

// PROTOTYPES
void handle_main_menu_decision();
void create_new_roster();
void manage_roster();
void add_student_to_db();
// void manage_student_db();
// void search_for_student();
// void show_help_menu();

typedef struct
{
  // Student Personal Information
  char firstName[20];
  char lastName[20];
  int studentID;
  char email[50];
  char phoneNumber[20];
  char address[50];
  // Student Academic Information
  // todo need to add a member the encompasses 'goal,diploma,major,cert,course'

} Student;

struct Course
{
  char courseName[20];
  char courseDescription[50];
  char courseCredits[20];
  char courseInstructor[20];
  char courseLocation[20];
  char courseStatus[20];
  char courseGrade[20];
  char courseStartDate[20];
  char courseEndDate[20];
} Course;
#endif