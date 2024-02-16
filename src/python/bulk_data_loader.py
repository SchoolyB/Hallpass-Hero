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


     
class Student:
  FirstName = None
  LastName = None
  StudentID = None

def main():

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
      print(f"{settings.Colors.YELLOW}To cancel this operation enter 'cancel'{settings.Colors.RESET}")
      userInput = input(f"Enter the student's {type}: \n")
      if userInput == "cancel":
        utils.__utils_operation_cancelled("get_student_information")
      else:
        Student.FirstName = userInput
        print(f"You entered {userInput}")
        
    # Get the last name 
    elif type == "last name":
      print(f"{settings.Colors.YELLOW}To cancel this operation enter 'cancel'{settings.Colors.RESET}")
      print("If the student doesn't have a last name enter 'none'")
      userInput = input(f"Enter the student's {type}: \n")
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
      print(f"{settings.Colors.YELLOW}To cancel this operation enter 'cancel'{settings.Colors.RESET}")
      userInput = input(f"Enter the student's {type}: \n")
      if userInput == "cancel":
        utils.__utils_operation_cancelled("get_student_information")
      Student.StudentID = userInput
    
  def confirm_student_information(firstName, lastName, studentId):
    print(f"Are you sure you want to process the following student information?[y/n]")
    print(f"First Name: {firstName}")
    print(f"Last Name: {lastName}")
    print(f"Student ID: {studentId}")
    confirmation = input()
    if confirmation == "y" or confirmation == "Y":
     return True
    elif confirmation == "n" or confirmation == "N":
     return False
    else:
      utils.clear()
      print("Invalid input. Please try again")
      time.sleep(1)
      confirm_student_information(firstName, lastName, studentId)
      
  get_student_information("first name")
  get_student_information("last name")
  get_student_information("student id")
  confirm_student_information(Student.FirstName, Student.LastName, Student.StudentID) 


if __name__ == "__main__":
  main()