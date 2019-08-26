cmake_minimum_required(VERSION 3.14)
project(desktopnotificationservices)
find_package(Qt5Core COMPONENTS Core Quick Qml)
find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBNOTIFY REQUIRED libnotify)
add_definitions(-DQT_NO_KEYWORDS)

file(GLOB notificationservices_src
    "include/*.h"
    "src/*.cpp"
)

add_library(${PROJECT_NAME} ${notificationservices_src})


target_link_libraries(${PROJECT_NAME} ${LIBNOTIFY_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBNOTIFY_INCLUDE_DIRS})

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
target_include_directories(${PROJECT_NAME} PUBLIC include)
qt5_use_modules(${PROJECT_NAME} Core DBus Widgets)
