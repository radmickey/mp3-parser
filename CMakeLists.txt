cmake_minimum_required(VERSION 3.14)

project(
        mp3_Parser
        VERSION 1.0
        DESCRIPTION "MP3 ID3v2.4 parser in C++"
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
add_subdirectory(bin)
