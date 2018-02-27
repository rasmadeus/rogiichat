cmake_minimum_required (VERSION 3.1.0)
project(chat)
option(QT5BUILD "Build using Qt5 libs" TRUE)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(QT_INCLUDES ${Qt5Core_INCLUDE_DIRS})
include_directories(${QT_INCLUDES})
QT5_WRAP_CPP(chat_HEADERS_MOC ${HuggleLite_HEADERS})

set(chat_SOURCES "main.cpp" "main_view.cpp" "client.cpp" "server.cpp" "participants_model.cpp" "participant.cpp" "log.cpp")
set(chat_HEADERS "main_view.h" "client.h" "server.h" "participants_model.h" "participant.h" "log.h")
add_executable(chat ${chat_SOURCES} ${chat_HEADERS})

SET_TARGET_PROPERTIES(${target} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER YES)
SET_TARGET_PROPERTIES(${target} PROPERTIES XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${target}/pch.h")

qt5_use_modules(chat Core Gui Widgets Network)

