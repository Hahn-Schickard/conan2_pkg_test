cmake_minimum_required(VERSION 3.24.0)

execute_process(COMMAND conan --version
                OUTPUT_VARIABLE CONAN_EXE_OUTPUT
                OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" CONAN_VERSION ${CONAN_EXE_OUTPUT})

message(STATUS "Detected Conan version: ${CONAN_VERSION}")

if(${CONAN_VERSION} STRGREATER_EQUAL "2.0.0")
        message(STATUS "Using conan v2 cmake integration")
        set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/conan.cmake)
endif()

#@+ ======================== User PROJECT_NAME configuration ============================
set(THIS conan2_pkg_test)
#@- =========================== END OF USER CONFIGURATION ===============================

project(${THIS} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDES_DIR "${PROJECT_SOURCE_DIR}/includes")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_host_system_information(RESULT DISTRO QUERY DISTRIB_ID)
cmake_host_system_information(RESULT DISTRO_VERSION QUERY DISTRIB_VERSION_CODENAME)
set(OS_INFO "${DISTRO}-${DISTRO_VERSION}-${CMAKE_SYSTEM_PROCESSOR}")

if(UNIX AND NOT APPLE AND BUILD_SHARED_LIBS)
        set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/")
endif()

option(STATIC_CODE_ANALYSIS "Enable Code Linter" ON)
option(COVERAGE_TRACKING "Enable code test coverage tracking with gcov" ON)

#@+ ====================== User CMAKE_OPTIONS configuration ========================
option(WITH_NANODBC "Enable nanodbc package usage" OFF)
option(WITH_CURL "Enable CURL package usage" OFF)
option(WITH_SUBPROJECT "Enable subproject package usage" ON)
#@- ===================== END OF USER CONFIGURATION ================================

include(GNUInstallDirs)
include(linter)
include(compiler_settings)

if(${CONAN_VERSION} STRLESS "2.0.0")
        set(WITH_SUBPROJECT OFF) # subproject does not compile properlly for v1
        message(STATUS "Using conan v1 cmake integration")
        include(conan_v1)
        execute_conan_install()
endif()

#@+ ======================== User PACKAGES configuration ===========================
find_package(fmt REQUIRED)
find_package(libselinux REQUIRED)
find_package(Threads REQUIRED)

if(WITH_NANODBC)
        find_package(nanodbc REQUIRED)
endif()

if(WITH_CURL)
        find_package(CURL REQUIRED)
endif()

if(WITH_SUBPROJECT)
        find_package(EXAMPLE_REST REQUIRED)
endif()
#@- ========================= END OF USER CONFIGURATION ============================

add_subdirectory(sources)

install(FILES
        ${PROJECT_SOURCE_DIR}/LICENSE
        ${PROJECT_SOURCE_DIR}/AUTHORS
        ${PROJECT_SOURCE_DIR}/CHANGELOG.md
        DESTINATION ${CMAKE_INSTALL_INFODIR}
)

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${OS_INFO}-${CMAKE_BUILD_TYPE}")
set(CPACK_PACKAGE_VENDOR "Example_Org")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/package")
include(CPack)
