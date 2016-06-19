#ifndef SEMANTIC
#define SEMANTIC

#include <stdio.h>
#include "structs.h"
#include "collection/list.h"
#include "collection/hashtable.h"
#include "func.h"

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
    struct nargs* paramList;
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

bool doSemantic(struct Root* root);
bool check_stmt_list(struct statement_list *_stmt_list, struct Method* method);
bool check_expr_list(struct expression_list *_expr_list, struct Method* method);
bool check_stmt(struct statement *stmt, struct Method* method);
bool check_expr(struct expression *expr, struct Method* method);
bool check_if( struct nif *_nif, struct Method* method);
bool check_var(struct nvar *var, struct Method* method);
bool check_val(struct nval *val, struct Method* method);
bool check_loop(struct loop* _loop, struct Method* method);
bool check_args(struct nargs *_args, struct Method* method, char* functionName);
bool check_func(struct nfunc *func);

//bool addLocalVariableToTable(struct VarDecl* varDecl, struct Method* method);
struct LocalVariable* findActiveLocalVariableByScope(List* variablesTable, char* varName, int scope);
//for tree printing purpose
struct LocalVariable* findLocalVariableByScope(List* variablesTable, char* varName, int scope);
struct LocalVariable* findActiveLocalVariableById(List* variablesTable, char* varName);
struct LocalVariable* addVariableToLocalVarsTable(char* id, struct SemanticType* type, struct Method* method, bool isMutable);
bool addVarSpecToLocalVarsTable(struct nvar* varSpec, struct Method* method);
bool addConstSpecToLocalVarsTable(struct nval* constSpec, struct Method* method);
bool addConstantToLocalConstantTable(char* constName, HashTable* localConstTable, struct Method* method);
bool addParamToLocalVarsTable(char* paramName, struct SemanticType* type, struct Method* method);
void deactivateLocalVariablesByScope(List* localVariablesTable, int scope);
bool isContainStatementType(struct statement_list* stmtList, enum statement_type stmtType);

struct Constant* addUtf8ToConstantsTable(char* utf8);
struct Constant* addStringToConstantsTable(char* string);
struct Constant* addIntegerToConstantsTable(int value);
struct Constant* addFloatToConstantsTable(float value);
struct Constant* addNameAndTypeToConstantsTable(char* name, char* type);
struct Constant* addFieldRefToConstantsTable(char* fieldName, char* typeName);
struct Constant* addMethodRefToConstantsTable(char* methodName, char* methodDescriptor);
struct Constant* addClassToConstantsTable(char* className);
struct Constant* getConstantUtf8(char* utf8);

char* createMethodDescriptor(struct nargs* paramList, char* returnTypeStr);
char* convertTypeToString(struct SemanticType* type);

struct Field* getField(struct Class* _class, char* fieldName);
struct Method* getMethod(char* methodName);
struct SemanticType* getFunctionReturnType(struct nfunc* functionDecl, struct Method* method);

void printLocalVariablesTable(struct Method* method);
void printConstantsTable();
void printMethodsTable();
void printConstant(struct Constant* constant);

struct  Class* semanticClass;

//node of list : struct Constant*
List* constantsTable;

//key: char*, value: Field*;
HashTable* fieldsTable;

//key: char*, value: struct Method* ;
HashTable* methodsTable;

struct Constant* constantClass;

#endif
