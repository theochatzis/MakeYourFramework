cmake_minimum_required(VERSION 3.16)
project(MakeYourFramework CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

find_package(ROOT REQUIRED COMPONENTS Tree Hist RIO MathCore)
find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
execute_process(COMMAND ${Python_EXECUTABLE} -m pybind11 --cmakedir
                OUTPUT_VARIABLE pybind11_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
find_package(pybind11 CONFIG REQUIRED)

# libmyf: data formats (header only), core and modules
file(GLOB CORE_SOURCES Core/src/*.cc)
file(GLOB MODULE_SOURCES Modules/*.cc)
add_library(myf SHARED ${CORE_SOURCES} ${MODULE_SOURCES})
target_include_directories(myf PUBLIC DataFormats/include Core/include)
target_link_libraries(myf PUBLIC ROOT::Tree ROOT::TreePlayer ROOT::Hist ROOT::RIO ROOT::MathCore)
target_compile_options(myf PRIVATE -Wall -Wextra)

# the python module, written next to the python package so that "import myf" finds it
pybind11_add_module(_myf bindings/PyMyf.cc)
target_link_libraries(_myf PRIVATE myf)
set_target_properties(_myf PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/python/myf)
