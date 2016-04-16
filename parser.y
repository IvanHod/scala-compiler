%{
#include <stdio.h>
#include <malloc.h>
#include "structs.h"
#include "func.h"

    void yyerror(char const *s);
    extern int yylex(void);

%}

%union {
        int int_const;
        float float_const;
        char* string;
        int boolean;
        struct expression *expr;
        struct expression_list *expr_list;
        struct statement *stmt;
        struct statement_list *stmt_list;
        struct nif *if_stmt;
        struct nargs *func_args;
        struct nif_loop  *if_loop_expr_list;
        struct nclass *Class;
        struct nvar *decl_var;
        struct nval *decl_val;
        struct ids *ids_list;
        struct Root *root;
    };

%type <root> root
%type <expr> expr
%type <expr_list> expr_list
%type <stmt> stmt
%type <stmt_list> stmt_list
%type <if_stmt> if_stmt
%type <func_args> func_args
%type <if_loop_expr_list> if_loop_expr_list
%type <Class> Class
%type <decl_var> decl_var
%type <decl_val> decl_val
%type <ids_list> ids_list

%token<int_const> CONST_INT
%token<float_const> CONST_FLOAT CONST_DOUBLE
%token<string> CONST_STR
%token<boolean> TRUE FALSE
%token NULL_EXPR
%token PRIVATE PROTECTED PUBLIC
%token ARRAY
%token NEW STATIC THIS LAZY OBJECT EXTENDS CLASS OVERRIDE RETURN
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
%nonassoc ']' ')'
%left POSTFIX_INC POSTFIX_DEC
%left '.'

%start root

