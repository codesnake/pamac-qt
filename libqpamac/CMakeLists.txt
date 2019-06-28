cmake_minimum_required(VERSION 3.14)
project(libqpamac)
find_package(Qt5Core REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules (GLIB2 REQUIRED glib-2.0)
pkg_check_modules (PAMAC REQUIRED pamac)
add_definitions(-DQT_NO_KEYWORDS)
find_library(pamac REQUIRED)

file(GLOB qpamac_src
    "include/*.h"
    "include/libqpamac"
    "src/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${qpamac_src})
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} ${GLIB2_LIBRARIES} ${PAMAC_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${GLIB2_INCLUDE_DIRS} ${PAMAC_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${GLIB2_CFLAGS_OTHER} )

qt5_use_modules(${PROJECT_NAME} Core Qml Quick)
