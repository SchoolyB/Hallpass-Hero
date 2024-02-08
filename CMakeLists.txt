cmake_minimum_required(VERSION 3.10)

# Project name
project(Hallpass-Hero)

# Find SQLite3 package
find_package(SQLite3 REQUIRED)

# Include SQLite3 headers
include_directories(${SQLite3_INCLUDE_DIRS})

# Source files
set(SOURCES_C
    src/c/main.c
    src/c/_add_student.c
    src/c/_bulk_data_loader.c
    src/c/_create_roster.c
    src/c/_manage_roster.c
    src/c/_manage_student_db.c
    src/c/_search.c
    src/c/_settings.c
    src/lib/utils.c
)

set(SOURCES_CPP
    src/cpp/db.cpp
    src/lib/utils.cpp
)

# C++ source file library
add_library(cppLib OBJECT ${SOURCES_CPP})

# C source file executable
add_executable(HallpassHero.exe ${SOURCES_C} $<TARGET_OBJECTS:cppLib>)

# Specify include directories for the executable
target_include_directories(HallpassHero.exe PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/headers ${CMAKE_SOURCE_DIR}/src/lib/Cuazar/lib)

# Link against SQLite library
target_link_libraries(HallpassHero.exe PRIVATE ${SQLite3_LIBRARIES})

# Set up CPack for packaging
set(CPACK_PACKAGE_NAME "HallpassHero")
set(CPACK_GENERATOR "ZIP")  # or other generators like DEB, RPM
include(CPack)

# Install the executable
install(TARGETS HallpassHero.exe DESTINATION ~/Downloads/HallpassHero)
