cmake_minimum_required(VERSION 3.15)
project(SpeedyGo)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(main main.cpp)

target_include_directories(main PRIVATE "C:/Program Files/MySQL/MySQL Connector C++ 8.1/include")
target_link_libraries(main PRIVATE "C:/Program Files/MySQL/MySQL Connector C++ 8.1/lib64/vs14/mysqlcppconn.lib")
target_link_libraries(main PRIVATE CURL::libcurl)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Release/utils
)

configure_file(${CMAKE_SOURCE_DIR}/utils/legaturi.txt ${CMAKE_BINARY_DIR}/Release/utils/legaturi.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/utils/contor_TSP_log.txt ${CMAKE_BINARY_DIR}/Release/utils/contor_TSP_log.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/utils/distante_orase.json ${CMAKE_BINARY_DIR}/Release/utils/ldistante_orase.json COPYONLY)

# Add mysql-connector-cpp Dlls
configure_file(${CMAKE_SOURCE_DIR}/utils/DLLs/libcrypto-3-x64.dll ${CMAKE_BINARY_DIR}/Release/libcrypto-3-x64.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/utils/DLLs/libssl-3-x64.dll ${CMAKE_BINARY_DIR}/Release/libssl-3-x64.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/utils/DLLs/mysqlcppconn-9-vs14.dll ${CMAKE_BINARY_DIR}/Release/mysqlcppconn-9-vs14.dll COPYONLY)
