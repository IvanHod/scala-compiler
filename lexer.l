%option noyywrap never-interactive

%top {
    #pragma warning(disable: 4996)
    #include <stdio.h>
    #include <stdlib.h>
    #include "bison.h"
}

%{
%}

%x LINE_COMMENT
%x COMMENT
%x CHAR
%x STRING
%x MULT_STRING
%x s_STRING
%x f_STRING

_id_ [A-Za-z_][A-Za-z_0-9]*

%%

    /* ------------------------ commentaries rules ----------------------------- */

"//"                    { BEGIN(LINE_COMMENT);printf("LINE_COMMENT\n"); }
"/*"                    { BEGIN(COMMENT); printf("START_COMMENT"); }

<LINE_COMMENT>\n        { BEGIN(INITIAL); }
<LINE_COMMENT>.*        {}

<COMMENT>"*/"           { BEGIN(INITIAL);printf("END_COMMENT"); }
<COMMENT>.              {}

    /* ------------------------ strings rules ---------------------------------- */

"'"                     { BEGIN(CHAR);printf("CONST_CHAR"); }
"\""                    { BEGIN(STRING);printf("CONST_STRING"); }
"\"\"\""                { BEGIN(MULT_STRING);printf("CONST_MANY_STRING"); }

<CHAR>"'"               { BEGIN(INITIAL); }
<CHAR>.                 {}
<STRING>"\\n"           { printf(" \n "); }
<STRING>"\\f"           { printf(" \n "); }
<STRING>"\\r"           { printf(" \n "); }
<STRING>"\\b"           { printf(" \n "); }
<STRING>"\\t"           { printf(" \t "); }
<STRING>"\\\\"          { printf("\\ "); }
<STRING>"\\\'"          { printf("\' "); }
<STRING>"\\\""          { printf("\" "); }
<STRING>"\""            { BEGIN(INITIAL); }
<STRING>.               {}
<MULT_STRING>"\"\"\""   { BEGIN(INITIAL); }
<MULT_STRING>.          {}

    /* ------------------------ s_STRING rules --------------------------------- */

"s\""                   { BEGIN(s_STRING); printf("s\" "); }

<s_STRING>\${_id_}      { printf("ID "); }
<s_STRING>\$\{          { BEGIN( s_STRING ); printf("${ "); }
<s_STRING>"\""          { BEGIN(INITIAL); printf("\""); }
<s_STRING>.             {}

<s_STRING>"}"           { BEGIN( s_STRING ); printf("} "); }

    /* ------------------------ f_STRING rules --------------------------------- */

"f\""                   { BEGIN(f_STRING); printf("f\" "); }

<f_STRING>\${_id_}      { printf("ID "); }
<f_STRING>%s            { printf("%s "); }
<f_STRING>%.f|d         { printf("%s "); }



    /* ------------------------ array initialize --------------------------------*/

"Array"                 { return ARRAY; }
"["                     { return '['; }
"]"                     { return ']'; }

    /* ------------------------ keyword from "system" unit -------------------- */

def                     { return DEF; }
"class"                 { return CLASS; }
override                { return OVERRIDE; }

    /* ------------------------ constants and variables ----------------------- */

val                     { return VAL; }
var                     { return VAR; }
<INITIAL,s_STRING>true  { return TRUE; }
<INITIAL,s_STRING>false { return FALSE; }
<INITIAL,s_STRING>Null  { return NULL_EXPR; }

    /* ----------------------- general keywords ------------------------------- */

public                  { return PUBLIC; }
private                 { return PRIVATE; }
protected               { return PROTECTED; }
static                  { return STATIC; }
import                  { return IMPORT; }
this                    { return THIS; }
lazy                    { return LAZY; }
new                     { return NEW; }
object                  { return OBJECT; }
"return"                { return RETURN; }
"\t"                    { printf("\t"); }
"\n"                    { printf(";"); }

    /* ----------------------- cycle keywords -------------------------------- */

while                   { return WHILE; }
for                     { return FOR; }
until                   { return UNTIL; }
"<-"                    { return LEFT_ARROW; }
to                      { return TO; }
case                    { return CASE; }
match                   { return MATCH; }
if                      { return IF; }
else                    { return ELSE; }

    /* ----------------------- signs & operators ----------------------------- */

"{"                     { return '{'; }
"}"                     { return '}'; }
":"                     { return ':'; }
";"                     { return ';'; }
","                     { return ','; }
<INITIAL,s_STRING>"("   { return '('; }
<INITIAL,s_STRING>")"   { return ')'; }
<INITIAL,s_STRING>"=="  { return EQUAL; }
<INITIAL,s_STRING>"!="  { return NOT_EQUAL; }
<INITIAL,s_STRING>">="  { return MORE_EQ_THAN; }
<INITIAL,s_STRING>"<="  { return LESS_EQ_THAN; }
<INITIAL,s_STRING>"+="  { return PLUS_EQUAL; }
<INITIAL,s_STRING>"-="  { return MINUS_EQUAL; }
<INITIAL,s_STRING>"&="  { return AND_EQUAL; }
<INITIAL,s_STRING>"|="  { return OR_EQUAL; }
<INITIAL,s_STRING>"*="  { return MUL_EQUAL; }
<INITIAL,s_STRING>"/="  { return DIV_EQUAL; }
<INITIAL,s_STRING>"%="  { return DIV_WITH_REM_EQUAL; }
<INITIAL,s_STRING>"="   { return '='; }
<INITIAL,s_STRING>"++"  { return PREFIX_INC; }
<INITIAL,s_STRING>"--"  { return PREFIX_DEC; }
<INITIAL,s_STRING>"+"   { return '+'; }
<INITIAL,s_STRING>"-"   { return '-'; }
<INITIAL,s_STRING>"*"   { return '*'; }
<INITIAL,s_STRING>"/"   { return '/'; }
<INITIAL,s_STRING>"%"   { return '%'; }
<INITIAL,s_STRING>"&&"  { return AND; }
<INITIAL,s_STRING>"&"   { return '&'; }
<INITIAL,s_STRING>"||"  { return OR; }
<INITIAL,s_STRING>"|"   { return '|'; }
<INITIAL,s_STRING>">"   { return '>'; }
<INITIAL,s_STRING>"<"   { return '<'; }
<INITIAL,s_STRING>"^"   { return '^'; }
<INITIAL,s_STRING>"."   { return '.'; }

    /* ------------------------- declare ---------------------------------- */

<INITIAL,s_STRING>println                { return PRINTFLN; }
<INITIAL,s_STRING>0x[1-9ABCDFE]+         { printf("HEX_LITERAL: %f", atof(yytext));  }
<INITIAL,s_STRING>0[0-7]+                { printf("OCT_LITERAL: %i ", strtol(yytext, NULL, 8)); }
<INITIAL,s_STRING>([1-9][0-9]*)|0        { printf("INT_LITERAL: %d", atoi(yytext)); }
<INITIAL,s_STRING>[1-9][0-9]*\.[0-9]*    { printf("NUMBER_FLOAT "); }
<INITIAL,s_STRING>[1-9]e[0-9]*f          { printf("NUMBER_FLOAT "); }

<INITIAL,s_STRING>{_id_}                 { printf("ID "); }

<INITIAL,s_STRING>.                      {}
%%
