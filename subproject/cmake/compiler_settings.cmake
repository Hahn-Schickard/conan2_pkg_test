if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Using Clang compiler")
    # setup your Clang toolchain here
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(STATUS "Using GNU compiler")

add_definitions(
    -W
    -Wunused-variable
    -Wunused-parameter
    -Wunused-function
    -Wunused
    -Wno-system-headers
    -Wno-deprecated
    -Woverloaded-virtual
    -Wwrite-strings
    -Wall
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--disable-new-dtags")

if (NOT WIN32 AND COVERAGE_TRACKING)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage ")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

    add_definitions(
        -fprofile-arcs
        -ftest-coverage
        -fdiagnostics-color=auto
        -lgcov
    )
endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # setup your MSVC toolchain here
endif()
