cmake_minimum_required(VERSION 3.17)
project(crawl)

set(CMAKE_CXX_STANDARD 14)
set(CURL_LIBRARY "-lcurl")

find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)

include_directories(
        ${CURL_INCLUDE_DIR}
        ${LIBXML2_INCLUDE_DIR})
add_executable(crawl
        main.cpp
        curlpp.cpp)

target_link_libraries(crawl ${CURL_LIBRARIES} ${LIBXML2_LIBRARIES})