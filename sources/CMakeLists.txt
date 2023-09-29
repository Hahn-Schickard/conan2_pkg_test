#@+ ======================== User MODULE NAME configuration ============================
set(THIS Runner)
#@- =========================== END OF USER CONFIGURATION ===============================

set(MODULE ${PROJECT_NAME}_${THIS})

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

add_executable(${MODULE})

target_sources(${MODULE} 
               PRIVATE ${sources_list}
)

#@+ ======================== User DEPENDENCIES configuration ============================
target_link_libraries(${MODULE}
                      PUBLIC fmt::fmt
                      PUBLIC libselinux::libselinux
                      PUBLIC Threads::Threads
)

if(WITH_NANODBC)
    target_link_libraries(${MODULE}
                          PUBLIC nanodbc::nanodbc
    )
    add_compile_definitions(ENABLE_NANODBC)
endif()

if(WITH_CURL)
    target_link_libraries(${MODULE}
                          PUBLIC CURL::libcurl
    )
    add_compile_definitions(ENABLE_CURL)
endif()

if(WITH_SUBPROJECT)
    target_link_libraries(${MODULE}
                          PUBLIC EXAMPLE_REST::EXAMPLE_REST
    )
    add_compile_definitions(ENABLE_SUBPROJECT)
endif()
#@- =========================== END OF USER CONFIGURATION ===============================

target_compile_features(${MODULE} PUBLIC cxx_std_17)

install(
    TARGETS ${MODULE}
    RUNTIME_DEPENDENCIES
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
