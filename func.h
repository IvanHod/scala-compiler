#ifndef FUNC
#define FUNC

#include <stdbool.h>
#include "structs.h"

char simbol;
char string[200];
char left_string[200];
char right_string[200];
bool isLeftIsString = true;
char multistring[500];

char * fromText(char* str) {
    char * res = (char *)malloc(sizeof(char)*1024);
    strcpy(res, str);
    return res;
}

struct Root* CreateProgramm(char* id, struct statement_list *stmt_list) {
    struct Root *node = (struct Root *)malloc(sizeof(struct Root));
    node->stmt_list = stmt_list;
    node->id = id;
    return node;
}

struct nclass* CreateClass( char* id, struct nargs* _nargs, struct statement_list* stmt_list  ) {
    struct nclass *node = (struct nclass *)malloc(sizeof(struct nclass));
    node->args = _nargs;
    node->name = id;
    node->stmt_list = stmt_list;
    return node;
}

struct expression *CreateExprID(const char* name) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->String = name;
    node->type = id;
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    return node;
}

struct expression *CreateExprINT(int val ) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->Int = val;
    node->type = Int;
    return node;
}

struct expression *CreateExprFLOAT(float val ) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->Float = val;
    node->type = Float;
    return node;
}

struct expression *CreateExprCHAR(char val ) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->Char = val;
    node->type = Char;
    return node;
}

struct expression *CreateExprSTR(char* val ) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->String = string;
    node->type = String;
    return node;
}

struct expression *createExprBOOLEAN(int val ) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->boolean = val;
    node->type = Int;
    return node;
}

struct expression *CreateExprNULL() {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->type = Null;
    return node;
}

struct expression *CreateExprInBraces(struct expression *expr) {
    return expr;
}

struct expression *CreateExprOperation(struct expression *expr_1, enum expr_type _type, struct expression *expr_2 ) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = expr_1;
    node->rigth = expr_2;
    node->type = _type;
    return node;
}

struct expression *CreateExprType(struct expression *expr ) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = expr;
    node->rigth = NULL;
    node->type = Type;
    return node;
}

struct expression *CreateExprCallFunc(char* id, struct expression_list *expr_list) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->String = id;
    node->expr_list = expr_list;
    node->type = call_func;
    return node;
}

struct expression *CreateExprPrintln( struct expression_list *expr_list) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = NULL;
    node->rigth = NULL;
    node->type = println;
    node->expr_list = expr_list;
    return node;
}

struct expression *CreateExprPrintln_s( struct hardString *str) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = NULL;
    node->rigth = NULL;
    node->type = println_s;
    node->hString = str;
    return node;
}

struct hardString * CreateHardString(struct hardString *prev, struct expression *expr) {
    struct hardString *node = (struct hardString *)malloc(sizeof(struct hardString ));
    node->left = fromText(left_string);
    node->right = fromText(right_string);
    strcpy(left_string, "");
    strcpy(right_string, "");
    node->expr = expr;
    node->next = prev;
    return node;
}

struct expression_list *CreateExprList( struct expression_list *expr_list , struct expression *expr) {
    struct expression_list *node = (struct expression_list *)malloc(sizeof(struct expression_list));
    node->expr = expr;
    node->next = NULL;
    if( expr_list != NULL ) {
        node->next = expr_list;
    }
    return node;
}

struct nif *CreateIfStmt( struct expression *expr, struct statement *stmt, struct statement *else_stmt) {
    struct nif *node = (struct nif *)malloc(sizeof(struct nif));
    node->expr = expr;
    node->to_then = stmt;
    node->to_else = else_stmt;
    return node;
}

struct nif_loop* CreateIfLoopExprList( struct nif_loop *parent, struct expression *expr ) {
    struct nif_loop *node = (struct nif_loop *)malloc(sizeof(struct nif_loop));
    if( parent == NULL ) {
        node->expr = expr;
        return node;
    } else {
        parent->next = node;
        parent->next->expr = expr;
        return parent->next;
    }
}

struct statement* CreateStmtExprList( struct expression_list *expr_list ) {
    struct statement *node = (struct statement *)malloc(sizeof(struct statement));
    node->expr_list = expr_list;
    node->type = EXPR_LIST;
    return node;
}

struct statement* CreateStmtStmtList( struct statement_list *stmt_list ) {
    struct statement *node = (struct statement *)malloc(sizeof(struct statement));
    node->stmt_list = stmt_list;
    node->type = STMT_LIST;
    return node;
}

struct statement* CreateStmtModifires( struct statement *stmt, enum modifires mod ) {
    stmt->mod = mod;
    return stmt;
}

struct statement* CreateStmtIf( struct nif *_nif ) {
    struct statement *node = (struct statement *)malloc(sizeof(struct statement));
    node->_if = _nif;
    node->type = NIF;
    return node;
}

struct statement* CreateStmtFor( char* id, struct expression *expr1, struct expression *expr2, struct nif_loop *IF, struct statement *stmt) {
    struct statement *node = (struct statement *)malloc(sizeof(struct statement));
    struct loop *_loop = (struct loop *)malloc(sizeof(struct loop));
    _loop->id = id;
    _loop->expr_1 = expr1;
    _loop->expr_2 = expr2;
    _loop->expr_while = NULL;
    _loop->stmt = stmt;
    _loop->type = CICLE_FOR;
    _loop->if_loop = IF;
    node->_loop = _loop;
    node->type = LOOP;
    return node;
}

