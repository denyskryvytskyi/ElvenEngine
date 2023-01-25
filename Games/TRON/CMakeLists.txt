set(APP_SOURCES
    "src/TRON.h"
    "src/TRON.cpp"
    "assets/shaders/texture_shader.frag"
    "assets/shaders/texture_shader.vert"
)

set(PROJECT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(APP_NAME TRON)

add_executable(${APP_NAME} ${APP_SOURCES})

set_target_properties(${APP_NAME} PROPERTIES FOLDER "Games")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${APP_SOURCES})

target_link_libraries(${APP_NAME} PUBLIC ElvenEngine)

target_include_directories(${APP_NAME}
        PRIVATE ${PROJECT_INCLUDE_DIRECTORIES}
        )

set_target_properties(${APP_NAME} PROPERTIES 
                      VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

target_compile_definitions(${APP_NAME} PRIVATE
  $<$<CONFIG:Debug>:DEBUG_MODE>
  $<$<CONFIG:Release>:RELEASE_MODE>
)

add_custom_command(TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets

        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/settings.cfg
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/settings.cfg)