cmake_minimum_required(VERSION 3.15)
project(SpeedyGo)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(main main.cpp)

target_include_directories(main PRIVATE "C:/Program Files/MySQL/MySQL Connector C++ 8.1/include")
target_link_libraries(main PRIVATE CURL::libcurl)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(main PRIVATE "C:/Program Files/MySQL/MySQL Connector C++ 8.1/lib64/vs14/mysqlcppconn.lib")
