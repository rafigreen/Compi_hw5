cmake_minimum_required(VERSION 3.22)
project(COMPI_HW5)

set(CMAKE_CXX_STANDARD 17)

set_source_files_properties(lex.yy.c PROPERTIES LANGUAGE CXX)
add_executable(COMPI_HW5
        hw3_output.cpp type.cpp src.cpp gen.cpp parser.tab.cpp lex.yy.c)
