cmake_minimum_required(VERSION 3.23)
project(SpeedyGo)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(main main.cpp)

# Change the directory according to your preferences
target_include_directories(main PRIVATE "C:/Program Files/MySQL/MySQL Connector C++ 8.1/include")

target_link_libraries(main PRIVATE
    # Change the directory according to your preferences
    "C:/Program Files/MySQL/MySQL Connector C++ 8.1/lib64/vs14/mysqlcppconn.lib"
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Release/utils
)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Release/logs
)

configure_file(${CMAKE_SOURCE_DIR}/utils/legaturi.txt ${CMAKE_BINARY_DIR}/Release/utils/legaturi.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/utils/contor_TSP_log.txt ${CMAKE_BINARY_DIR}/Release/utils/contor_TSP_log.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/utils/distante_orase.json ${CMAKE_BINARY_DIR}/Release/utils/ldistante_orase.json COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/logs/TSP_log.txt ${CMAKE_BINARY_DIR}/Release/logs/TSP_log.txt COPYONLY)

# Add mysql-connector-cpp Dlls (if you come across errors try to manually renew the DLLs)
# Add inside src/utils/DLLs the dlls from the msi installation 
# They are usually located at MySQL/MySQL Connector C++ 8.1/lib64/

# Last version used v8.1.0
# Installer link https://dev.mysql.com/downloads/connector/cpp/

configure_file(${CMAKE_SOURCE_DIR}/utils/DLLs/libcrypto-3-x64.dll ${CMAKE_BINARY_DIR}/Release/libcrypto-3-x64.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/utils/DLLs/libssl-3-x64.dll ${CMAKE_BINARY_DIR}/Release/libssl-3-x64.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/utils/DLLs/mysqlcppconn-9-vs14.dll ${CMAKE_BINARY_DIR}/Release/mysqlcppconn-9-vs14.dll COPYONLY)

