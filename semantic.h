#ifndef SEMANTIC
#define SEMANTIC

#include <stdbool.h>
#include "structs.h"

enum ConstantType {
    CONSTANT_Utf8 = 1,
    CONSTANT_Integer = 3,
    CONSTANT_Float = 4,
    CONSTANT_String = 8,
    CONSTANT_NameAndType = 12,
    CONSTANT_Class = 7,
    CONSTANT_Fieldref = 9,
    CONSTANT_Methodref = 10
};

struct Constant {
    enum ConstantType type;
    int intValue;
    float floatValue;
    char* utf8;
    int id;
    //constant for class name, in case method references or field references
    struct Constant* const1;
    //constant type name and type, in case method references or field references
    struct Constant* const2;
};

struct LocalVariable {
    int id;
    struct SemanticType* semanticType;
    char* name;
    int scope;
    bool isActive;
    bool isMutable;
};

struct Field {
    struct Constant* constFieldref;
    struct SemanticType* type;
    int id;
    bool isMutable;
};

struct Method {
    //method name, because class is unique
    struct Constant* constMethodref;
    //key: varname, value : LocalVariable
    struct SemanticType* returnType;
    List* localVariablesTable;
    struct ParameterList* paramList;
};

struct Class {
    char* className;
    //node of list : struct Constant*
    List* constantsTable;
    //key: char*, value: Field*;
    HashTable* fieldsTable;
    //key: char*, value: struct Method* ;
    HashTable* methodsTable;
};

void check_stmt_list(struct statement_list *_stmt_list);
void check_expr_list(struct expression_list *_expr_list);
void check_stmt(struct statement *stmt);
void check_expr(struct expression *expr);
void check_if( struct nif *_nif );
void check_var(struct nvar *var);
void check_val(struct nval *val);
void check_loop(struct loop* _loop);
void check_id_list(struct id_list *_id_list);
void check_args(struct nargs *_args);

#endif
