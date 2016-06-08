#include "func.h"
#include "semantic.h"

char* CLASS_NAME = "GO_CLASS";
int scope = 0;

bool doSemantic(struct Root* root) {
    //create class to wrap programm
    semanticClass = (struct Class*)malloc(sizeof(struct Class));
    semanticClass->className = CLASS_NAME;
    //Initialize constantsTable
    list_new(&constantsTable);
    semanticClass->constantsTable = constantsTable;

    //Initialize fieldsTable
    hashtable_new(&fieldsTable);
    semanticClass->fieldsTable = fieldsTable;

    //Initialize methodsTable
    hashtable_new(&methodsTable);
    semanticClass->methodsTable = methodsTable;

    //add CLASS's name to constantsTAble
    struct Constant* constClassName = addUtf8ToConstantsTable(root->id);
    //add class to constants table
    constantClass = addClassToConstantsTable(root->id);

    bool isOk = true;
    struct statement* stmt = root->stmt_list->first;

    while (stmt != NULL && isOk) {
        isOk = check_stmt_list(stmt);
        stmt = stmt->next;
    }
    return isOk;
}


bool check_stmt_list(struct statement_list *_stmt_list) {
    struct statement *stmt = _stmt_list->first;
    bool isOk = true;
    while ( stmt != NULL && isOk ) {
        isOk = check_stmt(stmt);
        stmt = stmt->next;
    }
    return isOk;
}

bool check_expr_list(struct expression_list *_expr_list) {
    struct expression *expr = _expr_list->first;
    bool isOk = true;
    while ( expr != NULL && isOk ) {
        isOk = check_expr(expr);
        expr = expr->next;
    }
    return isOk;
}

bool check_stmt(struct statement *stmt) {
    if( stmt == NULL ) return;
    bool isOk = true;
    switch (stmt->type) {
    case EXPR_LIST  :
        isOk = check_expr_list(stmt->expr_list);
        break;
    case STMT_LIST  :
        isOk = check_stmt_list(stmt->stmt_list);
        break;
    case NIF    :
        isOk = check_if(stmt->_if);
        break;
    case LOOP   :
        isOk = check_loop(stmt->_loop);
        break;
    case NVAR   :
        isOk = check_var(stmt->to_print_var);
        break;
    case NVAL   :
        isOk = check_val(stmt->to_print_val);
        break;
    case NFUNC  :
        isOk = check_func(stmt->to_print_func);
        break;
    case NMATCH :

        break;
    }
    return isOk;
}

