cmake_minimum_required(VERSION 3.24.0)

execute_process(COMMAND conan --version
                OUTPUT_VARIABLE CONAN_EXE_OUTPUT
                OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" CONAN_VERSION ${CONAN_EXE_OUTPUT})

message(STATUS "Detected Conan version: ${CONAN_VERSION}")

if(${CONAN_VERSION} STRGREATER_EQUAL "2.0.0" AND CMAKE_CONAN)
        message(STATUS "Using conan v2 cmake integration")
        set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/conan.cmake)
endif()

#@+ ======================== User PROJECT_NAME configuration ============================
set(THIS EXAMPLE_REST)
#@- =========================== END OF USER CONFIGURATION ===============================

project(${THIS} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDES_DIR "${PROJECT_SOURCE_DIR}/includes")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(STATIC_CODE_ANALYSIS "Enable Code Linter" ON)
option(COVERAGE_TRACKING "Enable code test coverage tracking with gcov" ON)

include(GNUInstallDirs)
include(linter)
include(compiler_settings)

if(${CONAN_VERSION} STRLESS "2.0.0" AND CMAKE_CONAN)
        message(STATUS "Using conan v1 cmake integration")
        include(conan_v1)
        execute_conan_install()
endif()

#@+ ======================== User PACKAGES configuration ===========================
find_package(cassandra-cpp-driver REQUIRED)
find_package(oatpp REQUIRED)
find_package(Threads REQUIRED)
#@- ========================= END OF USER CONFIGURATION ============================

add_subdirectory(sources)
