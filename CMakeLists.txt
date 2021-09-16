cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_STANDARD 17)

project (StdBot)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

link_directories(/usr/local/lib/ ${CMAKE_SOURCE_DIR}/libs/spdlog)

add_executable(StdBot src/StdBot.cpp)

set(CMAKE_BUILD_TYPE Debug)

target_link_libraries(StdBot OpenSSL::SSL Threads::Threads)

target_compile_options(StdBot PRIVATE
	-Wall
    -Wextra # reasonable and standard
    -Wshadow # warn the user if a variable declaration shadows one from a parent context
    -Wnon-virtual-dtor # warn the user if a class with virtual functions has a
                         # non-virtual destructor. This helps catch hard to track down memory errors
    -Wold-style-cast # warn for c-style casts
    -Wcast-align # warn for potential performance problem casts
    -Wunused # warn on anything being unused
    -Woverloaded-virtual # warn if you overload (not override) a virtual function
    -Wpedantic # warn if non-standard C++ is used
    -Wconversion # warn on type conversions that may lose data
    -Wnull-dereference # warn if a null dereference is detected
    -Wdouble-promotion # warn if float is implicit promoted to double
    -Wformat=2 # warn on security issues around functions that format output (ie printf)
    -Wmisleading-indentation # warn if indentation implies blocks where blocks do not exist
    -Wduplicated-cond # warn if if / else chain has duplicated conditions
    -Wduplicated-branches # warn if if / else branches have duplicated code
    -Wlogical-op # warn about logical operations being used where bitwise were probably wanted
    -Wuseless-cast # warn if you perform a cast to the same type
    
    -O0 -g3 -ggdb
)

target_include_directories(StdBot PUBLIC
	${CMAKE_SOURCE_DIR}/libs
	${CMAKE_SOURCE_DIR} 
)

target_precompile_headers(StdBot PRIVATE
	${CMAKE_SOURCE_DIR}/libs/httplib.h
	${CMAKE_SOURCE_DIR}/libs/json.hpp
	${CMAKE_SOURCE_DIR}/libs/spdlog/spdlog.h
)