bool check_expr(struct expression *expr) {
    if( expr == NULL ) return true;
    bool isOk = true;
    struct SemanticType* type = (struct SemanticType*)malloc(sizeof(struct SemanticType));
    switch(expr->type) {
    case id: {

        break;
    }
    case Int:
        type->typeName = INT_TYPE_NAME;
        break;
    case Bool:
        type->typeName = BOOL_TYPE_NAME;
        break;
    case Float:
        type->typeName = FLOAT32_TYPE_NAME;
        break;
    case Char: {
        type->typeName = CHAR_TYPE_NAME;
        break;
    }
    case String:
        type->typeName = STRING_TYPE_NAME;
        break;
    case assigment:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case equal:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case no_equal:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case no:
        printf_expr(expr->left);
        break;
    case more:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case more_eq:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case less:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case less_eq:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case plus:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case plus_eq:
        expr->rigth = CreateExprOperation(expr->left, plus, expr->rigth);
        expr->type = assigment;
        isOk = check_expr(expr->rigth);
        if(isOk) {

        } else {

        }
        break;
    case usub:
        if(expr->left->type == Int || expr->left->type == Float) {
            expr->left->Int = expr->left->Int * -1;
            expr = expr->left;
        } else if( expr->left->type == id ) {
            expr->rigth = expr->left;
            expr->left = CreateExprINT(0);
            expr->type = sub;
        } else {
            printf("Unar minus can to use with int or float number");
            isOk = false;
        }
        break;
    case sub:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case sub_eq:
        expr->rigth = CreateExprOperation(expr->left, sub, expr->rigth);
        expr->type = assigment;
        isOk = check_expr(expr->rigth);
        if(isOk) {

        } else {

        }
        break;
    case mul:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case mul_eq:
        expr->rigth = CreateExprOperation(expr->left, mul, expr->rigth);
        expr->type = assigment;
        isOk = check_expr(expr->rigth);
        if(isOk) {

        } else {

        }
        break;
    case DIV:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case div_eq:
        expr->rigth = CreateExprOperation(expr->left, DIV, expr->rigth);
        expr->type = assigment;
        isOk = check_expr(expr->rigth);
        if(isOk) {

        } else {

        }
        break;
    case div_residue:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case unarOR:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case _or:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case or_equal:
        expr->rigth = CreateExprOperation(expr->left, _or, expr->rigth);
        expr->type = assigment;
        isOk = check_expr(expr->rigth);
        if(isOk) {

        } else {

        }
        break;
    case unarXOR:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case _xor:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case unarAND:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case _and:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case and_equal:
        expr->rigth = CreateExprOperation(expr->left, _and, expr->rigth);
        expr->type = assigment;
        isOk = check_expr(expr->rigth);
        if(isOk) {

        } else {

        }
        break;
    case call_func:
        printf_expr_list(expr->expr_list);
        break;
    case prefix_inc:
        expr->type = plus;
        expr->rigth = CreateExprINT(1);
        break;
    case prefix_dec:
        expr->type = sub;
        expr->rigth = CreateExprINT(1);
        break;
    case postfix_inc:
        expr->type = plus;
        expr->rigth = expr->left;
        expr->left = CreateExprINT(1);
        break;
    case postfix_dec:
        expr->type = sub;
        expr->rigth = expr->left;
        expr->left = CreateExprINT(1);
        break;
    case point:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case println:
        printf_expr_list(expr->expr_list);
        break;
    }
    return isOk;
}

bool check_if( struct nif *_nif ) {
    bool isOk = true;
    isOk = check_expr(_nif->expr);
    isOk = isOk && check_stmt(_nif->to_then);
    isOk = isOk && check_stmt(_nif->to_else);
    return isOk;
}

bool check_loop(struct loop* _loop) {
    bool isOk = true;
    switch (_loop->type) {
    case CICLE_FOR:
        isOk = check_expr(_loop->expr_1);
        isOk = isOk && check_expr(_loop->expr_2);
        isOk = isOk && check_stmt(_loop->stmt);
        break;
    case CICLE_WHILE:
        isOk = check_expr(_loop->expr_while);
        isOk = isOk && check_stmt(_loop->stmt);
        break;
    }
    return isOk;
}

bool check_var(struct nvar *var) {
    bool isOk = true;
    isOk = check_id_list(var->id_list);
    isOk = isOk && check_expr(var->result);
    isOk = isOk && check_expr(var->return_value);
    return isOk;
}

bool check_val(struct nval* val) {
    bool isOk = true;
    isOk = check_id_list(val->ids_list);
    return isOk;
}

bool check_func(struct nfunc *func) {
    bool isOk = true;
    isOk = check_args(func->_args);
    isOk = isOk && check_expr(func->return_var);
    isOk = isOk && check_stmt(func->body);
    return isOk;
}

bool check_id_list(struct id_list *_id_list) {
    struct id_list *idList =_id_list;
    bool isOk = true;
    while ( idList != NULL && isOk ) {
        isOk = true;
        idList = idList->next;
    }
    return isOk;
}

bool check_args(struct nargs *_args) {
    struct narg *arg = _args->first;
    bool isOk = true;
    while ( arg != NULL && isOk ) {
        isOk = check_expr(arg->expr);
        arg = arg->next;
    }
    return isOk;
}


struct Constant* addUtf8ToConstantsTable(char* utf8) {
    struct Constant* constant = NULL;
    bool found = false;
    int size = list_size(constantsTable),
            i = 0;
    for (i = 0; !found && i < size; ++i) {
        list_get_at(constantsTable, i, &constant);
        if (constant->type == CONSTANT_Utf8 && !strcmp(constant->utf8, utf8)) {
            found = true;
        }
        i++;
    }
    if (!found) {
        constant = (struct Constant*) malloc(sizeof(struct Constant));
        constant->type = CONSTANT_Utf8;
        constant->utf8 = utf8;
        constant->id = size + 1;
        list_add(constantsTable, constant);
    }
    return constant;
}

