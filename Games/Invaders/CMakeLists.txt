set(APP_SOURCES
    "src/Invaders.h"
    "src/Invaders.cpp"
)

set(PROJECT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(APP_NAME Invaders)

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

if(WIN32)
  set(IRRKLANG_BIN_PATH ${PROJECT_SOURCE_DIR}/Engine/vendor/irrklang/bin/win/)

  add_custom_command(TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${IRRKLANG_BIN_PATH}irrKlang.dll
    ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/irrKlang.dll

    COMMAND ${CMAKE_COMMAND} -E copy
    ${IRRKLANG_BIN_PATH}ikpMP3.dll
    ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/ikpMP3.dll

    COMMAND ${CMAKE_COMMAND} -E copy
    ${IRRKLANG_BIN_PATH}ikpFlac.dll
    ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/ikpFlac.dll)
else()
  message("This is not supported platform for now!")
endif()

set(RES_COMMON_PATH ${PROJECT_SOURCE_DIR}/Engine/res)

add_custom_command(TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RES_COMMON_PATH}
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets

        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/settings.cfg
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/settings.cfg)