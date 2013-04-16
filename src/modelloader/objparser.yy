%require "2.5"
%skeleton "lalr1.cc"

%defines
%locations

%define namespace "modelloader"
%define parser_class_name "ObjParser"

%parse-param {std::vector<glm::vec3>& verticesData}
%parse-param {std::vector<glm::uvec3>& indicesData}
%parse-param {std::tuple<glm::vec3, glm::vec3>& extremes}
%parse-param {modelloader::ObjLexer& lexer}

%lex-param {modelloader::ObjLexer& lexer}

%error-verbose

%code requires {
	#include <vector>
	#include <tuple>
	#include <algorithm>
	
	#include <boost/lexical_cast.hpp>
	
	#include <glm/glm.hpp>
	
	#define YYSTYPE std::string
	
	#include <iostream>
	
	namespace modelloader {
		class ObjLexer;
	}
}

%code {
	static int yylex(modelloader::ObjParser::semantic_type* yylval, modelloader::ObjParser::location_type* yylloc,
			modelloader::ObjLexer& lexer);
	
	void addVertex(float x, float y, float z, std::vector<glm::vec3>& verticesData, std::tuple<glm::vec3, glm::vec3>& extremes);
}

%token NL VERTEX FACE FLOAT INTEGER INDEXDELIMITER

%%

input : input line | line;

line : vertex NL | face NL;

vertex : VERTEX FLOAT FLOAT FLOAT {
	float x = boost::lexical_cast<float>($2);
	float y = boost::lexical_cast<float>($3);
	float z = boost::lexical_cast<float>($4);
	
	glm::vec3& currentMinExtreme = std::get<0>(extremes);
	glm::vec3& currentMaxExtreme = std::get<1>(extremes);
	
	if(x < currentMinExtreme.x || y < currentMinExtreme.y || z > currentMinExtreme.z) {
		//New minimal extreme
		glm::vec3 newMinExtreme;
		
		newMinExtreme.x = std::min(x, currentMinExtreme.x);
		newMinExtreme.y = std::min(y, currentMinExtreme.y);
		newMinExtreme.z = std::max(z, currentMinExtreme.z);
		
		extremes = std::tuple<glm::vec3, glm::vec3>(newMinExtreme, currentMaxExtreme);
	} else if(x > currentMaxExtreme.x || y > currentMaxExtreme.y || z < currentMaxExtreme.z) {
		//New maximal extreme
		glm::vec3 newMaxExtreme;
		
		newMaxExtreme.x = std::max(x, currentMaxExtreme.x);
		newMaxExtreme.y = std::max(y, currentMaxExtreme.y);
		newMaxExtreme.z = std::min(z, currentMaxExtreme.z);
		
		extremes = std::tuple<glm::vec3, glm::vec3>(currentMinExtreme, newMaxExtreme);
	}
	
	verticesData.emplace_back(x, y, z);
};

face : FACE INTEGER INTEGER INTEGER {
	indicesData.emplace_back(boost::lexical_cast<unsigned int>($2)-1, boost::lexical_cast<unsigned int>($3)-1, boost::lexical_cast<unsigned int>($4)-1);
};

%%

void modelloader::ObjParser::error(const modelloader::ObjParser::location_type& loc,
                                          const std::string &msg) {
	std::cerr << msg << " on " << loc << std::endl;
}

#include "modelloader/objlexer.hpp"
static int yylex(modelloader::ObjParser::semantic_type* yylval, modelloader::ObjParser::location_type* yylloc, modelloader::ObjLexer& lexer) {
	return lexer.yylex(yylval, yylloc);
}
