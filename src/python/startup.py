#===============================================================================
#File Name   : startup.py
#-------------------------------------------------------------------------------
#Author      : Marshall Burns a.k.a. Schooly
#-------------------------------------------------------------------------------
#Description : This small script is ran in main.c during start up.
#              It does the following:
#1. Ensures that the settings.config file has been update with the following settings:
#    - "SkipBulkLoaderInfoConfirmation"
#    - "SkipBulkLoaderPostEntryMenu"
#
#2.  
#             
#             
#===============================================================================
# TODO might not run this from main.c,. might run it from the bulk_data_loader.py file
import sys
import os
# Import the utils module from the lib/utils/python directory 
sys.path.append("../lib/utils/python")
import utils
import settings
import bulk_data_loader


def main():
  settings.main()
  bulk_data_loader.main()




if __name__ == "__main__":
  main()