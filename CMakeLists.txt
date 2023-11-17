cmake_minimum_required(VERSION 3.10)

# Project name
project(Hallpass-Hero)

set(SOURCES_C
    src/c/main.c
)

set(SOURCES_CPP)

# Create an executable from the C and C++ sources
add_executable(HallpassHero.exe ${SOURCES_C} ${SOURCES_CPP})


set(CPACK_PACKAGE_NAME "HallpassHero")
set(CPACK_GENERATOR "ZIP")  # or other generators like DEB, RPM
include(CPack)

install(TARGETS HallpassHero.exe DESTINATION ~/Downloads/HallpassHero)
