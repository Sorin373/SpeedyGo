cmake_minimum_required(VERSION 3.23)
project(SpeedyGo
    VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

if(NOT CMAKE_CXX_COMPILE_FEATURES)
    message(FATAL_ERROR "The compiler does not support C++17.")
endif()

find_package(CURL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

# Change the directory according to your preferences
target_include_directories(${PROJECT_NAME} PRIVATE "C:/Program Files/MySQL/MySQL Connector C++ 8.1/include")

target_link_libraries(${PROJECT_NAME} PRIVATE
    # Change the directory according to your preferences
    "C:/Program Files/MySQL/MySQL Connector C++ 8.1/lib64/vs14/mysqlcppconn.lib"
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Release/utils
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Release/logs
)

configure_file(${CMAKE_SOURCE_DIR}/utils/legaturi.txt ${CMAKE_BINARY_DIR}/Release/utils/legaturi.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/utils/contor_TSP_log.txt ${CMAKE_BINARY_DIR}/Release/utils/contor_TSP_log.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/utils/distante_orase.json ${CMAKE_BINARY_DIR}/Release/utils/ldistante_orase.json COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/logs/TSP_log.txt ${CMAKE_BINARY_DIR}/Release/logs/TSP_log.txt COPYONLY)

# Check if the config.json with the API key exists in the source directory
if(EXISTS ${CMAKE_SOURCE_DIR}/utils/config.json)
    configure_file(${CMAKE_SOURCE_DIR}/utils/config.json ${CMAKE_BINARY_DIR}/Release/utils/config.json COPYONLY)
endif()

# Add mysql-connector-cpp Dlls (if you come across errors try to manually renew the DLLs)
# Add inside src/utils/DLLs the dlls from the msi installation 
# They are usually located at MySQL/MySQL Connector C++ 8.1/lib64/

# Last version used v8.1.0
# Installer link https://dev.mysql.com/downloads/connector/cpp/

configure_file(${CMAKE_SOURCE_DIR}/utils/DLLs/libcrypto-3-x64.dll ${CMAKE_BINARY_DIR}/Release/libcrypto-3-x64.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/utils/DLLs/libssl-3-x64.dll ${CMAKE_BINARY_DIR}/Release/libssl-3-x64.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/utils/DLLs/mysqlcppconn-9-vs14.dll ${CMAKE_BINARY_DIR}/Release/mysqlcppconn-9-vs14.dll COPYONLY)

file(GLOB DLL_FILES "${CMAKE_BINARY_DIR}/Release/*.dll")
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
install(FILES ${DLL_FILES} DESTINATION bin)
install(FILES
    "${CMAKE_BINARY_DIR}/Release/libcrypto-3-x64.dll"
    "${CMAKE_BINARY_DIR}/Release/libssl-3-x64.dll"
    "${CMAKE_BINARY_DIR}/Release/mysqlcppconn-9-vs14.dll"
    DESTINATION bin
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/utils/ DESTINATION bin/utils FILES_MATCHING PATTERN "*")
install(DIRECTORY ${CMAKE_SOURCE_DIR}/logs/ DESTINATION bin/logs FILES_MATCHING PATTERN "*")

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
include(CPack)