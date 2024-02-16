# This small script is ran in main.c during start up. It does the following:
# 1. Ensures that the settings.config file has been update with the following settings:
#     - "SkipBulkLoaderInfoConfirmation"
#     - "SkipBulkLoaderPostEntryMenu"
# 2. 

import sys
import os
# Import the utils module from the lib/utils/python directory 
sys.path.append("../lib/utils/python")
import utils


def main():
  utils.add_settings_to_config_on_startup()


if __name__ == "__main__":
  main()