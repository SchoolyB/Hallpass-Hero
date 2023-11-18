cmake_minimum_required(VERSION 3.10)

# Project name
project(Hallpass-Hero)

# Find SQLite3 package
find_package(SQLite3 REQUIRED)

# Include SQLite3 headers
include_directories(${SQLite3_INCLUDE_DIRS})


set(SOURCES_C
    src/c/main.c
    src/c/_add_student.c
    src/c/_create_roster.c
    src/c/_manage_roster.c
    src/c/_manage_student_db.c
    src/c/_search.c
    src/db/db.c
    src/lib/utils.c
)

set(SOURCES_CPP)

find_package(SQLite3 REQUIRED)
# Create an executable from the C and C++ sources

add_executable(HallpassHero.exe ${SOURCES_C} ${SOURCES_CPP})
target_include_directories(HallpassHero.exe PRIVATE ${CMAKE_SOURCE_DIR}src/lib/headers)
target_link_libraries(HallpassHero.exe PRIVATE ${SQLite3_LIBRARIES})
set(CPACK_PACKAGE_NAME "HallpassHero")
set(CPACK_GENERATOR "ZIP")  # or other generators like DEB, RPM
include(CPack)

install(TARGETS HallpassHero.exe DESTINATION ~/Downloads/HallpassHero)
