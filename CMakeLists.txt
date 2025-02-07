cmake_minimum_required(VERSION 3.15)
project(lib)

include_directories(PUBLIC include)

add_library(ll STATIC src/ll.c)
add_library(ht STATIC src/ht.c)
add_library(uba STATIC src/uba.c)

add_executable(a.out tests/ll_test.c)
target_link_libraries(a.out ll)

option(ENABLE_TESTS "Enable tests" OFF)
message(STATUS "ENABLE_TESTS is set to: ${ENABLE_TESTS}")
if(ENABLE_TESTS)
    include(CTest)
    find_program(VALGRIND_EXECUTABLE NAMES valgrind)

    add_executable(uba_test tests/uba_test.c)
    target_link_libraries(uba_test uba)
    add_executable(ll_test tests/ll_test.c)
    target_link_libraries(ll_test ll)

    add_test(NAME test_ll COMMAND ll_test)
    add_test(NAME ll_memcheck COMMAND ${VALGRIND_EXECUTABLE}
        --leak-check=full --error-exitcode=1 ./ll_test)

    add_test(NAME test_uba COMMAND uba_test)
    add_test(NAME uba_memcheck COMMAND ${VALGRIND_EXECUTABLE}
        --leak-check=full --error-exitcode=1 ./uba_test)

    add_custom_target(tests
        COMMAND ctest --output-on-failure
        DEPENDS uba_test ll_test
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

endif()


# USAGE IN OTHER PROJECTS

#add_subdirectory(lib)
#add_executable(a.out main.c)
#target_link_libraries(a.out PRIVATE ll)

# mkdir build && cd build && cmake .. && cmake --build .
