cmake_minimum_required(VERSION 3.16)

project(untitled1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(untitled1
  main.cpp
)
target_include_directories(untitled1 PRIVATE "C:/boost_1_87_0")
target_link_libraries(untitled1 PRIVATE Qt${QT_VERSION_MAJOR}::Core)

add_executable(tests
  tests.cpp
  test_templates.h
)
target_include_directories(tests PRIVATE "C:/boost_1_87_0")

include(GNUInstallDirs)
install(TARGETS untitled1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
