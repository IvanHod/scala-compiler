%{
#pragma warning(disable: 4996)
    #include <stdio.h>
    extern int yyerror(const char *msg);
/*extern int yylex();
    extern char* yytext;
*/

    //#define NODE( x , y ) new node(x, y) Для унарного минуса %prec UMINUS
%}

%token CONST_INT CONST_FLOAT CONST_DOUBLE CONST_STR TRUE FALSE
%token _NULL NOTHING ANY ANYREF UNIT
%token PRIVATE PROTECTED PUBLIC
%token ARRAY
%token NEW STATIC THIS LAZY OBJECT EXTENDS CLASS OVERRIDE
%token WHILE FOR FOREACH TO CASE MATCH IF ELSE
%token PRINTLN
%token DEF VAL VAR
%token ID

%right '='
%left LESS_THAN MORE_THAN LESS_EQ_THAN MORE_EQ_THAN
%left '+' '-' EQUAL '|' OR '*' '/' '%' '&' AND
%left INC DEC UMINUS
%right '!'
%nonassoc ')'

%%

identifier: ID

%%

int yyerror(const char *msg) {
    //throw msg;
    return -1;
}
