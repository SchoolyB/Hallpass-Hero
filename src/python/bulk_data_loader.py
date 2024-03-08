#===============================================================================
#File Name   : bulk_data_loader.py
#-------------------------------------------------------------------------------
#Author      : Marshall Burns a.k.a. Schooly
#-------------------------------------------------------------------------------
#Description : This script handles all of the bulk data loading for Hallpass Hero
#              It is called via a system call from the C code in _bulk_data_loader.c 
#         
#===============================================================================

import sys
import os
import json
import time
from enum import Enum
import time
import settings
import random

# Import the utils module from the lib/utils/python directory 
sys.path.append("../lib/utils/python")
import utils

bulkLoaderDataFilePath = "../../build/data/active_data.json"
     
class Student:
  FirstName = None
  TruncatedLastName = None
  LastName = None
  StudentID = None


class Statistic:
  TotalStudentsProcessed = None
  TotalSessionsCompleted = None
  CurrentSessionNumber = None
  studentsProcessedInCurrentSession = None
  CurrentSessionStartTime = None
  CurrentSessionEndTime = None
  PreviousSessionStartTime = None
  PreviousSessionEndTime = None 

def main():
  utils.__utils_runtime_logger("attempted to run bulk data loader", "bulk_data_loader.main()")
  utils.clear()
  utils.check_and_populate_stats_file()
  ######################################################################
  # get_student_information(): Gets the student's first name, last name,  
  #                            and student ID
  #
  # see usage in handle_main_loop()
  ######################################################################
  def get_student_information(type):
    # Get the first name
    if type == "first name":
      userInput = input(f"Enter the student's {type}: \n")
      hasNonSpaceChar = utils.has_one_non_space_char(userInput)
      if(utils.check_for_special_chars(userInput) == True):
        print(f"{settings.Colors.RED}The student's first name cannot contain special characters{settings.Colors.RESET}")
        time.sleep(2)
        utils.clear()
        get_student_information("first name")
      else:
        if(hasNonSpaceChar == False):
          print(f"{settings.Colors.YELLOW}The student's first name cannot be empty{settings.Colors.RESET}")
          time.sleep(2)
          utils.clear()
          get_student_information("first name")
        elif (hasNonSpaceChar == True):
          if userInput == "cancel":
            utils.__utils_operation_cancelled("get_student_information")
            exit()
          else:
            Student.FirstName = userInput.capitalize()
          
        
    # Get the last name 
    elif type == "last name":
      utils.clear()
      print(f"First Name: {Student.FirstName}")
      print("Enter the student's last name")
      print(f"{settings.Colors.YELLOW}If the student doesn't have a last name enter 'none' {settings.Colors.RESET}")
      userInput = input(f"Enter the student's {type}: \n")
      hasNonSpaceChar = utils.has_one_non_space_char(userInput)
      if(utils.check_for_special_chars(userInput) == True):
        print(f"{settings.Colors.RED}The student's last name cannot contain special characters{settings.Colors.RESET}")
        time.sleep(2)
        utils.clear()
        get_student_information("last name")
      else:
        if(hasNonSpaceChar == False):
          print(f"{settings.Colors.RED}The student's last name cannot be empty, if the student doesn't have a last please enter 'none'.{settings.Colors.RESET}")
          time.sleep(3)
          print("Please try again.")
          utils.clear()
          get_student_information("last name")
        elif (hasNonSpaceChar == True):
          if userInput == "cancel":
            utils.__utils_operation_cancelled("get_student_information")
            exit()
          if(userInput == "none"):
             print(f"{settings.Colors.YELLOW}The student's last name will be set to 'none'{settings.Colors.RESET}")
             Student.LastName = "none"
          else:
             if(len(userInput) > 10):
                Student.TruncatedLastName = truncate_last_name(userInput)
                print(f"{settings.Colors.YELLOW}Entered last name {utils.Font.BOLD}{settings.Colors.YELLOW}{userInput}{settings.Colors.RESET}{settings.Colors.YELLOW} longer than 10 characters, this will be shortened to {utils.Font.BOLD}{Student.TruncatedLastName}{settings.Colors.RESET}{settings.Colors.YELLOW} in the student ID.{settings.Colors.RESET}")
                print("Are you sure you want to continue with this students last name?[y/n]")
                confirmation = input()
                if confirmation == "y" or confirmation == "Y": 
                   print(f"{settings.Colors.GREEN}The student's last name has been set to {Student.TruncatedLastName}{settings.Colors.RESET}")
                   Student.LastName = Student.TruncatedLastName.capitalize()
                elif confirmation == "n" or confirmation == "N":
                  print("Re-enter the student's last name")
                  time.sleep(2)
                  utils.clear()
                  print(f"First Name: {Student.FirstName}")
                  get_student_information("last name")
             else:
                Student.LastName = userInput.capitalize()  
    # Get and set the ID 
    #todo unfortunately I get the automatic generation of the student id is done in the C code so I will have to basically create a new function in Python to generate the student id....  or I can store the student id from the c code in a memory address and then retrieve it from the python code
    elif type == "student id":
      utils.clear()
      print(f"First Name: {Student.FirstName}")
      print(f"Last Name: {Student.LastName}")
      print("Checking status of auto student id generation in settings...")
      time.sleep(1)
      if(settings.GlobalSettings.AutoStudentIDGenerationEnabled == 1):
        print(f"{settings.Colors.GREEN}Auto student id generation is enabled{settings.Colors.RESET}")
        print(f"{settings.Colors.YELLOW}Automatically generating student ID{settings.Colors.RESET}")
        time.sleep(2)
        Student.StudentID = auto_generate_student_id(Student.FirstName, Student.LastName)
      else:
        print(f"{settings.Colors.RED}Auto student id generation is disabled{settings.Colors.RESET}")
        userInput = input(f"Enter the student's {type}: \n")
        hasNonSpaceChar = utils.has_one_non_space_char(userInput)
        if(hasNonSpaceChar == False):
          print(f"{settings.Colors.RED}The student's id cannot be empty{settings.Colors.RESET}")
          time.sleep(2)
          utils.clear()
          get_student_information("student id")
        elif (hasNonSpaceChar == True):
          if userInput == "cancel":
            utils.__utils_operation_cancelled("get_student_information")
            exit()
          elif(len(userInput) > 15):
            print(f"{settings.Colors.YELLOW}The id you entered is too long. Please try again.{settings.Colors.RESET}")
            time.sleep(2)
            utils.clear()
            get_student_information("student id")
          elif(len(userInput) < 2):
            print(f"{settings.Colors.YELLOW}The id you entered is too short. Please try again.{settings.Colors.RESET}")
            time.sleep(2)
            utils.clear()
            get_student_information("student id")



  ######################################################################
  # truncate_last_name(): Truncates the last name to 10 characters
  #
  # see usage in get_student_information()
  ######################################################################          
  def truncate_last_name(lastName):
    truncatedLastName = lastName[0:10]
    truncatedLastName = truncatedLastName.capitalize()
    return truncatedLastName
  

  ######################################################################
  # take_first_two_letters(): Takes the first two letters of the first
  #                           name and returns them in uppercase
  #
  # see usage in auto_generate_student_id()
  ######################################################################  
  def take_first_two_letters(firstName):
    firstTwoLettersFirstName = firstName[0:2]
    firstTwoLettersFirstName = firstTwoLettersFirstName.upper()
    return str(firstTwoLettersFirstName)


  ######################################################################
  # auto_generate_student_id(): Automatically generates a student ID
  #                             based on the student's first and last name
  #
  # see usage in get_student_information()
  ######################################################################
  def auto_generate_student_id(firstName, lastName):
    nums = [1, 2, 3, 4, 5, 6, 7, 8, 9]
    studentIDPrefix = take_first_two_letters(firstName) + str(lastName)
  
    studentIDSuffix = random.choice(nums)
    studentID = studentIDPrefix + str(studentIDSuffix)
    studentID += str(studentIDSuffix)
    print(studentID) 
    return studentID


