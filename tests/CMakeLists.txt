include(cmake/compiler_warnings.cmake)
add_library(${PROJECT_NAME}_project_warnings INTERFACE)
set_project_warnings(${PROJECT_NAME}_project_warnings ON)
include(cmake/ut.cmake)
add_executable(${PROJECT_NAME}_algorithm_test
        src/algorithm_test.cpp)
target_link_libraries(${PROJECT_NAME}_algorithm_test
        PUBLIC Boost::ut
        PRIVATE ${PROJECT_NAME}_aal
        PRIVATE ${PROJECT_NAME}_project_warnings)
set_target_properties(${PROJECT_NAME}_algorithm_test PROPERTIES OUTPUT_NAME "algorithm_test")
add_test(
        NAME ${PROJECT_NAME}_ALGORITHM_TEST COMMAND ${PROJECT_NAME}_algorithm_test
)