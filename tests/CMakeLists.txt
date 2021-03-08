include(ut.cmake)


add_executable(${PROJECT_NAME}_algorithm_test
        algorithm_test.cpp)
target_link_libraries(${PROJECT_NAME}_algorithm_test PRIVATE Boost::ut ${PROJECT_NAME}_aal)
add_test(${PROJECT_NAME}_ALGORITHM_TEST ${PROJECT_NAME}_algorithm_test)