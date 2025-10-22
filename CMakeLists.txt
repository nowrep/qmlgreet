cmake_minimum_required(VERSION 3.5)

project(qmlgreet VERSION 0.1.0)

include(FeatureSummary)

option(USE_QT6 "Use Qt6" OFF)

if(USE_QT6)
    set(QT_MIN_VERSION "6.2.0")
    find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Gui Quick)
else()
    set(QT_MIN_VERSION "5.15.0")
    find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Gui Quick)
endif()

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
