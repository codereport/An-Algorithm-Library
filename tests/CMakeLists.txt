include(cmake/compiler_warnings.cmake)
include(cmake/ut.cmake)
add_executable(${PROJECT_NAME}_algorithm_test
        src/algorithm_test.cpp)
set_project_warnings(${PROJECT_NAME}_algorithm_test ON)
target_link_libraries(${PROJECT_NAME}_algorithm_test PRIVATE Boost::ut ${PROJECT_NAME}_aal)
add_test(${PROJECT_NAME}_ALGORITHM_TEST ${PROJECT_NAME}_algorithm_test)