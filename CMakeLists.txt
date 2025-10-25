cmake_minimum_required(VERSION 3.10)
project(JobPortal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    main_crow.cpp
    c++ code/job_portal.cpp
    c++ code/Trie.cpp
    c++ code/candidate.cpp
)

# Create executable
add_executable(job_portal_server ${SOURCES})

# Link libraries
target_link_libraries(job_portal_server 
    Threads::Threads
)

# Copy HTML file to build directory
configure_file(${CMAKE_SOURCE_DIR}/index.html 
               ${CMAKE_BINARY_DIR}/templates/index.html 
               COPYONLY)

# Print instructions
message(STATUS "==============================================")
message(STATUS "Job Portal - Data Structures Project")
message(STATUS "==============================================")
message(STATUS "Build with: cmake .. && make")
message(STATUS "Run with: ./job_portal_server")
message(STATUS "Access at: http://localhost:8080")
message(STATUS "==============================================")