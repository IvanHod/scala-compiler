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
        float float_const;
        char* string;
        int boolean;
        struct Expression *expr;
    };

%type <root>root
%type <expr>expr

%token<int> CONST_INT
%token<float> CONST_FLOAT CONST_DOUBLE
%token<string> CONST_STR
%token<boolean> TRUE FALSE
%token NULL_EXPR
%token PRIVATE PROTECTED PUBLIC
%token ARRAY
%token NEW STATIC THIS LAZY OBJECT EXTENDS CLASS OVERRIDE
%token WHILE FOR TO CASE MATCH IF ELSE LEFT_ARROW
%token PRINTLN
%token DEF VAL VAR
%token<string> ID
%token PRINTFLN

%right '='
%left PLUS_EQUAL MINUS_EQUAL MUL_EQUAL DIV_EQUAL DIV_WITH_REM_EQUAL AND_EQUAL OR_EQUAL
%left '<' '>' LESS_THAN MORE_THAN LESS_EQ_THAN MORE_EQ_THAN
%left AND
%left OR
%left '|'
%left'^'
%left'&'
%left EQUAL NOT_EQUAL
%left '+' '-'
%left '*' '/' '%'
%left '!'
%left UMINUS
%right PREFIX_INC PREFIX_DEC
%nonassoc '[' ']' ')'
%left POSTFIX_INC POSTFIX_DEC
%left '.'

%start root

%%
    root: OBJECT ID '(' stmt_list ')' { $$ = root = CreateProgramm( ); }
        ;

    class: ID '(' func_args ')' '{' stmt_list '}'
            ;

    expr: ID                            { $$ = CreateExprID( $1 ); }
        | CONST_INT                     { $$ = CreateExprINT($1); }
        | CONST_FLOAT                   { $$ = CreateExprFLOAT($1); }
        | CONST_DOUBLE                  { $$ = CreateExprFLOAT($1); }
        | CONST_STR                     { $$ = CreateExprSTR($1); }
        | TRUE                          { $$ = CreateExprBOOLEAN($1); }
        | FALSE                         { $$ = CreateExprBOOLEAN($1); }
        | NULL_EXPR                     { $$ = CreateExprNULL(); }
        | expr PLUS_EQUAL expr          { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr MINUS_EQUAL expr         { $$ = CreateExprOperation( $1, expr_type.minus_eq, $3); }
        | expr MUL_EQUAL expr           { $$ = CreateExprOperation( $1, expr_type.mul_eq, $3); }
        | expr DIV_EQUAL expr           { $$ = CreateExprOperation( $1, expr_type.div_eq, $3); }
        | expr DIV_WITH_REM_EQUAL expr  { $$ = CreateExprOperation( $1, expr_type.div_rem_eq, $3); }
        | expr AND_EQUAL expr           { $$ = CreateExprOperation( $1, expr_type.and_eq, $3); }
        | expr OR_EQUAL expr            { $$ = CreateExprOperation( $1, expr_type.or_eq, $3); }
        | expr '=' expr                 { $$ = CreateExprOperation( $1, expr_type.assigment, $3); }
        | expr '.' expr                 { $$ = CreateExprOperation( $1, expr_type.assigment, $3); }
        | expr '+' expr                 { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | '-' expr %prec UMINUS         { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr '-' expr                 { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr '*' expr                 { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr '/' expr                 { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr '^' expr                 { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr '%' expr                 { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr '&' expr                 { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr AND expr                 { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr '|' expr                 { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr OR expr                  { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr EQUAL expr               { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr NOT_EQUAL                { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr LESS_THAN expr           { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr MORE_THAN expr           { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr LESS_EQ_THAN expr        { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr MORE_EQ_THAN expr        { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | PREFIX_DEC expr               { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | PREFIX_INC expr               { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr POSTFIX_DEC              { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | expr POSTFIX_INC              { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | ID '(' ')'                    { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | ID '(' expr_list ')'          { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | PRINTFLN '(' expr ')'         { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | PRINTFLN '(' 'g' expr ')'     { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        | PRINTFLN '(' 'f' expr ')'     { $$ = CreateExprOperation( $1, expr_type.plus_eq, $3); }
        ;

    expr_list:  expr
        | expr_list ',' expr
        ;

    if_stmt: IF '(' expr ')' stmt
        | IF '(' expr ')' stmt ELSE stmt
        ;

    if_loop_expr: IF expr
        ;

    if_loop_expr_list: if_loop_expr
        | if_loop_expr_list ';' if_loop_expr
        ;

    stmt: expr_list ';'
        | '{' stmt_list '}'
        | if_stmt
        | FOR '(' expr LEFT_ARROW expr if_loop_expr_list ')' stmt
        | WHILE '(' expr_list ')' stmt
        | DEF ID '(' func_args ')' stmt
        | DEF ID '(' func_args ')' ':' ID '=' stmt
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
