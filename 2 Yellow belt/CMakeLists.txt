cmake_minimum_required(VERSION 3.21)
project(2_Yellow_belt)


set(CMAKE_CXX_STANDARD 20)

add_executable(2_Yellow_belt main.cpp
        date.cpp
        tests/test_date.cpp
#        condition_parser.cpp
#        test_condition_parser.cpp
#        node.cpp
#        token.cpp
#        database.cpp
        )

