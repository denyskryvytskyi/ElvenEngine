set(GLAD_SOURCES
"include/glad41/gl.h"
"include/KHR41/khrplatform.h"
"src/glad41.c"
)

add_library(glad STATIC ${GLAD_SOURCES})

target_include_directories(glad
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        )