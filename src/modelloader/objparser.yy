%require "2.5"
%skeleton "lalr1.cc"

%defines
%locations

%define namespace "modelloader"
%define parser_class_name "ObjParser"

%parse-param {std::vector<float>& verticesData}
%parse-param {std::vector<unsigned int>& indicesData}
%parse-param {modelloader::ObjLexer& lexer}

%lex-param {modelloader::ObjLexer& lexer}

%code requires {
	#include <vector>
	#include <boost/lexical_cast.hpp>
	
	#define YYSTYPE std::string
	
	namespace modelloader {
		class ObjLexer;
	}
}

%code {
	static int yylex(modelloader::ObjParser::semantic_type* yylval, modelloader::ObjParser::location_type* yylloc,
			modelloader::ObjLexer& lexer);
}

%token NL VERTEX FACE FLOAT INTEGER INDEXDELIMITER

%%

input : input line | line;

line : vertex NL | face NL;

vertex : VERTEX FLOAT FLOAT FLOAT {
	verticesData.push_back(boost::lexical_cast<float>($2));
	verticesData.push_back(boost::lexical_cast<float>($3));
	verticesData.push_back(boost::lexical_cast<float>($4));
};

face : FACE INTEGER INTEGER INTEGER {
	indicesData.push_back(boost::lexical_cast<unsigned int>($2));
	indicesData.push_back(boost::lexical_cast<unsigned int>($3));
	indicesData.push_back(boost::lexical_cast<unsigned int>($4));
};

%%

void modelloader::ObjParser::error(const modelloader::ObjParser::location_type& loc,
                                          const std::string &msg) {
	//TODO Throw syntax error
}

#include "modelloader/objlexer.hpp"
static int yylex(modelloader::ObjParser::semantic_type* yylval, modelloader::ObjParser::location_type* yylloc, modelloader::ObjLexer& lexer) {
	return lexer.yylex(yylval, yylloc);
}