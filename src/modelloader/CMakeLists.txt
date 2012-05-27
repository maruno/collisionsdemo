#Generate modelloader
bison_target(objparser objparser.yy ${CMAKE_CURRENT_BINARY_DIR}/objparser.yy.cpp)
flex_target(objlexer objlexer.lex ${CMAKE_CURRENT_BINARY_DIR}/objlexer.yy.cpp)
add_flex_bison_dependency(objlexer objparser)

add_library(objloader STATIC ${BISON_objparser_OUTPUTS} ${FLEX_objlexer_OUTPUTS})