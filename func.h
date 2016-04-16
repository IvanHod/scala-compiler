#include "structs.h"

struct Root* CreateProgramm(char* id, struct statement_list *stmt_list) {
    struct Root *node = (struct Root *)malloc(sizeof(struct Root));
    return node;
}

struct expression *createExprID(const char* name) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->String = name;
    node->type = expr_type.ID;
    crt->left = NULL;
    crt->next = NULL;
    crt->rigth = NULL;
    return node;
}

struct expression *CreateExprINT(int val ) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->Conts_int = val;
    node->type = expr_type.ID;
    return node;
}

struct expression *CreateExprFLOAT(float val ) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->Conts_float = val;
    node->type = expr_type.ID;
    return node;
}

struct expression *CreateExprSTR(char* val ) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->String = val;
    node->type = expr_type.ID;
    return node;
}

struct expression *createExprBOOLEAN(bool val ) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->boolean = val;
    node->type = expr_type.ID;
    return node;
}

struct expression *CreateExprNULL() {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    return node;
}

struct expression *CreateExprOperation(struct expression* expr_1, enum expr_type, struct expression* expr_1 ) {
    struct expression *node = (struct expression *)malloc(sizeof(struct expression));
    return node;
}