struct Constant* addStringToConstantsTable(char* string) {
    struct Constant* constant = NULL;
    bool found = false;
    struct Constant* constUtf8 = addUtf8ToConstantsTable(string);
    int size = list_size(constantsTable),
            i = 0;
    for (i = 0; !found && i < size; ++i) {
        list_get_at(constantsTable, i, &constant);
        if (constant->type == CONSTANT_String && constant->const1 == constUtf8) {
            found = true;
        }
        i++;
    }
    if (!found) {
        constant = (struct Constant*) malloc(sizeof(struct Constant));
        constant->type = CONSTANT_String;
        constant->const1 = constUtf8;
        constant->id = size + 1;
        list_add(constantsTable, constant);
    }
    return constant;
}

struct Constant* addIntegerToConstantsTable(int value) {
    struct Constant* constant = NULL;
    bool found = false;
    int size = list_size(constantsTable),
            i = 0;
    for (i = 0; !found && i < size; ++i) {
        list_get_at(constantsTable, i, &constant);
        if (constant->type == CONSTANT_Integer && constant->intValue == value) {
            found = true;
        }
        i++;
    }
    if (!found) {
        constant = (struct Constant*) malloc(sizeof(struct Constant));
        constant->type = CONSTANT_Integer;
        constant->intValue = value;
        constant->id = size + 1;
        list_add(constantsTable, constant);
    }
    return constant;
}

struct Constant* addFloatToConstantsTable(float value) {
    struct Constant* constant = NULL;
    bool found = false;
    int size = list_size(constantsTable),
            i = 0;
    for (i = 0; !found && i < size; ++i) {
        list_get_at(constantsTable, i, &constant);
        if (constant->type == CONSTANT_Float && constant->floatValue == value) {
            found = true;
        }
    }
    if (!found) {
        constant = (struct Constant*) malloc(sizeof(struct Constant));
        constant->type = CONSTANT_Float;
        constant->floatValue = value;
        constant->id = size + 1;
        list_add(constantsTable, constant);
    }
    return constant;
}


struct Constant* addClassToConstantsTable(char* className) {
    struct Constant* constant = NULL;
    struct Constant* classNameConst = addUtf8ToConstantsTable(className);
    bool found = false;
    int size = list_size(constantsTable),
            i = 0;
    for (i = 0; !found && i < size; ++i) {
        list_get_at(constantsTable, i, &constant);
        if (constant->type == CONSTANT_Class && constant->const1== classNameConst) {
            found = true;
        }
    }
    if (!found) {
        constant = (struct Constant*) malloc(sizeof(struct Constant));
        constant->type = CONSTANT_Class;
        constant->const1 = classNameConst;
        constant->id = size + 1;
        list_add(constantsTable, constant);
    }
    return constant;
}
struct Constant* addNameAndTypeToConstantsTable(char* name, char* type) {

    struct Constant* constant = NULL;
    struct Constant* nameConst = addUtf8ToConstantsTable(name);
    struct Constant* typeConst = addUtf8ToConstantsTable(type);
    int size = list_size(constantsTable),
            i = 0;
    bool found = false;
    for (i = 0; !found && i < size; ++i) {
        list_get_at(constantsTable, i, &constant);
        if (constant->type == CONSTANT_NameAndType && constant->const1 == nameConst && constant->const2 == typeConst) {
            found = true;
        }
    }

    if (!found) {
        constant = (struct Constant*) malloc(sizeof(struct Constant));
        constant->type = CONSTANT_NameAndType;
        constant->const1 = nameConst;
        constant->const2 = typeConst;
        constant->id = size + 1;
        list_add(constantsTable, constant);
    }
    return constant;
}

struct Constant* getConstantUtf8(char* utf8) {
    struct Constant* constant = NULL;
    int size = list_size(constantsTable),
            i = 0;
    bool found = false;
    for (i = 0; !found && i < size; ++i) {
        list_get_at(constantsTable, i, &constant);
        if (constant->type == CONSTANT_Utf8 && !strcmp(utf8, constant->utf8)) {
            found = true;
        }
    }
    return constant;
}
