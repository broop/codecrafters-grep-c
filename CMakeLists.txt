cmake_minimum_required(VERSION 3.13)

project(codecrafters-grep)

set(CMAKE_C_STANDARD 23) # Enable the C23 standard

# Main grep executable
add_executable(grep src/main.c src/matcher.c)

# Test executable
enable_testing()
add_executable(test_main src/test_main.c src/matcher.c)
add_test(NAME regex_tests COMMAND test_main)