%%
    root: OBJECT ID '(' stmt_list ')' { $$ = CreateProgramm( $2, $4 ); }
        ;

    Class: ID '(' func_args ')' '{' stmt_list '}' { $$ = CreateClass($1, $3, $6 ); }
        ;

    expr: ID                            { $$ = CreateExprID( $1 );printf("Id"); }
        | CONST_INT                     { $$ = CreateExprINT($1); }
        | CONST_FLOAT                   { $$ = CreateExprFLOAT($1); }
        | CONST_DOUBLE                  { $$ = CreateExprFLOAT($1); }
        | CONST_STR                     { $$ = CreateExprSTR($1); }
        | TRUE                          { $$ = CreateExprBOOLEAN($1); }
        | FALSE                         { $$ = CreateExprBOOLEAN($1); }
        | NULL_EXPR                     { $$ = CreateExprNULL(); }
        | expr PLUS_EQUAL expr          { $$ = CreateExprOperation( $1, exprType.plus_eq, $3); }
        | expr MINUS_EQUAL expr         { $$ = CreateExprOperation( $1, exprType.minus_eq, $3); }
        | expr MUL_EQUAL expr           { $$ = CreateExprOperation( $1, exprType.mul_eq, $3); }
        | expr DIV_EQUAL expr           { $$ = CreateExprOperation( $1, exprType.div_eq, $3); }
        | expr DIV_WITH_REM_EQUAL expr  { $$ = CreateExprOperation( $1, exprType.div_rem_eq, $3); }
        | expr AND_EQUAL expr           { $$ = CreateExprOperation( $1, exprType.and_eq, $3); }
        | expr OR_EQUAL expr            { $$ = CreateExprOperation( $1, exprType.or_eq, $3); }
        | expr '=' expr                 { $$ = CreateExprOperation( $1, exprType.assigment, $3); }
        | expr '.' expr                 { $$ = CreateExprOperation( $1, exprType.point, $3); }
        | expr '+' expr                 { $$ = CreateExprOperation( $1, exprType.plus, $3); }
        | '-' expr %prec UMINUS         { $$ = CreateExprOperation( $2, exprType.uminus, NULL); }
        | expr '-' expr                 { $$ = CreateExprOperation( $1, exprType.minus, $3); }
        | expr '*' expr                 { $$ = CreateExprOperation( $1, exprType.mul, $3); }
        | expr '/' expr                 { $$ = CreateExprOperation( $1, exprType.div, $3); }
        | expr '^' expr                 { $$ = CreateExprOperation( $1, exprType.XOR, $3); }
        | expr '%' expr                 { $$ = CreateExprOperation( $1, exprType.div_residue, $3); }
        | expr '&' expr                 { $$ = CreateExprOperation( $1, exprType.unarAND, $3); }
        | expr AND expr                 { $$ = CreateExprOperation( $1, exprType.AND, $3); }
        | expr '|' expr                 { $$ = CreateExprOperation( $1, exprType.unarOR, $3); }
        | expr OR expr                  { $$ = CreateExprOperation( $1, exprType.unarOR, $3); }
        | expr EQUAL expr               { $$ = CreateExprOperation( $1, exprType.equal, $3); }
        | expr NOT_EQUAL                { $$ = CreateExprOperation( $1, exprType.no_equal, NULL); }
        | expr LESS_THAN expr           { $$ = CreateExprOperation( $1, exprType.less, $3); }
        | expr MORE_THAN expr           { $$ = CreateExprOperation( $1, exprType.more, $3); }
        | expr LESS_EQ_THAN expr        { $$ = CreateExprOperation( $1, exprType.less_eq, $3); }
        | expr MORE_EQ_THAN expr        { $$ = CreateExprOperation( $1, exprType.more_eq, $3); }
        | PREFIX_DEC expr               { $$ = CreateExprOperation( $2, exprType.prefix_dec, NULL); }
        | PREFIX_INC expr               { $$ = CreateExprOperation( $2, exprType.prefix_inc, NULL); }
        | expr POSTFIX_DEC              { $$ = CreateExprOperation( $1, exprType.postfix_dec, NULL); }
        | expr POSTFIX_INC              { $$ = CreateExprOperation( $1, exprType.postfix_inc, NULL); }
        | ID '(' ')'                    { $$ = CreateExprCallFunc( $1 ); }
        | ID '(' expr_list ')'          { $$ = CreateExprCallFunc( $1, $3 ); }
        | PRINTFLN '(' ')'              { $$ = CreateExprPrintln( exprType.println ); }
        | PRINTFLN '(' expr ')'         { $$ = CreateExprPrintln( exprType.println, $3 ); }
        | PRINTFLN '(' 'g' expr ')'     { $$ = CreateExprPrintln( exprType.println, $4 ); }
        | PRINTFLN '(' 'f' expr ')'     { $$ = CreateExprPrintln( exprType.println, $4 ); }
        ;

    expr_list:  expr                    { $$ = CreateExprList( $1, NULL ); }
        | expr_list ',' expr            { $$ = CreateExprList( $1, $3 ); }
        ;

    if_stmt: IF '(' expr ')' stmt        { $$ = CreateIfStmt( $3, $5, NULL ); }
        | IF '(' expr ')' stmt ELSE stmt { $$ = CreateIfStmt( $3, $5, $7 ); }
        ;

    if_loop_expr_list: IF expr          { $$ = CreateIfLoopExprList( $2 ); }
        | if_loop_expr_list ';' IF expr { $$ = CreateIfLoopExprList( $1, $4 ); }
        ;

    stmt: expr_list ';'                 { $$ = CreateStmtExprList($1); }
        | '{' stmt_list '}'             { $$ = CreateStmtExprList($2); }
        | if_stmt                       { $$ = CreateStmtIf( $1 ); }
        | FOR '(' expr LEFT_ARROW expr ')' stmt   { $$ = CreateStmtFor($3, $5, $7, NULL); }
        | FOR '(' expr LEFT_ARROW expr if_loop_expr_list ')' stmt   { $$ = CreateStmtFor($3, $5, $8, $6); }
        | WHILE '(' expr_list ')' stmt  { $$ = CreateStmtWhile( $3, $5 ); }
        | DEF ID '(' func_args ')' stmt { $$ = CreateStmtFunc($2, $4, $6); }
        | DEF ID '(' func_args ')' ':' ID '=' stmt { $$ = CreateStmtFunc($2, $4, $9, $7); }
        | decl_var                      { $$ = CreateStmtDeclVar($1); }
        | decl_val                      { $$ = CreateStmtDeclVar($1); }
        | Class                         { $$ = CreateStmtClass($1); }
        ;

    stmt_list: stmt                     { $$ = CreateStmtList($1, NULL); }
        | stmt_list stmt                { $$ = CreateStmtList($1, $2); }
        ;

    func_args: ID ':' expr              { $$ = CreateFuncArgs( NULL, $1, $3 ); }
        | func_args ',' ID ':' expr     { $$ = CreateFuncArgs( $1, $3, $5); }
        ;

    decl_var: VAR ids_list              { $$ = CreateDeclVar($2); }
        | VAR ids_list '=' expr ';'     { $$ = CreateDeclVar($2, $4); }
        | VAR ids_list ':' ID '=' expr ';' { $$ = CreateDeclVar($2, $4, $6); }
        | VAR ids_list '=' NEW ARRAY '[' expr ']' '(' expr ')' { $$ = CreateDeclVarArray($2, $7); }
        | VAR ids_list '=' ARRAY '(' expr_list ')' { $$= CreateDeclVarArray($2, $6); }
        ;

    decl_val: VAL ID ';'                { $$ = CreateDeclVal($2); }
        | VAL ids_list ';'              { $$ = CreateDeclVal($2); }
        | VAL ids_list '=' expr ';'     { $$ = CreateDeclVal($2, $4); }
        ;

    ids_list: ID ',' ID                 { $$ = CreateIdsList($1, $3); }
        | ids_list ',' ID               { $$ = CreateIdsList($1, $3); }
        ;


%%

int yyerror(const char *msg) {
    printf("%s", msg);
}
