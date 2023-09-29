cmake_minimum_required(VERSION 3.6) 

set(THIS Example)
set(MODULE ${PROJECT_NAME}_${THIS})

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

add_executable(${MODULE})

target_sources(${MODULE} PRIVATE 
                       ${sources_list}
)

target_link_libraries(${MODULE} 
                      PRIVATE ${PROJECT_NAME}_API
                      PRIVATE ${PROJECT_NAME}_Utility
)

target_compile_features(${MODULE} PUBLIC cxx_std_17)
