/*
===============================================================================
File Name   : utils.cpp
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains several utility functions that are used
              throughout the 'C++' portion of the source code. This file also
              contains the typedefs for the structs used in the program.
===============================================================================
*/

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <ctime>
#include "../headers/cpp/cpp_files.hpp"
/*
Allows using elements from
the 'std' namespace without
prefixing them all with 'std::'
*/
using namespace std;

/************************************************************************************
 * cpp_utils_error_logger(): Logs errors errors that occur specifically in 'C++' files
 * Note: For 'C' code error logging see __utils_error_logger in utils.c
 ************************************************************************************/
int cpp_utils_error_logger(const char *message, const char *function, const CppErrorLevel level)
{
  fstream errorLog;

  // Get the current time
  time_t currentTime = time(nullptr);

  // Convert the time to local time
  tm *localTime = localtime(&currentTime);

  // Format and print the local time
  char timeString[100]; // Buffer to hold the formatted time string
  strftime(timeString, sizeof(timeString), "%m-%d-%Y %H:%M:%S", localTime);

  errorLog.open("../logs/errors.log", fstream::app);
  switch (level)
  {
  case CppErrorLevel::MINOR:
    errorLog << "From C++ Logged @ " << timeString << endl;
    errorLog << "Minor Error: " << message << " in function call " << function << "()" << endl;
    errorLog << "=============================================================" << endl;
    break;
  case CppErrorLevel::MODERATE:
    errorLog << "From C++ Logged @ " << timeString << endl;
    errorLog << "Moderate Error: " << message << " in function call " << function << "()" << endl;
    errorLog << "=============================================================" << endl;
    break;
  case CppErrorLevel::CRITICAL:
    errorLog << "From C++ Logged @ " << timeString << endl;
    errorLog << "=============================================================" << endl;
    errorLog << "CRITICAL ERROR: " << message << " in function call " << function << "()" << endl;
    break;
  default:
    break;
  }
  return 0;
}
/************************************************************************************
 * CPP_UTILS_SLEEP(): Can you believe I hade to make a sleep function? C++ is lame.
 ************************************************************************************/
void CPP_UTILS_SLEEP(int seconds)
{
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
}
