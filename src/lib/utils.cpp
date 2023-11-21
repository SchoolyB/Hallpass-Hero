/*
===============================================================================
File Name   : utils.cpp
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains several utility functions that are used
              throughout the 'C++' portion of the source code. This file also
              contains the typedefs for the structs used in the program.
              TODO might move the funcs to the header file
===============================================================================
*/

#include <iostream>
#include <fstream>
#include "utils.hpp"
/*
Allows using elements from
the 'std' namespace without
prefixing them all with 'std::'
*/
using namespace std;

int CPP_UTILS_ERROR_LOGGER(const char *message, const char *function, const CppErrorLevel level)
{
  fstream errorLog;
  errorLog.open("../logs/errors.log", fstream::app);

  switch (level)
  {
  case CppErrorLevel::MINOR:
    errorLog << "Minor Error: " << message << " in function " << function << endl;
    errorLog << "=============================================================" << endl;
    break;
  case CppErrorLevel::MODERATE:
    errorLog << "Moderate Error: " << message << " in function " << function << endl;
    errorLog << "=============================================================" << endl;
    break;
  case CppErrorLevel::CRITICAL:
    errorLog << "=============================================================" << endl;
    errorLog << "CRITICAL ERROR: " << message << " in function " << function << endl;
    break;
  default:
    break;
  }
  return 0;
}
