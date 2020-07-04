cmake_minimum_required(VERSION 3.5)

project(Timetable2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick Sql Svg LinguistTools REQUIRED)

set(HEADERS ${CMAKE_SOURCE_DIR}/src/internal/api_base.h
    ${CMAKE_SOURCE_DIR}/src/src/api.h
    ${CMAKE_SOURCE_DIR}/src/internal/api_structs.h
    ${CMAKE_SOURCE_DIR}/src/internal/traits.h
    ${CMAKE_SOURCE_DIR}/src/internal/json_parser.h
    ${CMAKE_SOURCE_DIR}/src/internal/macro.h
    ${CMAKE_SOURCE_DIR}/src/qml/search_model.h
    ${CMAKE_SOURCE_DIR}/src/qml/table_model.h
    ${CMAKE_SOURCE_DIR}/src/src/local_storage.h
    ${CMAKE_SOURCE_DIR}/src/src/database.h
    ${CMAKE_SOURCE_DIR}/src/internal/query_builder.h
    ${CMAKE_SOURCE_DIR}/src/qml/saved_model.h
    ${CMAKE_SOURCE_DIR}/src/qml/settings_model.h
    ${CMAKE_SOURCE_DIR}/src/internal/material.h
    ${CMAKE_SOURCE_DIR}/src/qml/settings_group_model.h
    ${CMAKE_SOURCE_DIR}/src/qml/search_filter_model.h
    ${CMAKE_SOURCE_DIR}/src/internal/executor.h
    ${CMAKE_SOURCE_DIR}/src/src/settings.h
    ${CMAKE_SOURCE_DIR}/src/src/language.h
    )

set(SOURCES  ${CMAKE_SOURCE_DIR}/src/src/api.cpp
    ${CMAKE_SOURCE_DIR}/src/internal/material.cpp
    ${CMAKE_SOURCE_DIR}/src/src/saved_model.cpp
    ${CMAKE_SOURCE_DIR}/src/src/search_filter_model.cpp
    ${CMAKE_SOURCE_DIR}/src/qml/search_model.cpp
    ${CMAKE_SOURCE_DIR}/src/src/settings_group_model.cpp
    ${CMAKE_SOURCE_DIR}/src/src/settings_model.cpp
    ${CMAKE_SOURCE_DIR}/src/qml/table_model.cpp
    ${CMAKE_SOURCE_DIR}/src/src/local_storage.cpp
    ${CMAKE_SOURCE_DIR}/src/internal/query_builder.cpp
    ${CMAKE_SOURCE_DIR}/src/internal/executor.cpp
    ${CMAKE_SOURCE_DIR}/src/src/settings.cpp
    main.cpp)

set (RESOURCES ${CMAKE_SOURCE_DIR}/qml.qrc)

set (TRANSLATIONS ${CMAKE_SOURCE_DIR}/languages/ru.ts
                  ${CMAKE_SOURCE_DIR}/languages/en.ts)

qt5_create_translation(GENERATED_TR ${HEADERS} ${SOURCES} ${RESOURCES} ${TRANSLATIONS})

if(ANDROID)
    message("Building for android")
    add_library(Timetable2 SHARED
        ${HEADERS}
        ${SOURCES}
        ${RESOURCES}
        ${GENERATED_TR}
        )
else()
    message("Building for another platform")
    add_executable(Timetable2
        ${HEADERS}
        ${SOURCES}
        ${RESOURCES}
        ${GENERATED_TR}
        )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_compile_definitions(Timetable2
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
        "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    add_compile_options(-Wall -pedantic-errors)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    add_compile_options(-Wall -pedantic-errors)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
    add_compile_options(/Zc:__cplusplus /permissive- /Wall)
endif()

conan_cmake_run(REQUIRES
    rapidjson/1.1.0
    magic_get/1.0
    BASIC_SETUP
    BUILD missing)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt5::Core Qt5::Quick Qt5::Sql Qt5::Svg ${CONAN_LIBS})
