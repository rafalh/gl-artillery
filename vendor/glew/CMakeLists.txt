project(glew)

include_directories(include)

set(SOURCES src/glew.c)
file(GLOB_RECURSE HEADERS include/GL/*.h)

add_library(glew STATIC ${SOURCES} ${HEADERS})

target_compile_definitions(glew PRIVATE GLEW_NO_GLU GLEW_STATIC)
