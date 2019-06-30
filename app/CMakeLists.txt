cmake_minimum_required(VERSION 3.14)
project(pamac-qt-app)

find_package(Qt5Core REQUIRED)
find_package(Qt5QuickCompiler)
qt5_add_resources(RESOURCES qml.qrc)
file(GLOB app_src "src/cpp/include/*.h" "src/cpp/src/*.cpp")
add_executable(${PROJECT_NAME} ${app_src} ${RESOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTORCC TRUE)

target_include_directories(${PROJECT_NAME} PUBLIC "src/cpp/include")
target_link_libraries(${PROJECT_NAME} libqpamac)
target_link_libraries(${PROJECT_NAME} qtnotificationservice)

qt5_use_modules(${PROJECT_NAME} Core Qml Quick QuickControls2)
