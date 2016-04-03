%option noyywrap never-interactive

%top {
    #pragma warning(disable: 4996)
    #include <stdio.h>
    #include <stdlib.h>
}

%{
%}

%x LINE_COMMENT
%x COMMENT
%x CHAR
%x STRING
%x MULT_STRING
%x s_STRING
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

Array                   { printf("ARRAY"); }
"["                     { printf("[ "); }
"]"                     { printf("] "); }

    /* ------------------------ keyword from "system" unit -------------------- */

def                     { printf("DEF "); }
"class"                 { printf("CLASS "); }
override                { printf("OVERRIDE "); }

    /* ------------------------ constants and variables ----------------------- */

val                     { printf("VAL "); }
var                     { printf("VAR "); }
<INITIAL,s_STRING>true  { printf("TRUE "); }
<INITIAL,s_STRING>false { printf("FALSE "); }
<INITIAL,s_STRING>Null  { printf("NULL "); }

    /* ----------------------- general keywords ------------------------------- */

Nothing                 { printf("NOTHING "); }
Any                     { printf("ANY "); }
AnyRef                  { printf("ANYREF "); }
Unit                    { printf("UNIT "); }
super                   { printf("SUPER "); }
public                  { printf("PUBLIC "); }
private                 { printf("PRIVATE "); }
protected               { printf("PROTECTED "); }
extends                 { printf("EXTENDS "); }
static                  { printf("STATIC "); }
import                  { printf("IMPORT"); }
this                    { printf("THIS "); }
lazy                    { printf("LAZY "); }
new                     { printf("NEW "); }
object                  { printf("OBJECT "); }
"return"                { printf("RETURN "); }
"\t"                    { printf("\t"); }
"\n"                    { printf(";"); }

    /* ----------------------- cycle keywords -------------------------------- */

while                   { printf("WHILE "); }
for                     { printf("FOR "); }
until                   { printf("UNTIL "); }
"<-"                    { printf("<- "); }
to                      { printf("TO "); }
case                    { printf("CASE "); }
match                   { printf("MATCH "); }
if                      { printf("IF "); }
else                    { printf("ELSE "); }

    /* ----------------------- signs & operators ----------------------------- */

"{"                     { printf("{ "); }
"}"                     { printf("} "); }
":"                     { printf(": "); }
";"                     { printf("; "); }
","                     { printf(", "); }
<INITIAL,s_STRING>"("   { printf("( "); }
<INITIAL,s_STRING>")"   { printf(") "); }
<INITIAL,s_STRING>"=="  { printf("== "); }
<INITIAL,s_STRING>"!="  { printf("!= "); }
<INITIAL,s_STRING>">="  { printf(">= "); }
<INITIAL,s_STRING>"<="  { printf("<= "); }
<INITIAL,s_STRING>"+="  { printf("+= "); }
<INITIAL,s_STRING>"-="  { printf("-= "); }
<INITIAL,s_STRING>"&="  { printf("&= "); }
<INITIAL,s_STRING>"|="  { printf("|= "); }
<INITIAL,s_STRING>"^="  { printf("|= "); }
<INITIAL,s_STRING>"*="  { printf("*= "); }
<INITIAL,s_STRING>"/="  { printf("/= "); }
<INITIAL,s_STRING>"%="  { printf("\%= "); }
<INITIAL,s_STRING>"="   { printf("= "); }
<INITIAL,s_STRING>"++"  { printf("++ "); }
<INITIAL,s_STRING>"--"  { printf("-- "); }
<INITIAL,s_STRING>"+"   { printf("+ "); }
<INITIAL,s_STRING>"-"   { printf("- "); }
<INITIAL,s_STRING>"*"   { printf("* "); }
<INITIAL,s_STRING>"/"   { printf("/ "); }
<INITIAL,s_STRING>"%"   { printf("%% "); }
<INITIAL,s_STRING>"&&"  { printf("&& "); }
<INITIAL,s_STRING>"&"   { printf("& "); }
<INITIAL,s_STRING>"||"  { printf("|| "); }
<INITIAL,s_STRING>"|"   { printf("| "); }
<INITIAL,s_STRING>">>>" { printf(">>> "); }
<INITIAL,s_STRING>">>"  { printf(">> "); }
<INITIAL,s_STRING>">"   { printf("> "); }
<INITIAL,s_STRING>"<<<" { printf("<<< "); }
<INITIAL,s_STRING>"<<"  { printf("<< "); }
<INITIAL,s_STRING>"<"   { printf("< "); }
<INITIAL,s_STRING>"^"   { printf("^ "); }
<INITIAL,s_STRING>"."   { printf(". "); }

    /* ------------------------- declare ---------------------------------- */

<INITIAL,s_STRING>println                { printf("PRINTFLN"); }
<INITIAL,s_STRING>0x[1-9ABCDFE]+         { printf("HEX_LITERAL: %f", atof(yytext));  }
<INITIAL,s_STRING>0[0-7]+                { printf("OCT_LITERAL: %i ", strtol(yytext, NULL, 8)); }
<INITIAL,s_STRING>([1-9][0-9]*)|0        { printf("INT_LITERAL: %d", atoi(yytext)); }
<INITIAL,s_STRING>[1-9][0-9]*\.[0-9]*    { printf("NUMBER_FLOAT "); }
<INITIAL,s_STRING>[1-9]e[0-9]*f          { printf("NUMBER_FLOAT "); }

<INITIAL,s_STRING>{_id_}                 { printf("ID "); }

<INITIAL,s_STRING>.                       {}
%%
