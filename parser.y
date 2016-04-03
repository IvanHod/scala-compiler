%{
#pragma warning(disable: 4996)
    #include <stdio.h>
    #include "structs.h"
    extern int yyerror(const char *msg);
    extern int yylex();
    extern char* yytext;

%}

%union {
        int int_const;
        char* string;
    };

%type <root>root

%token CONST_INT CONST_FLOAT CONST_DOUBLE CONST_STR TRUE FALSE
%token NULL_EXPR
%token PRIVATE PROTECTED PUBLIC
%token ARRAY
%token NEW STATIC THIS LAZY OBJECT EXTENDS CLASS OVERRIDE
%token WHILE FOR TO CASE MATCH IF ELSE
%token PRINTLN
%token DEF VAL VAR
%token<ch> ID

%right '='
%left PLUS_EQUAL MINUS_EQUAL MUL_EQUAL DIV_EQUAL DIV_WITH_REM_EQUAL AND_EQUAL OR_EQUAL
%left '<' '>' LESS_THAN MORE_THAN LESS_EQ_THAN MORE_EQ_THAN
%left AND OR '|' '^' '&' EQUAL NOT_EQUAL
%left '+' '-' '*' '/' '%'
%left '!'
%left UMINUS
%right PREFIX_INC PREFIX_DEC
%nonassoc '[' ']' ')'
%left POSTFIX_INC POSTFIX_DEC

%start root

%%
    root: OBJECT ID '(' stmt_list ')' { $$ = root = CreateProgramm(); }
          ;

    class: ID '(' func_args ')' '{' stmt_list '}'
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
        | expr PLUS_EQUAL expr
        | expr MINUS_EQUAL expr
        | expr MUL_EQUAL expr
        | expr DIV_EQUAL expr
        | expr DIV_WITH_REM_EQUAL expr
        | expr AND_EQUAL expr
        | expr OR_EQUAL expr
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
        | PREFIX_DEC expr
        | PREFIX_INC expr
        | expr POSTFIX_DEC
        | expr POSTFIX_INC
        | ID  '(' expr_list ')'
        ;

    expr_list:  expr
                | expr_list ',' expr
                ;

    simpl_stmt : expr;

    if_stmt: IF '(' expr ')' '{' stmt_list '}'
        | IF '(' expr ')' simpl_stmt
        | if_stmt ELSE IF '(' expr ')' '{' stmt_list '}'
        | if_stmt ELSE IF '(' expr ')' simpl_stmt
        | if_stmt  ELSE '{' stmt_list '}'
        | if_stmt  ELSE simpl_stmt
        ;

    stmt: expr ';'
        | if_stmt
        | FOR '(' expr TO expr ')' '{' stmt_list '}'
        | FOR '(' expr TO expr ')' stmt
        | FOR '(' expr TO '(' expr ',' expr ')' ')' '{' stmt_list '}'
        | FOR '(' expr TO '(' expr ',' expr ')' ')' stmt
        | WHILE '(' expr_list ')' '{' stmt_list '}'
        | WHILE '(' expr_list ')' stmt
        | DEF ID '(' func_args ')' stmt
        | DEF ID '(' func_args ')' ':' ID '=' stmt
        | DEF ID '(' func_args ')' '{' stmt_list '}'
        | decl_var
        | decl_val
        | class
        ;

    stmt_list: stmt
        | stmt_list stmt
    ;

    func_args: ID ':' expr
        | func_args ',' ID ':' expr
        ;

    decl_var: VAR ids_list
        | VAR ids_list '=' expr ';'
        | VAR ids_list ':' ID '=' expr ';'
        | VAR ids_list '=' NEW ARRAY '[' ID ']' '(' expr ')'
        | VAR ids_list '=' ARRAY '(' expr_list ')'
    ;

    decl_val: VAL ID ';'
        | VAL ids_list ';'
        | VAL ids_list '=' expr ';'
        ;

    ids_list: ID ',' ID
        | ids_list ',' ID
        ;


%%

int yyerror(const char *msg) {
    //throw msg;
    return -1;
}
