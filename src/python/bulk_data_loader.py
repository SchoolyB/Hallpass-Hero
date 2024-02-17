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

# Import the utils module from the lib/utils/python directory 
sys.path.append("../lib/utils/python")
import settings
import utils

bulkLoaderDataFilePath = "../../build/data/active_data.json"


     
class Student:
  FirstName = None
  LastName = None
  StudentID = None

def main():
  # Clear the instructions from the terminal
  utils.clear()
  # The current session number of the bulk data loader
  totalSessionsCompleted = None
  # The total number of students ever uploaded to the system
  totalStudentsProcessed = None


  # increments the total session number by one
  def update_bulk_data_loader_session():
      bulkDataLoaderSessionNumber = totalSessionsCompleted 
      bulkDataLoaderSessionNumber += 1
      return bulkDataLoaderSessionNumber

  # increments the total students processed number by one
  def update_total_student_number():
      bulkDataLoaderTotalStudentNumber = totalStudentsProcessed
      bulkDataLoaderTotalStudentNumber += 1
      return bulkDataLoaderTotalStudentNumber

  # increments the student number of the current session by one
  def session_student_iota():
      bulkDataLoaderSessionStudentNumber = 0
      bulkDataLoaderSessionStudentNumber += 1
      return bulkDataLoaderSessionStudentNumber

  # checks the total number of students processed from the stats.log file and returns it  
  def get_total_students_processed():
      retrievedTotalStudentsProcessed = None
      with open(utils.statsLogFilePath, "r") as file:
          for line in file:
              key, value = map(str.strip, line.split("=", 1))
              if key == "TotalStudentsProcessed":
                  retrievedTotalStudentsProcessed = value
                  return retrievedTotalStudentsProcessed
          else:
              utils.__utils_error_logger("Error: Could not find TotalStudentsProcessed key in stats.log", "check_total_student_number", utils.ErrorLevel.MODERATE)

  # checks the total number of sessions completed from the stats.log file and returns it
  # def get_total_sessions_completed():
  #   with open(utils.statsLogFilePath, "r") as file:
  #      for line in file:
  #         key, value = map(str.strip, line.split("=", 1))
  #         if key == "TotalSessionsCompleted":
  #             retrievedTotalSessionsCompleted = value
  #             return retrievedTotalSessionsCompleted
  #         else:
  #             utils.__utils_error_logger("Error: Could not find TotalSessionsCompleted key in stats.log", "check_total_sessions_completed", utils.ErrorLevel.MODERATE)
  
  
  # try:
  #   with open("../../logs/stats.log", "a") as file:
  #     # First when running the bulk data loader script is to get the current stats from the stats.log file
  #     totalStudentsProcessed = get_total_students_processed()
  #     totalSessionsCompleted = get_total_sessions_completed()
  # except FileNotFoundError as error:
  #   print(f"File Not Found: {utils.statsLogFilePath}")

      # Second is begin the bulk data loader process
  def get_student_information(type):
    # Get the first name
    if type == "first name":
      userInput = input(f"Enter the student's {type}: \n")
      hasNonSpaceChar = utils.has_one_non_space_char(userInput)
      if(hasNonSpaceChar == False):
        print(f"{settings.Colors.RED}The student's first name cannot be empty{settings.Colors.RESET}")
        time.sleep(2)
        utils.clear()
        get_student_information("first name")
      elif (hasNonSpaceChar == True):
        if userInput == "cancel":
          utils.__utils_operation_cancelled("get_student_information")
        else:
          Student.FirstName = userInput
          
        
    # Get the last name 
    elif type == "last name":
      utils.clear()
      print(f"First Name: {Student.FirstName}")
      print("Enter the student's last name")
      print("If the student doesn't have a last name enter 'none'")
      userInput = input(f"Enter the student's {type}: \n")
      hasNonSpaceChar = utils.has_one_non_space_char(userInput)
      if(hasNonSpaceChar == False):
        print(f"{settings.Colors.RED}The student's last name cannot be empty{settings.Colors.RESET}")
        time.sleep(2)
        utils.clear()
        get_student_information("last name")
      elif (hasNonSpaceChar == True):
        if userInput == "cancel":
          utils.__utils_operation_cancelled("get_student_information")
        if(userInput == "none"):
           print(f"{settings.Colors.YELLOW}The student's last name will be set to 'none'{settings.Colors.RESET}")
           Student.LastName = "none"
        else:
           Student.LastName = userInput
    # Get the ID 
    #todo unfortunately I get the automatic generation of the student id is done in the C code so I will have to basically create a new function in Python to generate the student id....  or I can store the student id from the c code in a memory address and then retrieve it from the python code
    elif type == "student id":
      utils.clear()
      print(f"First Name: {Student.FirstName}")
      print(f"Last Name: {Student.LastName}")
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
        Student.StudentID = userInput
    
  def confirm_student_information(firstName, lastName, studentId):
    utils.clear()
    # If the setting to skip the confirmation is disabled
    if(settings.GlobalSettings.SkipBulkLoaderInfoConfirmation == 0):
      print(f"Are you sure you want to process the following student information?[y/n]")
      print(f"First Name: {firstName}")
      print(f"Last Name: {lastName}")
      print(f"Student ID: {studentId}")
      confirmation = input()
      if confirmation == "y" or confirmation == "Y":
        #  insert the data into json
       print(f"{settings.Colors.GREEN}Adding student to bulk data loader{settings.Colors.RESET}")
       pass #todo remove this line only here to prevent an error for now
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
       pass #todo remove this line only here to prevent an error for now
  
  def handle_non_confirmation():
    utils.clear()
    print(f"{settings.Colors.YELLOW}You have chosen not to process the following student information:{settings.Colors.RESET}")
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


  print(f"{settings.Colors.YELLOW}To cancel this operation enter 'cancel'{settings.Colors.RESET}")
  get_student_information("first name")
  get_student_information("last name")
  get_student_information("student id")
  confirm_student_information(Student.FirstName, Student.LastName, Student.StudentID) 


if __name__ == "__main__":
  main()