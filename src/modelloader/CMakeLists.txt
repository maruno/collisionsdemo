#Generate modelloader
bison_target(objparser objparser.yy ${CMAKE_CURRENT_BINARY_DIR}/objparser.yy.cpp)
flex_target(objlexer objlexer.lex ${CMAKE_CURRENT_BINARY_DIR}/objlexer.yy.cpp)
add_flex_bison_dependency(objlexer objparser)

set(modelloader_INCS vertexbuffer.hpp vertexbufferfactory.hpp objlexer.hpp)
set(modelloader_SRCS vertexbuffer.cpp vertexbufferfactory.cpp)

add_library(objloader STATIC ${BISON_objparser_OUTPUTS} ${FLEX_objlexer_OUTPUTS} ${modelloader_INCS} ${modelloader_SRCS})
