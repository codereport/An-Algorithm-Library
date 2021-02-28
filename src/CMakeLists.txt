# Interface library is great if this is a header only library.
add_library(aal INTERFACE)

# add this directory to the include search path:
target_include_directories(aal INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# use target_link_libraries(... aal) to add this library to your existing project.