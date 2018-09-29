cmake_minimum_required(VERSION 3.0)

project (daedalus)

add_definitions(-DCURL_STATICLIB)

file(GLOB_RECURSE daedalus_SOURCES "src/*.cpp")
file(GLOB_RECURSE daedalus_HEADERS "src/*.h")

set (CMAKE_CXX_STANDARD 17)

set (daedalus_INCLUDE_DIRS "")
foreach (_headerFile ${daedalus_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND daedalus_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES daedalus_INCLUDE_DIRS)

set(CURL_LIBRARY "-lcurl")
set(CURL_STATICLIB true)
find_package(CURL REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.66 REQUIRED COMPONENTS system)
find_package (Threads)

set (LIB_DIR_LOCATION ${NAME}/lib)
find_library(SQLite3 NAMES sqlite3 PATHS ${LIB_DIR_LOCATION})
set(INCLUDE_DIRS ${NAME}/include)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable (daedalus ${daedalus_SOURCES})
    include_directories(${CURL_INCLUDE_DIR})
    target_link_libraries(daedalus sqlite3)
    target_include_directories(daedalus PRIVATE ${INCLUDE_DIRS})
    target_link_libraries(daedalus ${CURL_LIBRARIES})
    target_link_libraries(daedalus ${Boost_LIBRARIES})
    target_link_libraries (daedalus ${CMAKE_THREAD_LIBS_INIT})
endif()
