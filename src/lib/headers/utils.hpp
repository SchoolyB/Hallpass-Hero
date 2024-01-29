/*
===============================================================================
File Name   : utils.hpp
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This header file contains the utility C++ function prototypes
              and enum classes used in the program.
-------------------------------------------------------------------------------
Notes       : This uses extern "C" linkage to allow the C++ functions to be
              called from C code.
===============================================================================
*/
#ifndef UTILS_HPP
#define UTILS_HPP

enum class CppErrorLevel
{
  MINOR,
  MODERATE,
  CRITICAL
};

int cpp_utils_error_logger(const char *message, const char *function, const CppErrorLevel level);
void CPP_UTILS_SLEEP(int seconds);
#endif