struct statement* CreateStmtWhile( struct expression *expr, struct statement *stmt ) {
    struct statement *node = (struct statement *)malloc(sizeof(struct statement));
    struct loop *_loop = (struct loop *)malloc(sizeof(struct loop));
    _loop->expr_1 = NULL;
    _loop->expr_2 = NULL;
    _loop->expr_while = expr;
    _loop->stmt = stmt;
    _loop->type = CICLE_WHILE;
    _loop->if_loop = NULL;
    node->_loop = _loop;
    node->type = LOOP;
    return node;
}

struct statement* CreateStmtFunc( char* id, struct nargs *_nargs, struct statement *stmt, struct expression* return_val) {
    struct statement *node = (struct statement *)malloc(sizeof(struct statement));
    struct nfunc *func = (struct nfunc *)malloc(sizeof(struct nfunc));
    func->body = stmt;
    func->name = id;
    func->_args = _nargs;
    func->return_var = return_val;
    node->to_print_func = func;
    node->type = NFUNC;
    return node;
}

struct statement* CreateStmtDeclVar( struct nvar *_var) {
    struct statement *node = (struct statement *)malloc(sizeof(struct statement));
    node->to_print_var = _var;
    node->type = NVAR;
    return node;
}

struct statement* CreateStmtDeclVal( struct nval *_val) {
    struct statement *node = (struct statement *)malloc(sizeof(struct statement));
    node->to_print_val = _val;
    node->type = NVAL;
    return node;
}

struct statement* CreateStmtClass( struct nclass* _class) {
    struct statement *node = (struct statement *)malloc(sizeof(struct statement));
    node->to_print_class = _class;
    node->type = NCLASS;
    return node;
}

struct statement* CreateStmtObject( char* id, struct statement_list* stmt_list) {
    struct statement *node = (struct statement *)malloc(sizeof(struct statement));
    node->to_print_object = (struct nobject *)malloc(sizeof(struct nobject));
    node->to_print_object->name = id;
    node->to_print_object->stmt_list = stmt_list;
    node->type = NOBJECT;
    return node;
}

struct statement_list* CreateStmtList( struct statement_list* stmt_list, struct statement *stmt) {
    struct statement_list *node = (struct statement_list *)malloc(sizeof(struct statement_list));
    node->next = stmt_list;
    node->stmt = stmt;
    return node;
}

struct nargs* CreateFuncArgs( struct nargs* _nargs, char* id, struct expression *expr) {
    struct nargs *node = (struct nargs *)malloc(sizeof(struct nargs));
    node->expr = expr;
    node->id = id;
    node->next = _nargs;
    return node;
}

struct nvar* CreateDeclVar( struct id_list* idList, char* id, struct expression *expr) {
    struct nvar *node = (struct nvar *)malloc(sizeof(struct nvar));
    struct id_list *_id_list = idList == NULL
            ? (struct id_list *)malloc(sizeof(struct id_list))
        : idList;
    if( idList == NULL ) {
        _id_list->id = id;
        _id_list->next = NULL;
    }
    string;
    node->id_list = _id_list;
    node->array_expr_1 = NULL;
    node->array_expr_2 = NULL;
    node->result = expr;
    return node;
}

struct nvar* CreateDeclVarOfType( struct id_list* idList, char* id, struct expression * type, struct expression *expr) {
    struct nvar *node = (struct nvar *)malloc(sizeof(struct nvar));
    struct id_list *_id_list = idList == NULL
            ? (struct id_list *)malloc(sizeof(struct id_list))
        : idList;
    if( idList == NULL ) {
        _id_list->id = id;
        _id_list->next = NULL;
    }
    node->id_list = _id_list;
    node->return_value = type;
    node->result = expr;
    node->array_expr_1 = NULL;
    node->array_expr_2 = NULL;
    return node;
}

struct nvar* CreateDeclVarArrayNew( struct id_list* idList, struct expression* id, struct expression *type, struct expression *count) {
    struct nvar *node = (struct nvar *)malloc(sizeof(struct nvar));
    struct id_list *_id_list = idList == NULL
            ? (struct id_list *)malloc(sizeof(struct id_list))
        : idList;
    if( idList == NULL ) {
        _id_list->id = id;
        _id_list->next = NULL;
    }
    node->id_list = _id_list;
    node->array_expr_1 = type;
    node->array_expr_2 = count;
    return node;
}

struct nvar* CreateDeclVarArray( struct id_list* idList, char* id, struct expression_list *expr_list) {
    struct nvar *node = (struct nvar *)malloc(sizeof(struct nvar));
    struct id_list *_id_list = idList == NULL
            ? (struct id_list *)malloc(sizeof(struct id_list))
        : idList;
    if( idList == NULL ) {
        _id_list->id = id;
        _id_list->next = NULL;
    }
    node->id_list = _id_list;
    node->array_expr_list = expr_list;
    return node;
}

struct nval* CreateDeclVal( struct id_list* idList, char* id, struct expression *expr) {
    struct nval *node = (struct nval *)malloc(sizeof(struct nval));
    struct id_list *_id_list = idList == NULL
            ? (struct id_list *)malloc(sizeof(struct id_list))
        : idList;
    if( idList == NULL ) {
        _id_list->id = id;
        _id_list->next = NULL;
    }
    node->ids_list                                                                                                                                                                                                                                                                                                                                                                                                                                                                           = _id_list;
    node->expr = expr;
    return node;
}

struct id_list* CreateIdList( struct id_list* idList, char* id_1, char* id_2) {
    struct id_list *node = (struct id_list *)malloc(sizeof(struct id_list));
    if( idList == NULL) {
        node->id = id_1;
        struct id_list *node1 = (struct id_list *)malloc(sizeof(struct id_list));
        node1->id = id_2;
        node1->next = NULL;
        node->next = node1;
    } else {
        node->next = idList;
        node->next->id = id_1;
        node->next->next = NULL;
    }
    return node;
}
#endif
