
set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(httpserver httpserver.cpp)
target_link_libraries(httpserver pthread)
