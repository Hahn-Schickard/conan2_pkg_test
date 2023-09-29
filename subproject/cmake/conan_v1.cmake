macro(execute_conan_install)
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
      message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
      file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
              "${CMAKE_BINARY_DIR}/conan.cmake"
              TLS_VERIFY ON)
  endif()

  include(${CMAKE_BINARY_DIR}/conan.cmake)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
  list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

  if(CONAN_EXPORTED)
      MESSAGE(STATUS "Creating conan package")
      include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
      conan_basic_setup()
  else()
      MESSAGE(STATUS "Detecting conan settings")
      conan_cmake_autodetect(settings
                            BUILD_TYPE ${TYPE}
      )
      MESSAGE(STATUS "Running conan install")
      conan_cmake_install(PATH_OR_REFERENCE ${PROJECT_SOURCE_DIR}
                          BUILD missing
                          SETTINGS ${settings}
      )
  endif()
endmacro(execute_conan_install)
