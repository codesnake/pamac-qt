cmake_minimum_required(VERSION 3.14)
project(pamac-qt)

find_package(Qt5Core REQUIRED)

find_package(Qt5QuickCompiler)

if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    qt5_add_resources(RESOURCES qml.qrc)
else ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    qtquick_compiler_add_resources(RESOURCES qml.qrc)
endif ( CMAKE_BUILD_TYPE STREQUAL "Debug" )

file(GLOB app_src "src/cpp/include/*.h" "src/cpp/src/*.cpp")

add_executable(${PROJECT_NAME} ${app_src} ${RESOURCES})


set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTORCC TRUE)
add_definitions(-DQT_QML_DEBUG)
target_include_directories(${PROJECT_NAME} PUBLIC "src/cpp/include")
target_link_libraries(${PROJECT_NAME} libqpamac)
target_link_libraries(${PROJECT_NAME} desktopnotificationservices)

qt5_use_modules(${PROJECT_NAME} Core Qml Quick QuickControls2 Widgets)
configure_file(data/applications/pamac-qt.desktop.in data/applications/pamac-qt.desktop)
configure_file(data/applications/pamac-qt-tray.desktop.in data/applications/pamac-qt-tray.desktop)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/data/applications/pamac-qt.desktop DESTINATION share/applications/ )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/data/applications/pamac-qt-tray.desktop DESTINATION /etc/xdg/autostart/ )
