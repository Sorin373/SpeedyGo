cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(SpeedyGo
    VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

if(NOT CMAKE_CXX_COMPILE_FEATURES)
    message(FATAL_ERROR "The compiler does not support C++17.")
endif()

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif()

if (WIN32)
    find_package(CURL CONFIG REQUIRED)
    find_package(nlohmann_json CONFIG REQUIRED)
elseif(UNIX)
    set(FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR /usr)
    include_directories(${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/include)
    link_directories(${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/lib)
    
    find_package(CURL REQUIRED)
    find_package(nlohmann_json CONFIG REQUIRED)
endif()

if (WIN32)
    enable_language("RC")
    set(WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/assets/resource.rc)
endif()

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_executable(${PROJECT_NAME}
    "src/classes/supply.cpp"
    "src/classes/authentification.cpp"
    "src/classes/depot.cpp"
    "src/classes/product.cpp"
    "src/classes/city.cpp"
    "src/classes/DistanceHandler.cpp"
    "src/classes/route.cpp"

    "src/main.cpp" 
    "src/declarations.cpp"
    "src/logic.cpp"
    "src/GoogleMatrixAPI.cpp"
    "src/database.cpp"
    "src/haversine.cpp"
    ${WIN32_RESOURCES}
)

# Change the directory according to your preferences
target_include_directories(${PROJECT_NAME} PRIVATE "C:/Program Files/MySQL/MySQL Connector C++ 8.1/include")

target_link_libraries(${PROJECT_NAME} PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
         # Change the directory according to your preferences
        "C:/Program Files/MySQL/MySQL Connector C++ 8.1/lib64/vs14/mysqlcppconn.lib"
    )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        mysqlcppconn
    )
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Release/utils
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Release/logs
)

configure_file(${CMAKE_SOURCE_DIR}/src/utils/legaturi.txt ${CMAKE_BINARY_DIR}/Release/utils/legaturi.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/utils/contor_TSP_log.txt ${CMAKE_BINARY_DIR}/Release/utils/contor_TSP_log.txt COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/utils/distante_orase.json ${CMAKE_BINARY_DIR}/Release/utils/ldistante_orase.json COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/logs/TSP_log.txt ${CMAKE_BINARY_DIR}/Release/logs/TSP_log.txt COPYONLY)

# Check if the config.json with the API key exists in the source directory
if(EXISTS ${CMAKE_SOURCE_DIR}/src/utils/config.json)
    configure_file(${CMAKE_SOURCE_DIR}/src/utils/config.json ${CMAKE_BINARY_DIR}/Release/utils/config.json COPYONLY)
endif()

# Add mysql-connector-cpp Dlls (if you come across errors try to manually renew the DLLs)
# Add inside src/utils/DLLs the dlls from the msi installation 
# They are usually located at MySQL/MySQL Connector C++ 8.1/lib64/

# Last version used v8.1.0
# Installer link https://dev.mysql.com/downloads/connector/cpp/

configure_file(${CMAKE_SOURCE_DIR}/src/utils/DLLs/libcrypto-3-x64.dll ${CMAKE_BINARY_DIR}/Release/libcrypto-3-x64.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/utils/DLLs/libssl-3-x64.dll ${CMAKE_BINARY_DIR}/Release/libssl-3-x64.dll COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/utils/DLLs/mysqlcppconn-9-vs14.dll ${CMAKE_BINARY_DIR}/Release/mysqlcppconn-9-vs14.dll COPYONLY)

# Cpack installer configuration
file(GLOB DLL_FILES "${CMAKE_BINARY_DIR}/Release/*.dll")
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/logs/ DESTINATION bin/logs FILES_MATCHING PATTERN "*")
install(FILES ${CMAKE_SOURCE_DIR}/src/utils/contor_TSP_log.txt DESTINATION bin/utils)
install(FILES ${CMAKE_SOURCE_DIR}/src/utils/distante_orase.json DESTINATION bin/utils)
install(FILES ${CMAKE_SOURCE_DIR}/src/utils/legaturi.txt DESTINATION bin/utils)
install(FILES ${DLL_FILES} DESTINATION bin)

if(EXISTS "${CMAKE_BINARY_DIR}/Release/libcrypto-3-x64.dll" AND
   EXISTS "${CMAKE_BINARY_DIR}/Release/libssl-3-x64.dll" AND
   EXISTS "${CMAKE_BINARY_DIR}/Release/mysqlcppconn-9-vs14.dll")
   
    install(FILES
        "${CMAKE_BINARY_DIR}/Release/libcrypto-3-x64.dll"
        "${CMAKE_BINARY_DIR}/Release/libssl-3-x64.dll"
        "${CMAKE_BINARY_DIR}/Release/mysqlcppconn-9-vs14.dll"
        DESTINATION bin
    )
else()
    message(FATAL_ERROR "One or more required DLL files are missing.")
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/ DESTINATION bin/assets FILES_MATCHING PATTERN "*")
else()
    message(SEND_ERROR "Logo icon not found.")
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

if (UNIX)
    set(CPACK_GENERATOR "DEB;RPM;TGZ")
    set(CPACK_PACKAGE_CONTACT "sorin.andrei.tudose@gmail.com")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Sorin <sorin.andrei.tudose@gmail.com")
elseif(WIN32)
    # Using Nsis for Windows (https://sourceforge.net/projects/nsis/files/latest/download)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MUI_ICON ${CMAKE_SOURCE_DIR}/assets/LOGO.ico)
    set(CPACK_NSIS_MUI_UNIICON ${CMAKE_SOURCE_DIR}/assets/LOGO.ico)
    set(CPACK_NSIS_MUI_HEADERICON ${CMAKE_SOURCE_DIR}/assets/LOGO.ico)
    set(CPACK_NSIS_DISPLAY_NAME ${PROJECT_NAME})
    set(CPACK_NSIS_PACKAGE_NAME ${PROJECT_NAME})
endif()

include(CPack)