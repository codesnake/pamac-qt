cmake_minimum_required(VERSION 3.14)
project(pamac-qt)


add_subdirectory(libqpamac)
add_subdirectory(qmltable)
add_subdirectory(notificationservice)
if(NOT DEFINED VERSION)
execute_process(COMMAND git --git-dir ./.git --work-tree ./ describe --always --tags
                OUTPUT_VARIABLE RES
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
string(STRIP ${RES} RES)
add_definitions(-DVERSION=\"${RES}\")
endif(NOT DEFINED VERSION)
add_subdirectory(app)


install(TARGETS libqpamac desktopnotificationservices pamac-qt-app)
