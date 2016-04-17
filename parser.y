%{
#include <stdio.h>
#include <malloc.h>
#include "structs.h"
#include "func.h"

int yyerror(const char *msg);

%}

%union {
    int int_const;
    float float_const;
    char* string;
    char* id;
    int boolean;
    struct expression *_expr;
    struct expression_list *_expr_list;
    struct statement *_stmt;
    struct statement_list *_stmt_list;
    struct nif *_if_stmt;
    struct nargs *_func_args;
    struct nif_loop *_if_loop_expr_list;
    struct nclass *_Class;
    struct nvar *_decl_var;
    struct nval *_decl_val;
    struct ids *_id_list;
    struct Root *_root;
};

%error-verbose

%type <_root> root
%type <_expr> expr
%type <_expr_list> expr_list
%type <_stmt> stmt
%type <_stmt_list> stmt_list
%type <_if_stmt> if_stmt
%type <_func_args> func_args
%type <_if_loop_expr_list> if_loop_expr_list
%type <_Class> Class
%type <_decl_var> decl_var
%type <_decl_val> decl_val
%type <_id_list> id_list

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
%token<id> ID
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


%%
    root: OBJECT ID '{' stmt_list '}'   { $$ = root = CreateProgramm( $2, $4 );             }
        ;

    Class: ID '(' func_args ')' '{' stmt_list '}' { $$ = CreateClass($1, $3, $6 );          }
        ;

    expr: ID                            { $$ = CreateExprID( $1 );                          }
        | CONST_INT                     { $$ = CreateExprINT($1);                           }
        | CONST_FLOAT                   { $$ = CreateExprFLOAT($1);                         }
        | CONST_DOUBLE                  { $$ = CreateExprFLOAT($1);                         }
        | CONST_STR                     { $$ = CreateExprSTR($1);                           }
        | TRUE                          { $$ = CreateExprINT($1);                           }
        | FALSE                         { $$ = CreateExprINT($1);                           }
        | NULL_EXPR                     { $$ = CreateExprNULL();                            }
        | expr PLUS_EQUAL expr          { $$ = CreateExprOperation( $1, plus_eq, $3);       }
        | expr MINUS_EQUAL expr         { $$ = CreateExprOperation( $1, sub_eq, $3);        }
        | expr MUL_EQUAL expr           { $$ = CreateExprOperation( $1, mul_eq, $3);        }
        | expr DIV_EQUAL expr           { $$ = CreateExprOperation( $1, div_eq, $3);        }
        | expr DIV_WITH_REM_EQUAL expr  { $$ = CreateExprOperation( $1, div_residue, $3);   }
        | expr AND_EQUAL expr           { $$ = CreateExprOperation( $1, and_equal, $3);     }
        | expr OR_EQUAL expr            { $$ = CreateExprOperation( $1, or_equal, $3);      }
        | expr '=' expr                 { $$ = CreateExprOperation( $1, assigment, $3);     }
        | expr '.' expr                 { $$ = CreateExprOperation( $1, point, $3);         }
        | expr '+' expr                 { $$ = CreateExprOperation( $1, plus, $3);          }
        | '-' expr %prec UMINUS         { $$ = CreateExprOperation( $2, usub, NULL);        }
        | expr '-' expr                 { $$ = CreateExprOperation( $1, sub, $3);           }
        | expr '*' expr                 { $$ = CreateExprOperation( $1, mul, $3);           }
        | expr '/' expr                 { $$ = CreateExprOperation( $1, mul, $3);           }
        | expr '^' expr                 { $$ = CreateExprOperation( $1, _xor, $3);          }
        | expr '%' expr                 { $$ = CreateExprOperation( $1, div_residue, $3);   }
        | expr '&' expr                 { $$ = CreateExprOperation( $1, unarAND, $3);       }
        | expr AND expr                 { $$ = CreateExprOperation( $1, _and, $3);          }
        | expr '|' expr                 { $$ = CreateExprOperation( $1, unarOR, $3);        }
        | expr OR expr                  { $$ = CreateExprOperation( $1, _or, $3);           }
        | expr EQUAL expr               { $$ = CreateExprOperation( $1, equal, $3);         }
        | expr NOT_EQUAL                { $$ = CreateExprOperation( $1, no_equal, NULL);    }
        | expr LESS_THAN expr           { $$ = CreateExprOperation( $1, less, $3);          }
        | expr MORE_THAN expr           { $$ = CreateExprOperation( $1, more, $3);          }
        | expr LESS_EQ_THAN expr        { $$ = CreateExprOperation( $1, less_eq, $3);       }
        | expr MORE_EQ_THAN expr        { $$ = CreateExprOperation( $1, more_eq, $3);       }
        | PREFIX_DEC expr               { $$ = CreateExprOperation( $2, prefix_dec, NULL);  }
        | PREFIX_INC expr               { $$ = CreateExprOperation( $2, prefix_inc, NULL);  }
        | expr POSTFIX_DEC              { $$ = CreateExprOperation( $1, postfix_dec, NULL); }
        | expr POSTFIX_INC              { $$ = CreateExprOperation( $1, postfix_inc, NULL); }
        | ARRAY '[' expr ']'            { $$ = CreateExprType( $3 ); }
        | ID '(' ')'                    { $$ = CreateExprCallFunc( $1, NULL );              }
        | ID '(' expr_list ')'          { $$ = CreateExprCallFunc( $1, $3 );                }
        | PRINTFLN '(' ')'              { $$ = CreateExprPrintln( println, NULL );          }
        | PRINTFLN '(' expr ')'         { $$ = CreateExprPrintln( println, $3 );            }
        | PRINTFLN '(' 'g' expr ')'     { $$ = CreateExprPrintln( println, $4 );            }
        | PRINTFLN '(' 'f' expr ')'     { $$ = CreateExprPrintln( println, $4 );            }
        ;

    expr_list:  expr                    { $$ = CreateExprList( $1, NULL );                  }
        | expr_list ',' expr            { $$ = CreateExprList( $1, $3 );                    }
        ;

    if_stmt: IF '(' expr ')' stmt       { $$ = CreateIfStmt( $3, $5, NULL );                }
        |IF '(' expr ')' stmt ELSE stmt { $$ = CreateIfStmt( $3, $5, $7 );                  }
        ;

    if_loop_expr_list: IF expr          { $$ = CreateIfLoopExprList( NULL, $2 );            }
        | if_loop_expr_list ';' IF expr { $$ = CreateIfLoopExprList( $1, $4 );              }
        ;

    stmt: expr_list ';'                 { $$ = CreateStmtExprList($1);                      }
        | '{' stmt_list '}'             { $$ = CreateStmtStmtList($2);                      }
        | if_stmt                       { $$ = CreateStmtIf( $1 );                          }
        | FOR '(' expr LEFT_ARROW expr ')' stmt               { $$ = CreateStmtFor($3, $5, $7, NULL);             }
        | FOR '(' expr LEFT_ARROW expr if_loop_expr_list ')' stmt  { $$ = CreateStmtFor($3, $5, $8, $6);               }
        | WHILE '(' expr_list ')' stmt  { $$ = CreateStmtWhile( $3, $5 );                   }
        | DEF ID '(' func_args ')' stmt { $$ = CreateStmtFunc($2, $4, $6, NULL);            }
        | DEF ID '(' func_args ')' ':' ID '=' stmt             { $$ = CreateStmtFunc($2, $4, $9, $7);              }
        | decl_var                      { $$ = CreateStmtDeclVar($1);                       }
        | decl_val                      { $$ = CreateStmtDeclVar($1);                       }
        | Class                         { $$ = CreateStmtClass($1);                         }
        ;

    stmt_list: stmt                     { $$ = CreateStmtList(NULL, $1);                    }
        | stmt_list stmt                { $$ = CreateStmtList($1, $2);                      }
        ;

    func_args: ID ':' expr              { $$ = CreateFuncArgs( NULL, $1, $3 );              }
        | func_args ',' ID ':' expr     { $$ = CreateFuncArgs( $1, $3, $5);                 }
        ;

    decl_var: VAR ID ';'                { $$ = CreateDeclVar(NULL, $2, NULL);               }
        | VAR id_list ';'               { $$ = CreateDeclVar($2, NULL, NULL);               }
        | VAR ID '=' expr ';'           { $$ = CreateDeclVar(NULL, $2, $4);                 }
        | VAR id_list '=' expr ';'      { $$ = CreateDeclVar($2, NULL, $4);                 }
        | VAR ID ':' ID '=' expr ';'    { $$ = CreateDeclVarOfType(NULL, $2, $4, $6);       }
        | VAR id_list ':' ID '=' expr ';'             { $$ = CreateDeclVarOfType($2, NULL, $4, $6);       }
        | VAR ID '=' NEW ARRAY '[' expr ']' '(' expr ')' ';'   { $$ = CreateDeclVarArrayNew(NULL, $2, $7, $10);    }
        | VAR id_list '=' NEW ARRAY '[' expr ']' '(' expr ')' ';'   { $$ = CreateDeclVarArrayNew($2, NULL, $7, $10);    }
        | VAR ID '=' ARRAY '(' expr_list ')' ';'       { $$ = CreateDeclVarArray(NULL, $2, $6);            }
        | VAR id_list '=' ARRAY '(' expr_list ')' ';'       { $$ = CreateDeclVarArray($2, NULL, $6);            }
        ;

    decl_val: VAL ID ';'                { $$ = CreateDeclVal(NULL, $2, NULL);               }
        | VAL id_list ';'               { $$ = CreateDeclVal($2, NULL, NULL);               }
        | VAL ID '=' expr ';'           { $$ = CreateDeclVal(NULL, $2, $4);                 }
        | VAL id_list '=' expr ';'      { $$ = CreateDeclVal($2, NULL, $4);                 }
        ;

    id_list: ID ',' ID                  { $$ = CreateIdList(NULL, $1, $3);                  }
        | id_list ',' ID                { $$ = CreateIdList($1, $3, NULL);                  }
        ;


%%

int yyerror(const char *msg) {
     printf("%s\n", msg);
}
