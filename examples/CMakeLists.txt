project(examples)
cmake_minimum_required(VERSION 2.8)
INCLUDE_DIRECTORIES(../src)

add_executable(${PROJECT_NAME}-reciver reciver.cpp)
target_link_libraries(${PROJECT_NAME}-reciver kooy)

add_executable(${PROJECT_NAME}-sender sender.cpp)
target_link_libraries(${PROJECT_NAME}-sender kooy)
