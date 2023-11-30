cmake_minimum_required(VERSION 3.24.0)
project(Test)

add_subdirectory(matplotplusplus)

# add_executable(nodeflow_test test_main.cpp)
# add_executable(nodeflow_test eval.cpp)
# add_executable(nodeflow_test test_main.cpp)
# add_executable(nodeflow_test graph.cpp)
# add_executable(nodeflow_test sub_graph.cpp)
# add_executable(nodeflow_test stream.cpp)
# add_executable(nodeflow_test getAllNode.cpp)
add_executable(nodeflow_test expression_parser.cpp)

target_link_libraries(nodeflow_test nodeflow matplot)
