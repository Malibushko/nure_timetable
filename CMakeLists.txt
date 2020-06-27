cmake_minimum_required(VERSION 3.5)

project(Timetable2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

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
            ${CMAKE_SOURCE_DIR}/src/internal/executor.h)

set(SOURCES  ${CMAKE_SOURCE_DIR}/src/src/api.cpp
             ${CMAKE_SOURCE_DIR}/src/internal/material.cpp
             main.cpp)

set (RESOURCES ${CMAKE_SOURCE_DIR}/qml.qrc)

set (TRANSLATIONS ${CMAKE_SOURCE_DIR}/languages/ru.ts
                  ${CMAKE_SOURCE_DIR}/languages/en.ts)

qt5_create_translation(GENERATED_TR ${HEADERS} ${SOURCES} ${RESOURCES} ${TRANSLATIONS})

if(ANDROID)
    add_library(Timetable2 SHARED
      ${HEADERS}
      ${SOURCES}
      ${RESOURCES}
      ${GENERATED_TR}
    )
else()
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

conan_cmake_run(REQUIRES
                rapidjson/1.1.0
                magic_get/1.0
                BASIC_SETUP
                BUILD missing)

target_link_libraries(Timetable2
  PRIVATE Qt5::Core Qt5::Quick Qt5::Sql Qt5::Svg ${CONAN_LIBS})
