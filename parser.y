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
%token WHILE FOR TO CASE MATCH IF ELSE
%token PRINTLN
%token DEF VAL VAR
%token<ch> ID

%right '='
%left PLUS_EQUAL MINUS_EQUAL MUL_EQUAL DIV_EQUAL DIV_WITH_REM_EQUAL AND_EQUAL OR_EQUAL LESS_EQUAL MORE_EQUAL IMP_EQUAL
%left '<' '>' LESS_THAN MORE_THAN LESS_EQ_THAN MORE_EQ_THAN
%left AND OR '|' '^' '&' EQUAL NOT_EQUAL
%left '+' '-' '*' '/' '%'
%left '!'
%left UMINUS
%right PREFIX_INC PREFIX_DEC
%nonassoc '[' ']' '(' ')'
%left POSTFIX_INC POSTFIX_DEC

%start root

%%
    root: class_list
          ;

    class_list: class
            | class_list class
            ;

    class: OBJECT ID stmt_list
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
          | PREFIX_DEC expr
          | PREFIX_INC expr
          | expr POSTFIX_DEC
          | expr POSTFIX_INC
          | ID  '(' expr_list ')'
          ;

    expr_list:  simpl_stmt
                | expr_list ',' simpl_stmt
                ;

    simpl_stmt : expr;

    if_stmt: IF '(' simpl_stmt ')' '{' stmt_list '}'
        | IF '(' simpl_stmt ')' simpl_stmt
        | if_stmt ELSE IF '(' simpl_stmt ')' '{' stmt_list '}'
        | if_stmt ELSE IF '(' simpl_stmt ')' simpl_stmt
        | if_stmt  ELSE '{' stmt_list '}'
        | if_stmt  ELSE simpl_stmt
        ;

    stmt: simpl_stmt
        | if_stmt
        | FOR '(' simpl_stmt TO simpl_stmt ')' '{' stmt_list '}'
        | FOR '(' simpl_stmt TO simpl_stmt ')' simpl_stmt
        ;

    stmt_list: stmt
        | stmt_list stmt
    ;

%%

int yyerror(const char *msg) {
    //throw msg;
    return -1;
}
