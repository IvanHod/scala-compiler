%option noyywrap never-interactive

%top {
    //#pragma warning(disable: 4996)
    #include <stdio.h>
    #include <stdlib.h>
    #include "bison.h"
    #include <stdbool.h>
    extern int yyparse(void);
    char * formText(char* str);


    extern simbol;
    extern string[200];
    extern multistring[500];

    extern left_string[200];
    extern right_string[200];
    extern isLeftIsString;
}

%{
%}

%x LINE_COMMENT
%x COMMENT
%x CHAR
%x STRING
%x MULT_STRING
%x s_STRING
%x s_id_STRING
%x s_str_STRING
%x f_STRING

_id_ [A-Za-z_][A-Za-z_0-9]*

%%

    /* ------------------------ commentaries rules ----------------------------- */

"//"                    { BEGIN(LINE_COMMENT);              }
"/*"                    { BEGIN(COMMENT);                   }

<LINE_COMMENT>\n        { BEGIN(INITIAL);                   }
<LINE_COMMENT>.*        {}

<COMMENT>"*/"           { BEGIN(INITIAL);                   }
<COMMENT>.              {}

    /* ------------------------ strings rules ---------------------------------- */

"'"                     { BEGIN(CHAR);                      }
"\""                    { strcpy(string, "");BEGIN(STRING); }
"\"\"\""                { BEGIN(MULT_STRING);               }

<CHAR>"'"               { BEGIN(INITIAL); return CONST_CHAR; }
<CHAR>.                 { simbol = yytext[0];               }
<STRING>"\\n"           { strcat(string, yytext);           }
<STRING>"\\f"           { strcat(string, yytext);           }
<STRING>"\\r"           { strcat(string, yytext);           }
<STRING>"\\b"           { strcat(string, yytext);           }
<STRING>"\\t"           { strcat(string, yytext);           }
<STRING>"\\\\"          { strcat(string, yytext);           }
<STRING>"\\\'"          { strcat(string, yytext);           }
<STRING>"\\\""          { strcat(string, yytext);           }
<STRING>"\""            { BEGIN(INITIAL); return CONST_STR; }
<STRING>.               { strcat(string, yytext);           }
<MULT_STRING>"\"\"\""   { BEGIN(INITIAL); return CONST_STR; }
<MULT_STRING>.          { strcat(multistring, yytext);      }

    /* ------------------------ s_STRING rules --------------------------------- */

"s\""                   { strcpy(left_string, "");strcpy(right_string, "");BEGIN(s_str_STRING);}

<s_str_STRING>\$\{      { BEGIN( s_STRING ); return CONST_S_STR;                    }
<s_str_STRING>\$        { BEGIN( s_id_STRING ); return CONST_S_STR;                 }
<s_id_STRING>{_id_}     { BEGIN(s_str_STRING); isLeftIsString = false;
        yylval.id = formText(yytext); return ID;                                    }
<s_str_STRING>"\""      { BEGIN(INITIAL); isLeftIsString = true; return CONST_S_STR; }

<s_STRING>"}"           { BEGIN( s_str_STRING ); }
<s_str_STRING>.         { strcat(isLeftIsString ? left_string : right_string, formText(yytext));           }

    /* ------------------------ f_STRING rules --------------------------------- */

"f\""                   { strcpy(string, ""); BEGIN(f_STRING); }

<f_STRING>\${_id_}      { yylval.id = formText(yytext); return ID; }
<f_STRING>%s            { printf("%s "); }
<f_STRING>%.f|d         { printf("%s "); }
<f_STRING>.             { strcat(isLeftIsString ? left_string : right_string, yytext);           }



    /* ------------------------ array initialize --------------------------------*/

"Array"                 { return ARRAY; }
"["                     { return '[';   }
"]"                     { return ']';   }

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
this                    { return THIS; }
lazy                    { return LAZY; }
new                     { return NEW; }
object                  { return OBJECT; }

    /* ----------------------- cycle keywords -------------------------------- */

while                   { return WHILE; }
for                     { return FOR; }
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
<INITIAL,s_STRING>"=>"  { return EQUAL_RIGHT; }
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
<INITIAL,s_STRING>"++"  { return INC; }
<INITIAL,s_STRING>"--"  { return DEC; }
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

<INITIAL,s_STRING>println                { return PRINTLN; }
<INITIAL,s_STRING>0x[1-9ABCDFE]+         { yylval.int_const = atof(yytext);return CONST_INT;  }
<INITIAL,s_STRING>0[0-7]+                { yylval.int_const = strtol(yytext, NULL, 8);return CONST_INT; }
<INITIAL,s_STRING>([1-9][0-9]*)|0        { yylval.int_const = strtol(yytext, NULL, 10); return CONST_INT; }
<INITIAL,s_STRING>[1-9][0-9]*\.[0-9]*    { yylval.float_const = atof(yytext); return CONST_FLOAT; }
<INITIAL,s_STRING>[1-9]e[0-9]*f          { yylval.float_const = atof(yytext); return CONST_FLOAT; }

<INITIAL,s_STRING>{_id_}                 { yylval.id = formText(yytext); return ID; }

<INITIAL,s_STRING>.                      {}
%%

char * formText(char* str) {
    char * res = (char *)malloc(sizeof(char)*1024);
    strcpy(res, str);
    return res;
}
