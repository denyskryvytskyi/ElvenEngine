set(STB_SOURCES
"src/stb_image.cpp"
)

add_library(stb STATIC ${STB_SOURCES})

target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)