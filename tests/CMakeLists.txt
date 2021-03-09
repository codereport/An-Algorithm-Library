include(cmake/compiler_warnings.cmake)
include(cmake/ut.cmake)
add_executable(${PROJECT_NAME}_algorithm_test
        src/algorithm_test.cpp)
set_project_warnings(${PROJECT_NAME}_algorithm_test ON)
target_link_libraries(${PROJECT_NAME}_algorithm_test
        PRIVATE Boost::ut
        PRIVATE ${PROJECT_NAME}_aal)
set_target_properties(${PROJECT_NAME}_algorithm_test PROPERTIES OUTPUT_NAME "algorithm_test")
add_test(
        NAME ${PROJECT_NAME}_ALGORITHM_TEST COMMAND ${PROJECT_NAME}_algorithm_test
)