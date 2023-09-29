cmake_minimum_required(VERSION 3.6)

set(THIS API)
set(MODULE ${PROJECT_NAME}_${THIS})

file(GLOB includes_list "${INCLUDES_DIR}/API/*.hpp")
file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

add_library(${MODULE})

target_sources(${MODULE}
               PUBLIC ${includes_list}
               PRIVATE ${sources_list}
)

target_link_libraries(${MODULE}
                      PUBLIC oatpp::oatpp
                      PUBLIC Threads::Threads
                      PUBLIC ${PROJECT_NAME}_Utility
)

target_include_directories(${MODULE} PUBLIC
                           $<BUILD_INTERFACE:${INCLUDES_DIR}/API>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)

target_compile_features(${MODULE} PUBLIC cxx_std_17)

install(
    TARGETS ${MODULE}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES ${includes_list}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
