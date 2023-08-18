cmake_minimum_required(VERSION 3.15)
project(SpeedyGo)

set(CMAKE_PREFIX_PATH
    "C:/vcpkg/installed/x86-windows"
    "C:/vcpkg/packages/nlohmann-json_x86-windows/share/nlohmann_json"
    "C:/vcpkg/packages/curl_x86-windows/share/curl"
    "C:/Program Files/MySQL/MySQL Connector C++ 8.1"
)

find_package(CURL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(main main.cpp)

target_include_directories(main PRIVATE
    "C:/Program Files/MySQL/MySQL Connector C++ 8.1/include"
    ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include
)

target_link_libraries(main PRIVATE CURL::libcurl)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(main PRIVATE mysqlcppconn8)
target_link_libraries(main PRIVATE
    -L"C:/vcpkg/installed/x86-windows/bin"
    -L"C:/Program Files/MySQL/MySQL Connector C++ 8.1/lib64/vs14"
)
