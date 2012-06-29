set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project(cpu)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)


add_executable(cpu cpu.cpp)
add_executable(asm assembler.cpp)

Find_Package(Boost 1.49 COMPONENTS regex REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
if ( NOT Boost_FOUND )
    message(FATAL_ERROR "Boost not found !")
endif ( NOT Boost_FOUND )

target_link_libraries(cpu)

target_link_libraries(
    asm
    ${Boost_LIBRARY}
    ${Boost_REGEX_LIBRARY}
)

ADD_DEFINITIONS("-O3")
ADD_DEFINITIONS("-std=c++0x")
