#include "structs.h"

struct Root CreateProgramm() {

}

struct Expression *createExprID(const char* name) {
    struct Expression *node = (struct Expression *)malloc(sizeof(struct Expression));
    node->String = name;
    node->type = expr_type.ID;
    crt->left = NULL;
    crt->next = NULL;
    crt->rigth = NULL;
    return node;
}

struct Expression *createExprInt(int val ) {
    struct Expression *node = (struct Expression *)malloc(sizeof(struct Expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->Conts_int = val;
    node->type = expr_type.ID;
}

struct Expression *createExprFLOAT(float val ) {
    struct Expression *node = (struct Expression *)malloc(sizeof(struct Expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->Conts_float = val;
    node->type = expr_type.ID;
}

struct Expression *createExprSTR(char* val ) {
    struct Expression *node = (struct Expression *)malloc(sizeof(struct Expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->String = val;
    node->type = expr_type.ID;
}

struct Expression *createExprBOOLEAN(bool val ) {
    struct Expression *node = (struct Expression *)malloc(sizeof(struct Expression));
    node->left = NULL;
    node->next = NULL;
    node->rigth = NULL;
    node->boolean = val;
    node->type = expr_type.ID;
}
