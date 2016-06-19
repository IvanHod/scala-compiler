#ifndef FUNC
#define FUNC

#include <stdbool.h>
#include <stdlib.h>
#include "structs.h"

char simbol;
char string[200];
char left_string[200];
char right_string[200];
char multistring[500];

char * fromText(char* str);

struct Root* CreateProgramm(char* id, struct statement_list *stmt_list);
struct nclass* CreateClass( char* id, struct nargs* _nargs, struct statement_list* stmt_list  );
struct expression *CreateExprID(const char* name);
struct expression *CreateExprINT(int val );
struct expression *CreateExprFLOAT(float val );
struct expression *CreateExprCHAR(char val );
struct expression *CreateExprSTR(char* val );
struct expression *createExprBOOLEAN(bool val );
struct expression *CreateExprNULL();
struct expression *CreateExprInBraces(struct expression *expr);
struct expression *CreateExprOperation(struct expression *expr_1, enum expr_type _type, struct expression *expr_2 );
struct expression *CreateExprType(struct expression *expr );
struct expression *CreateExprCallFunc(char* id, struct expression_list *expr_list);
struct expression *CreateExprPrintln( struct expression_list *expr_list);
struct expression *CreateExprPrintln_s( struct hardString *str);
struct hardString * CreateHardString(struct hardString *prev, struct expression *expr);
struct expression_list *CreateExprList( struct expression_list *expr_list , struct expression *expr);
struct nif *CreateIfStmt( struct expression *expr, struct statement *stmt, struct statement *else_stmt);
struct nif_loop* CreateIfLoopExprList( struct nif_loop *parent, struct expression *expr );
struct statement* CreateStmtExprList( struct expression_list *expr_list );
struct statement* CreateStmtStmtList( struct statement_list *stmt_list );
struct statement* CreateStmtModifires( struct statement *stmt, enum modifires mod );
struct statement* CreateStmtIf( struct nif *_nif );
struct statement* CreateStmtFor( char* id, struct expression *expr1,
                                 struct expression *expr2, struct nif_loop *IF, struct statement *stmt);
struct statement* CreateStmtWhile( struct expression *expr, struct statement *stmt );
struct statement* CreateStmtFunc( char* id, struct nargs *_nargs, struct statement *stmt, struct expression* return_val);
struct statement* CreateStmtDeclVar( struct nvar *_var);
struct statement* CreateStmtDeclVal( struct nval *_val);
struct statement* CreateStmtMatch( struct match *_match);
struct statement* CreateStmtClass( struct nclass* _class);
struct statement* CreateStmtObject( char* id, struct statement_list* stmt_list);
struct statement_list* CreateStmtList( struct statement_list* stmt_list, struct statement *stmt);
struct case_list * CreateCaseList(struct case_list *list, struct expression* expr1, struct expression *expr2) ;
struct match * CreateMatch(struct expression *expr, struct case_list *caseList);
struct nargs* CreateFuncArgs( struct nargs* _nargs, char* id, struct expression *expr);
struct nvar* CreateDeclVar( struct id_list* idList, char* id, struct expression *expr);
struct nvar* CreateDeclVarOfType( struct id_list* idList, char* id, struct expression * type, struct expression *expr);
struct nvar* CreateDeclVarArrayNew( struct id_list* idList, struct expression* id, struct expression *type, struct expression *count);
struct nvar* CreateDeclVarArray( struct id_list* idList, char* id, struct expression_list *expr_list);
struct nval* CreateDeclVal( struct id_list* idList, char* id, struct expression *expr);
struct nval* CreateDeclValOfType( struct id_list* idList, char* id, struct expression * type, struct expression *expr);
struct id_list* CreateIdList( struct id_list* idList, char* id_1, char* id_2);

#endif
