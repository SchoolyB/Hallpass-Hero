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
import csv
import time
from enum import Enum
import time
import settings
import random
import sqlite3

# Import the utils module from the lib/utils/python directory 
sys.path.append("../lib/utils/python")
import utils

bulkLoaderDataFilePath = "../../build/data/active_data.csv"
     
class Student:
  FirstName = None
  TruncatedLastName = None
  LastName = None
  StudentID = None


class Statistic:
  TotalStudentsProcessed = None
  TotalSessionsCompleted = None
  CurrentSessionNumber = None
  StudentsProcessedInCurrentSession = None
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
          else:
            Student.StudentID = userInput




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
  
    studentIDSuffix = ''.join(map(str, random.sample(nums, 3)))
    studentID = studentIDPrefix + studentIDSuffix
    
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
    print(f"Checking status of skip bulk loader info confirmation in settings...")
    if(settings.GlobalSettings.SkipBulkLoaderInfoConfirmation == 0):
      print(f"{settings.Colors.RED}Skip bulk loader info confirmation is disabled{settings.Colors.RESET}")
      print(f"{settings.Colors.YELLOW}Are you sure you want to process the following student information?[y/n]{settings.Colors.RESET}")
      print(f"First Name: {firstName}")
      print(f"Last Name: {lastName}")
      print(f"Student ID: {studentId}")
      confirmation = input()
      if confirmation == "y" or confirmation == "Y":
       utils.clear()
       print(f"{settings.Colors.GREEN}Adding student to bulk data loader{settings.Colors.RESET}")
       insert_data_into_csv(Student.FirstName, Student.LastName, Student.StudentID)
       time.sleep(.5)
       print(f"{settings.Colors.GREEN}Student successfully added to bulk data loader{settings.Colors.RESET}")
       utils.clear()
       utils.increment_stat_value("TotalStudentsProcessed")
       utils.increment_stat_value("StudentsProcessedInCurrentSession")
      elif confirmation == "n" or confirmation == "N":
       handle_non_confirmation()
      else:
        utils.clear()
        print("Invalid input. Please try again")
        time.sleep(1)
        confirm_student_information(firstName, lastName, studentId)
        
        #If the setting to skip the confirmation is enabled 
    elif(settings.GlobalSettings.SkipBulkLoaderInfoConfirmation == 1):
      #  insert the data into csv
        print(f"{settings.Colors.GREEN}Skip bulk loader info confirmation is enabled{settings.Colors.RESET}")
        print(f"{settings.Colors.GREEN}Adding student to bulk data loader{settings.Colors.RESET}")
        utils.increment_stat_value("TotalStudentsProcessed")
        utils.increment_stat_value("StudentsProcessedInCurrentSession")
       

  ######################################################################
  # handle_non_confirmation(): Handles the user's decision to not confirm
  #                            the student information
  #
  # see usage in confirm_student_information()
  ######################################################################
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
  # insert_data_into_csv(): Inserts the passed in data into the csv file
  #
  # see usage in confirm_student_information()
  ######################################################################
  def insert_data_into_csv(firstName,lastName,studentID):
    with open(bulkLoaderDataFilePath, "a") as file:
      writer = csv.writer(file)
      writer.writerow([firstName, lastName, studentID])


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
    
    

  ######################################################################
  # bulk_load_students_into_db(): Loads all student data in active_data.csv
  #                               into the sqlite.db
  #
  ######################################################################
  def bulk_load_students_into_db():
    connection = sqlite3.connect("../../build/db.sqlite")
    cursor = connection.cursor()
    with open(bulkLoaderDataFilePath, "r") as file:
      reader = csv.reader(file)
      for row in reader:
        cursor.execute("INSERT INTO students (FirstName, LastName, StudentID) VALUES (?, ?, ?)", (row[0], row[1], row[2]))
    connection.commit()
    connection.close()
    

  ######################################################################
  # check_csv_column_in_table(): Checks if the student ID in the csv file
  #                              is already in the database, if it is remove
  #                              the duplicate from the csv file
  # 
  # see usage in check_and_handle_duplicate_id()
  ######################################################################
  def check_csv_column_in_table():
    conn = sqlite3.connect("../../build/db.sqlite")
    cursor = conn.cursor()
    with open("../../build/data/active_data.csv", 'r') as csvFile:
        csvReader = csv.reader(csvFile)
        header = next(csvReader)  # Skip the header row

        # Find the index of the column to check
        columnIndex = header.index("StudentID")

        # Iterate through rows in the CSV file
        for row in csvReader:
            query = f"SELECT * FROM students WHERE StudentID = ?"
            # Execute query
            cursor.execute(query, (row[columnIndex],))

            # Check if the data is present in the SQLite table
            result = cursor.fetchone()
            if result:
              # if the data is present in the SQLite table, remove the row from the CSV file
              remove_duplicates_from_csv()
              return True
            else:
              return False
    conn.close()


  ######################################################################
  # check_and_handle_duplicate_id(): Handles the logic for checking for
  #                                  duplicate student IDs in the csv file
  #
  #                                  
  ######################################################################
  def check_and_handle_duplicate_id():
    utils.clear()
    if(check_csv_column_in_table() == True):
          print("Please update or regenerate the students ID.")
          print("Enter 'ok' to continue")
          print(f"{settings.Colors.YELLOW}Enter 'cancel' to cancel this operation{settings.Colors.RESET}")
          userInput = input()
          if userInput == "ok":
            remove_duplicates_from_csv()
          elif userInput == "cancel":
            utils.__utils_operation_cancelled("bulk_data_loader")
            exit()
          else:
            utils.clear()
            print("Invalid input. Please try again")
            time.sleep(1)
            check_and_handle_duplicate_id()

  ######################################################################
  # remove_duplicates_from_csv(): Deletes all duplicate student IDs from the
  #                               csv file
  #
  # see usage in check_and_handle_duplicate_id()
  ######################################################################
  def remove_duplicates_from_csv():
    conn = sqlite3.connect("../../build/db.sqlite")
    cursor = conn.cursor()

    # Read the CSV file into a list of dictionaries
    with open("../../build/data/active_data.csv", 'r') as csvFile:
        csvReader = csv.DictReader(csvFile)
        csvData = list(csvReader)

    # Check if the specified column exists in the CSV file
    if "StudentID" not in csvData[0]:
        print(f"Error: Column 'StudentID' not found in the CSV file.")
        return

    # Find the index of the column to check
    columnIndex = csvData[0]["StudentID"]

    # Create a set to store unique values of the specified column from the CSV file
    csvValueSet = set()

    # Iterate through rows in the CSV file
    for row in csvData:
        csvValue = row["StudentID"]

        # Check if the value is already in the set (duplicate found)
        if csvValue in csvValueSet:
            print(f"Duplicate found in the CSV: {row}")
        else:
            csvValueSet.add(csvValue)

        # Execute a query to check if the value is present in the database
        query = f"SELECT 1 FROM students WHERE StudentID = ?"
        cursor.execute(query, (csvValue,))
        db_result = cursor.fetchone()

        # If the value is found in the database, remove the row from the CSV list
        if db_result:
            print(f"{settings.Colors.RED}Data also found in the database. Removing the line from the CSV: {row}{settings.Colors.RESET}")
            csvData.remove(row)

    # Write the updated data back to the CSV file
    with open("../../build/data/active_data.csv", 'w', newline='') as csvFile:
        csv_writer = csv.DictWriter(csvFile, fieldnames=csvData[0])
        csv_writer.writeheader()
        csv_writer.writerows(csvData)
    conn.close()



  handle_main_loop()
  # This var is used to trigger the while loop allowing constant input from the user until the user decides to exit the bulk data loader
  bulkDataLoadingCompleted = False
  while(bulkDataLoadingCompleted == False):
    if(settings.GlobalSettings.SkipBulkLoaderPostEntryMenu == 0):
      print("To add another student enter 'next' or to finalize and load all data enter 'done'")
      userInput = input()
      if userInput == "next":
        utils.clear()
        handle_main_loop()  
      elif userInput == "done":
        bulkDataLoadingCompleted = True
        utils.clear()
        check_and_handle_duplicate_id() 
        bulk_load_students_into_db()
        print(f"{settings.Colors.GREEN}Bulk data loading completed{settings.Colors.RESET}")
        utils.increment_stat_value("TotalSessionsCompleted")
        time.sleep(2)
        utils.clear()
      else:
        utils.clear()
        print("Invalid input. Please try again")
        time.sleep(1)
        bulkDataLoadingCompleted = False
    elif(settings.GlobalSettings.SkipBulkLoaderPostEntryMenu == 1):
      print("Checking status of skip bulk loader post entry menu in settings...")
      print(f"{settings.Colors.GREEN}Skip bulk loader post entry menu is enabled{settings.Colors.RESET}")
      handle_main_loop()
if __name__ == "__main__":
  main()