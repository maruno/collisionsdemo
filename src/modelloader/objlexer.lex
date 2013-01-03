%{
	#include "modelloader/objlexer.hpp"
	//Used to keep track of location
	#define YY_USER_ACTION yylloc->columns(yyleng);
	
	#include "src/modelloader/objparser.yy.hpp"
%}

%option c++
%option yyclass="ObjLexer"

%option noyywrap
%option nodefault
%option warn

LineTerminator \r|\n|\r\n
WhiteSpace [ \t\f]
Identifier [A-Za-z0-9 \-\.\(\):\']*
TextLine ({Identifier}|{WhiteSpace})*{LineTerminator}
Comment #{TextLine}
Float "-"?[0-9]+"."[0-9]+
Integer [1-9][0-9]*
Vertex "v"{WhiteSpace}
Face "f"{WhiteSpace}
Slash "/"
Crap ("mtllib"|"usemtl"|"s"|"o"|"g")({Identifier}|{WhiteSpace})*{LineTerminator}

%%

%{
	yylloc->step();
%}

{Comment} {}

{LineTerminator} {return modelloader::ObjParser::token::NL;}
{Vertex} {return modelloader::ObjParser::token::VERTEX;}
{Face} {return modelloader::ObjParser::token::FACE;}
{Float} {*yylval = yytext; return modelloader::ObjParser::token::FLOAT;}
{Integer} {*yylval = yytext; return modelloader::ObjParser::token::INTEGER;}
{Slash} {return modelloader::ObjParser::token::INDEXDELIMITER;}
{WhiteSpace} {}
{Crap} {}
. printf("Lexical analyzer: bad input character '%s' at line %d\n", yytext, yylineno);

%%
