cmake_minimum_required(VERSION 3.14)
project(qtnotificationservice)
find_package(Qt5 COMPONENTS Core)
file(GLOB notificationservices_src
    "include/*.h"
    "src/*.cpp"
)

add_library(${PROJECT_NAME} ${notificationservices_src})
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
target_include_directories(${PROJECT_NAME} PUBLIC include)
qt5_use_modules(${PROJECT_NAME} Core DBus Widgets)
