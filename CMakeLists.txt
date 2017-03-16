cmake_minimum_required(VERSION 2.8)

project (Hexapod)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE Hexapod_SOURCES "src/*.cpp")
file(GLOB_RECURSE Hexapod_HEADERS "src/*.h")

set (Hexapod_INCLUDE_DIRS "")
foreach (_headerFile ${Hexapod_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Hexapod_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES Hexapod_INCLUDE_DIRS)

find_package(Boost 1.36.0 COMPONENTS date_time filesystem system)
find_package(Curses REQUIRED)

if(Boost_FOUND)
	include_directories(${Hexapod_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${CURSES_INCLUDE_DIR})
	add_executable(Hexapod ${Hexapod_SOURCES})
	target_link_libraries(Hexapod ${Boost_LIBRARIES} ${CURSES_LIBRARIES} pthread)
endif()



# target_compile_features(Hexapod PRIVATE cxx_range_for)
