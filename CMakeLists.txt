# -------------------------------------------

cmake_minimum_required(VERSION 3.21)

# -------------------------------------------

project(formal_languages_task
        VERSION 1.0.0
        DESCRIPTION "Good job"
        )

# -------------------------------------------
# Some basic settings
# -------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package(GTest REQUIRED)

# -------------------------------------------
# Flags
# -------------------------------------------

SET(GCC_COVERAGE_COMPILE_FLAGS "-O2 -g -fsanitize=address,undefined")
SET(CMAKE_CXX_FLAGS  "${GCC_COVERAGE_COMPILE_FLAGS}")

# -------------------------------------------
# Shared libraries
# -------------------------------------------

add_subdirectory(machine)

add_subdirectory(regular)
add_dependencies(regular machine)
target_link_libraries(regular machine)

# -------------------------------------------
# Executable tests
# -------------------------------------------

set(TEST_SOURCES
        # -------------------------------------------
        # Test cases
        # -------------------------------------------

        tests/basic_test.cpp

        tests/test_helper.cpp

        tests/deterministic_test.cpp
        
        tests/operation_test.cpp

        # -------------------------------------------
        # Main File
        # -------------------------------------------

        test_main.cpp

)
add_executable(test_main ${TEST_SOURCES})
add_dependencies(test_main machine regular)
target_link_libraries(
        test_main
        GTest::gtest
        GTest::gtest_main
        machine
        regular
)

# -------------------------------------------
# Executable main
# -------------------------------------------

add_executable(main main.cpp)
add_dependencies(main regular)
target_link_libraries(
        main
        regular
)

# -------------------------------------------
# Install program
# -------------------------------------------

set(INSTALL_TARGETS
        main
        machine
        regular
)

install(
        TARGETS ${INSTALL_TARGETS}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)

# -------------------------------------------
