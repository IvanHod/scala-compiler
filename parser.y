%{
#pragma warning(disable: 4996)
    #include <stdio.h>
    extern int yyerror(const char *msg);
/*extern int yylex();
    extern char* yytext;
*/

    //#define NODE( x , y ) new node(x, y) Для унарного минуса %prec UMINUS
%}

%union {
        char* text;
    };

%token CONST_INT CONST_FLOAT CONST_DOUBLE CONST_STR TRUE FALSE
%token NULL_EXPR
%token PRIVATE PROTECTED PUBLIC
%token ARRAY
%token NEW STATIC THIS LAZY OBJECT EXTENDS CLASS OVERRIDE
%token WHILE FOR FOREACH TO CASE MATCH IF ELSE
%token PRINTLN
%token DEF VAL VAR
%token<ch> ID

%right '='
%right PLUS_EQUAL MINUS_EQUAL MUL_EQUAL DIV_EQUAL DIV_WITH_REM_EQUAL AND_EQUAL OR_EQUAL LESS_EQUAL MORE_EQUAL IMP_EQUAL
%left '<' '>' LESS_THAN MORE_THAN LESS_EQ_THAN MORE_EQ_THAN
%left '+' '-' EQUAL NOT_EQUAL '|' OR '^' '*' '/' '%' '&' AND
%left INC DEC UMINUS
%left '!'
%nonassoc ')'

%start root

%%
    root: class_list
          ;

    class_list: class
            | class_list class
            ;

    class: OBJECT ID '{' stmt_list '}'
            ;

    expr_list: expr
        | expr_list expr
    ;

    expr: ID
          | CONST_INT
          | CONST_FLOAT
          | CONST_DOUBLE
          | CONST_STR
          | TRUE
          | FALSE
          | NULL_EXPR
          | expr '=' expr
          | expr '+' expr
          | expr '-' expr
          | expr '*' expr
          | expr '/' expr
          | expr '^' expr
          | expr '%' expr
          | expr '&' expr
          | expr AND expr
          | expr '|' expr
          | expr OR expr
          | expr EQUAL expr
          | expr NOT_EQUAL
          | expr LESS_THAN expr
          | expr MORE_THAN expr
          | expr LESS_EQ_THAN expr
          | expr MORE_EQ_THAN expr
          | DEC expr
          | expr DEC
          | INC expr
          | expr INC
          ;

    stmt_list: stmt
        | stmt_list stmt
    ;

    stmt: expr ';'
        | '{' stmt '}'
        | IF '(' expr_list ')' stmt_list ';'
        | IF '(' expr ')'
        ;

%%

int yyerror(const char *msg) {
    //throw msg;
    return -1;
}