######################################################################
# confirm_student_information(): Confirms the student information with
#                                the user
# 
# see usage in handle_main_loop() & handle_reentry()                    
######################################################################
  def confirm_student_information(firstName, lastName, studentId):
    utils.clear()
    # If the setting to skip the confirmation is disabled
    if(settings.GlobalSettings.SkipBulkLoaderInfoConfirmation == 0):
      print(f"{settings.Colors.YELLOW}Are you sure you want to process the following student information?[y/n]{settings.Colors.RESET}")
      print(f"First Name: {firstName}")
      print(f"Last Name: {lastName}")
      print(f"Student ID: {studentId}")
      confirmation = input()
      if confirmation == "y" or confirmation == "Y":
       utils.clear()
       print(f"{settings.Colors.GREEN}Adding student to bulk data loader{settings.Colors.RESET}")
      #TODO  actually insert the data into the json file here
       utils.increment_stat_value("TotalStudentsProcessed")
       utils.increment_stat_value("studentsProcessedInCurrentSession")
      elif confirmation == "n" or confirmation == "N":
       handle_non_confirmation()
      else:
        utils.clear()
        print("Invalid input. Please try again")
        time.sleep(1)
        confirm_student_information(firstName, lastName, studentId)
        
        #If the setting to skip the confirmation is enabled 
    elif(settings.GlobalSettings.SkipBulkLoaderInfoConfirmation == 1):
      #  insert the data into json
        print(f"{settings.Colors.GREEN}Adding student to bulk data loader{settings.Colors.RESET}")
        utils.increment_stat_value("TotalStudentsProcessed")
        utils.increment_stat_value("studentsProcessedInCurrentSession")
       

  ######################################################################
  # handle_non_confirmation(): Handles the user's decision to not confirm
  #                            the student information
  #
  # see usage in confirm_student_information()
  ######################################################################
  def handle_non_confirmation():
    utils.clear()
    print(f"{settings.Colors.YELLOW}You have chosen not to process the following student information:{settings.ColorsRESET}")
    print(f"First Name: {Student.FirstName}")
    print(f"Last Name: {Student.LastName}")
    print(f"Student ID: {Student.StudentID}")
    print("")
    print("What would you like to do?")
    print("Please enter the corresponding number.")
    print("")
    print("1.Re-enter student information")
    print("2.Confirm this student information")

    userInput = input()
    if userInput == "1":
      handle_reentry()
    elif userInput == "2":
      confirm_student_information(Student.FirstName, Student.LastName, Student.StudentID)
    else:
      utils.clear()
      print("Invalid input. Please try again")
      time.sleep(1)
      handle_non_confirmation()

  ######################################################################
  # handle_reentry(): Handles the user's decision to re-enter the student
  #                   information
  #
  # see usage in handle_non_confirmation()
  ######################################################################
  def handle_reentry():
    utils.clear()
    print("What information would you like to re-enter?")
    print("Please enter the corresponding number.")
    print("1.First Name")
    print("2.Last Name")
    print("3.Student ID")
    userInput = input()
    if userInput == "1":
      get_student_information("first name")
      confirm_student_information(Student.FirstName, Student.LastName, Student.StudentID)
    elif userInput == "2":
      get_student_information("last name")
      confirm_student_information(Student.FirstName, Student.LastName, Student.StudentID)
    elif userInput == "3":
      get_student_information("student id")
      confirm_student_information(Student.FirstName, Student.LastName, Student.StudentID)
    else:
      utils.clear()
      print("Invalid input. Please try again")
      time.sleep(1)
      handle_reentry()


  ######################################################################
  # handle_main_loop(): Handles the main function loop for the bulk data loader
  #
  ######################################################################
  def handle_main_loop():
    print(f"{settings.Colors.YELLOW}To cancel this operation enter 'cancel'{settings.Colors.RESET}")
    get_student_information("first name")
    get_student_information("last name")
    get_student_information("student id")
    confirm_student_information(Student.FirstName, Student.LastName, Student.StudentID) 
    
    
