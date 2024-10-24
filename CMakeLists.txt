cmake_minimum_required(VERSION 3.17)
project(test)

set(CMAKE_CXX_STANDARD 23)

file(GLOB APP_SOURCES */*.cpp)
foreach (testsourcefile ${APP_SOURCES})
    get_filename_component(testname ${testsourcefile} NAME_WLE)
    get_filename_component(dirname ${testsourcefile} DIRECTORY)
    file(GLOB dir_src CONFIGURE_DEPENDS
            "${dirname}/*.hpp"
            "${dirname}/*.cpp"
            )
    message("${testname}.cpp | ${dir_src}")
    add_executable("${testname}.cpp" ${dir_src})
endforeach (testsourcefile ${APP_SOURCES})