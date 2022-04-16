set(GLAD_SOURCES
"include/glad/gl.h"
"include/KHR/khrplatform.h"
"src/gl.c"
)

add_library(glad STATIC ${GLAD_SOURCES})

target_include_directories(glad
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        )