# Todo so the problem is that I will only need to insert the objects and arrays only once. Any time after that I will only need to insert the data inside the "students" array. need to find a way to check for the existence of "data" object and all of its contents then insert the new student data into the "students" array
  # def insert_student_into_json(studentSessionNumAsStr, studentFirstName, studentLastName, studentID):
  #   try:
  #     with open(bulkLoaderDataFilePath, "a") as file:
  #       data = {"data": {
  #         "columns": ["FirstName", "LastName", "StudentID"],
  #         "students": {
  #           {studentSessionNumAsStr}: [{studentFirstName}, {studentLastName}, {studentID}]}}}

  handle_main_loop()
  # This var is used to trigger the while loop allowing constant input from the user until the user decides to exit the bulk data loader
  bulkDataLoadingCompleted = False
  while(bulkDataLoadingCompleted == False):
    print("To add another student enter 'next' or to finalize and load all data enter 'done'")
    userInput = input()
    if userInput == "next":
      utils.clear()
      handle_main_loop()  
    elif userInput == "done":
      bulkDataLoadingCompleted = True
      utils.clear()
      print(f"{settings.Colors.GREEN}Bulk data loading completed{settings.Colors.RESET}")
      utils.increment_stat_value("TotalSessionsCompleted")
      time.sleep(2)
      utils.clear()
    else:
      utils.clear()
      print("Invalid input. Please try again")
      time.sleep(1)
      bulkDataLoadingCompleted = False

if __name__ == "__main__":
  main()