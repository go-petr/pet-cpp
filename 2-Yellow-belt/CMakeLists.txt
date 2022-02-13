cmake_minimum_required(VERSION 3.21)
project(2_Yellow_belt)


set(CMAKE_CXX_STANDARD 17)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(2_Yellow_belt main.cpp
        date.cpp
        test_date.cpp
        condition_parser.cpp
        test_condition_parser.cpp
        node.cpp
        token.cpp
        database.cpp
        test_database.cpp
        )

