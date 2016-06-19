#include "semantic.h"

char* CLASS_NAME = "GO_CLASS";
int scope = 0,
    i = 0;

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
    addUtf8ToConstantsTable(root->id);
    //add class to constants table
    constantClass = addClassToConstantsTable(root->id);

    bool isOk = true;
    struct statement* stmt = root->stmt_list->first;

    while (stmt != NULL && isOk) {
        isOk = check_stmt(stmt, NULL);
        stmt = stmt->next;
    }
    if( isOk ) {
        printConstantsTable();
    }
    return isOk;
}


bool check_stmt_list(struct statement_list *_stmt_list, struct Method* method) {
    if( _stmt_list == NULL ) return true;
    struct statement *stmt = _stmt_list->first;
    bool isOk = true;
    while ( stmt != NULL && isOk ) {
        isOk = check_stmt(stmt, method);
        stmt = stmt->next;
    }
    return isOk;
}

bool check_expr_list(struct expression_list *_expr_list, struct Method* method) {
    struct expression *expr = _expr_list->first;
    bool isOk = true;
    while ( expr != NULL && isOk ) {
        isOk = check_expr(expr, method);
        expr = expr->next;
    }
    return isOk;
}

bool check_stmt(struct statement *stmt, struct Method* method) {
    if( stmt == NULL ) return false;
    bool isOk = true;
    switch (stmt->type) {
    case EXPR_LIST  :
        isOk = check_expr_list(stmt->expr_list, method);
        break;
    case STMT_LIST  :
        isOk = check_stmt_list(stmt->stmt_list, method);
        break;
    case NIF    :
        isOk = check_if(stmt->_if, method);
        break;
    case LOOP   :
        isOk = check_loop(stmt->_loop, method);
        break;
    case NVAR   :
        isOk = check_var(stmt->to_print_var, method);
        break;
    case NVAL   :
        isOk = check_val(stmt->to_print_val, method);
        break;
    case NFUNC  :
        isOk = check_func(stmt->to_print_func);
        break;
    case NMATCH :

        break;
    default: break;
    }
    return isOk;
}

bool check_expr(struct expression *expr, struct Method* method) {
    if( expr == NULL ) return true;
    bool isOk = true;
    struct SemanticType* type = (struct SemanticType*)malloc(sizeof(struct SemanticType));
    switch(expr->type) {
    case id: {
        if( !strcmp(expr->String, "Int") ) {
            type->typeName = INT_TYPE_NAME;
            expr->semanticType = type;
        } else if( !strcmp(expr->String, "Float") ) {
            type->typeName = FLOAT32_TYPE_NAME;
            expr->semanticType = type;
        } else if( !strcmp(expr->String, "Boolean") ) {
            type->typeName = BOOL_TYPE_NAME;
            expr->semanticType = type;
        } else if( !strcmp(expr->String, "Char" )) {
            type->typeName = CHAR_TYPE_NAME;
            expr->semanticType = type;
        } else if( !strcmp(expr->String, "String" )) {
            type->typeName = STRING_TYPE_NAME;
            expr->semanticType = type;
        } else {
            addUtf8ToConstantsTable(expr->String);
            // поиск локальной переменной.
            struct LocalVariable* variable = findActiveLocalVariableById(method->localVariablesTable, expr->String);
            if (variable == NULL) { // если не найденна локальная переменная
                struct Field* field = getField(semanticClass, expr->String);
                if (field == NULL) {
                    printf("Semantic error. Identifier %s not declared\n", expr->String);
                    type->typeName = UNKNOWN_TYPE;
                    isOk = false;
                }
                else {
                    type->typeName = field->type->typeName;
                    type->idNum = field->id;
                }
            } else {
                type->typeName = variable->semanticType->typeName;
                type->idNum = variable->id;
            }
        }
        break;
    }
    case Int:
        type->typeName = INT_TYPE_NAME;
        expr->semanticType = type;
        addIntegerToConstantsTable(expr->Int);
        break;
    case Bool:
        type->typeName = BOOL_TYPE_NAME;
        expr->semanticType = type;
        addIntegerToConstantsTable(expr->boolean);
        break;
    case Float:
        type->typeName = FLOAT32_TYPE_NAME;
        expr->semanticType = type;
        addFloatToConstantsTable(expr->Float);
        break;
    case Char:
        type->typeName = CHAR_TYPE_NAME;
        expr->semanticType = type;
        break;
    case String:
        type->typeName = STRING_TYPE_NAME;
        expr->semanticType = type;
        addStringToConstantsTable(expr->String);
        break;
    case assigment:
        isOk = check_expr(expr->left, method);
        if(isOk) {
            struct LocalVariable* variable = findActiveLocalVariableById(method->localVariablesTable, expr->left->String);
            if( !variable->isMutable ) {
                isOk = false;
                printf("Semantic error: var %s is const.", expr->left->String);
            }
            if(isOk) {
                isOk = check_expr(expr->rigth, method);
                if(isOk) {
                    type->typeName = expr->rigth->semanticType->typeName;
                    type->idNum = expr->rigth->semanticType->idNum;
                }
            }
        }
        break;
    case no:
        isOk = check_expr(expr->left, method);
        if( isOk ) {
            if( expr->left->type == Bool ) {
                type->typeName = BOOL_TYPE_NAME;
            } else if( expr->left->type == id && expr->left->semanticType->typeName == BOOL_TYPE_NAME ) {
                type->typeName = BOOL_TYPE_NAME;
            } else {
                isOk = false;
                if(expr->left->type == id)
                    printf("Semantic error: unary ! is not member var of %s",expr->left->String );
                else if(expr->left->type == Int)
                    printf("Semantic error: unary ! can not to use with %d: ",expr->left->Int );
                else if(expr->left->type == Float)
                    printf("Semantic error: unary ! can not to use with %f: ",expr->left->Float );
                else if(expr->left->type == Char)
                    printf("Semantic error: unary ! can not to use with %c: ",expr->left->Char );
                else if(expr->left->type == String)
                    printf("Semantic error: unary ! can not to use with %s: ",expr->left->String );
            }
        }
        break;
    case usub:
        isOk = check_expr(expr->left, method);
        if(isOk) {
            type->typeName = expr->left->semanticType->typeName;
            type->idNum = expr->left->semanticType->idNum;
        }
        if(expr->left->type == Int) {
            expr->Int = expr->left->Int * -1;
            expr->type = expr->left->type;
        } else if( expr->left->type == Float ) {
            expr->Int = expr->left->Float * -1;
            expr->type = expr->left->type;
        } else if( expr->left->type == id ) {
            isOk = isOk && check_expr(expr->left, method);
            if(isOk) {
                if(expr->left->semanticType->typeName == BOOL_TYPE_NAME ) {
                    printf("Unar minus is not a member var %s of Boolean", expr->left->String);
                    isOk = false;
                }
            }
            expr->rigth = expr->left;
            expr->left = CreateExprINT(0);
            expr->type = sub;
        } else {
            printf("Unar minus can to use with int or float number");
            isOk = false;
        }
        break;
    case sub_eq:
    case plus_eq:
    case mul_eq:
    case div_eq: {
        enum expr_type exprType;
        switch(expr->type) {
            case sub_eq: exprType = sub; break;
            case plus_eq: exprType = plus; break;
            case mul_eq: exprType = mul; break;
            case div_eq: exprType = DIV; break;
            default:break;
        }
        expr->rigth = CreateExprOperation(expr->left, exprType, expr->rigth);
        expr->type = assigment;
        isOk = check_expr(expr, method);
        break;
    }
    case plus:
    case sub:
    case div_residue:
    case mul:
    case DIV: {
        isOk = check_expr(expr->left, method);
        isOk = isOk && check_expr(expr->rigth, method);
        if(isOk) {
            enum TypeNames leftType = expr->left->semanticType->typeName;
            enum TypeNames rightType = expr->rigth->semanticType->typeName;
            if(leftType == STRING_TYPE_NAME || rightType == STRING_TYPE_NAME) {
                type->typeName = STRING_TYPE_NAME;
            } else if( leftType == FLOAT32_TYPE_NAME && ( rightType == FLOAT32_TYPE_NAME || rightType == INT_TYPE_NAME || rightType == CHAR_TYPE_NAME) ||
                       rightType == FLOAT32_TYPE_NAME && ( leftType == FLOAT32_TYPE_NAME || leftType == INT_TYPE_NAME || leftType == CHAR_TYPE_NAME) ) {
                type->typeName = FLOAT32_TYPE_NAME;
            } else if( leftType == INT_TYPE_NAME && (rightType == INT_TYPE_NAME || rightType == CHAR_TYPE_NAME) ||
                       rightType == INT_TYPE_NAME && (leftType == INT_TYPE_NAME || leftType == CHAR_TYPE_NAME) ) {
                type->typeName = INT_TYPE_NAME;
            } else if( leftType == CHAR_TYPE_NAME && rightType == CHAR_TYPE_NAME)
                type->typeName = INT_TYPE_NAME;
            else {
                printf("Error: variable types %s and %s not incompatible", expr->left->String, expr->rigth->String);
                isOk = false;
            }
        }
        break;
    }
    case and_equal:
    case or_equal:
        expr->rigth = CreateExprOperation(expr->left, expr->type == and_equal ? _and : _or, expr->rigth);
        expr->type = assigment;
        type = NULL;
        isOk = check_expr(expr, method);
        break;
    case equal:
    case no_equal:
    case more:
    case more_eq:
    case less:
    case less_eq:
    case unarOR:
    case _or:
    case unarXOR:
    case _xor:
    case unarAND:
    case _and:
        isOk = check_expr(expr->left, method);
        isOk = isOk && check_expr(expr->rigth, method);
        if(isOk) {
            type->typeName = BOOL_TYPE_NAME;
        }
        break;
    case call_func:
        break;
    case prefix_inc:
        expr->type = plus;
        expr->rigth = CreateExprINT(1);
        type = NULL;
        isOk = check_expr(expr, method);
        break;
    case prefix_dec:
        expr->type = sub;
        expr->rigth = CreateExprINT(1);
        type = NULL;
        isOk = check_expr(expr, method);
        break;
    case postfix_inc:
        expr->type = plus;
        expr->rigth = expr->left;
        expr->left = CreateExprINT(1);
        type = NULL;
        isOk = check_expr(expr, method);
        break;
    case postfix_dec:
        expr->type = sub;
        expr->rigth = expr->left;
        expr->left = CreateExprINT(1);
        type = NULL;
        isOk = check_expr(expr, method);
        break;
    case point:
        break;
    default: break;
    }
    if( type != NULL )
        expr->semanticType = type;
    return isOk;
}

bool check_if( struct nif *_nif, struct Method* method) {
    bool isOk = true;
    isOk = check_expr(_nif->expr, method);
    isOk = isOk && check_stmt(_nif->to_then, method);
    isOk = isOk && check_stmt(_nif->to_else, method);
    return isOk;
}

bool check_loop(struct loop* _loop, struct Method* method) {
    bool isOk = true;
    switch (_loop->type) {
    case CICLE_FOR:
        isOk = check_expr(_loop->expr_1, method);
        isOk = isOk && check_expr(_loop->expr_2, method);
        isOk = isOk && check_stmt(_loop->stmt, method);
        break;
    case CICLE_WHILE:
        isOk = check_expr(_loop->expr_while, method);
        isOk = isOk && check_stmt(_loop->stmt, method);
        break;
    }
    return isOk;
}

bool check_var(struct nvar *var, struct Method* method) {
    // var a : return_value = result;
    bool isOk = true;
    struct SemanticType* type = NULL;
    if( var->return_value != NULL) {
        isOk = check_expr(var->return_value, method);
        type = (struct SemanticType*)malloc(sizeof(struct SemanticType));
        type->typeName = var->return_value->semanticType->typeName;
        type->idNum = var->return_value->semanticType->idNum;
    }
    if( isOk ) {
        isOk = isOk && check_expr(var->result, method);
        if( type != NULL ) {
            if(type->typeName != var->result->semanticType->typeName || type->idNum != var->result->semanticType->idNum ) {
                printf("Semantic error: disparity types of var: %s", var->idList->first->id);
                isOk = false;
            } else
                var->result->semanticType = type;
        }
    }
    if( isOk ) {
        isOk = addVarSpecToLocalVarsTable(var, method);
    }
    return isOk;
}

bool check_val(struct nval* val, struct Method* method) {
    // val a : return_value = expr;
    bool isOk = true;
    struct SemanticType* type = (struct SemanticType*)malloc(sizeof(struct SemanticType));
    if( val->return_value != NULL ) {
        isOk = check_expr(val->return_value, method);
        type->typeName = val->return_value->semanticType->typeName;
        type->idNum = val->return_value->semanticType->idNum;
    }
    if( isOk ) {
        isOk = isOk && check_expr(val->expr, method);
        if( type != NULL ) {
            if( type->typeName == val->expr->semanticType->typeName && type->idNum == val->expr->semanticType->idNum ) {
                printf("Semantic error: disparity types of val: %s", val->ids_list->first->id);
                isOk = false;
            }
            else
                val->expr->semanticType = type;
        }
    }
    if( isOk ) {
        isOk = addConstSpecToLocalVarsTable(val, method);
    }
    return isOk;
}

bool check_func(struct nfunc *func) {
    bool isOk = true;
    struct Method* method = NULL;
    if (hashtable_get(semanticClass->methodsTable, func->name, &method) != CC_OK) {
        struct nargs* paramList = func->_args;
        if (paramList != NULL) {
            isOk = check_args(paramList, method, func->name);
            if (!isOk) {
                printf("Semantic error. Unsupport types in parameter declaration in function %s\n", func->name);
                return false;
            }
        }

        struct SemanticType* semanticReturnType = getFunctionReturnType(func, method);
        if (semanticReturnType->typeName == UNKNOWN_TYPE) {
            printf("Semantic error. Unsupport return type in function %s\n", func->name);
            return false;
        }

        if (func->body == NULL) {
            printf("Body of function %s not found", func->name);
            return false;
        }
        //add return type to constant table
        char* returnTypeStr = convertTypeToString(semanticReturnType);
        ///addUtf8ToConstantsTable(returnTypeStr);
        //add name and type to constants table
        char* methodDescriptor = createMethodDescriptor(func->_args, returnTypeStr);
        //add method ref to constants table
        struct Constant* constMethodRef = addMethodRefToConstantsTable(func->name, methodDescriptor);
        //create method
        method = (struct Method*)malloc(sizeof(struct Method));
        method->constMethodref = constMethodRef;
        method->returnType = semanticReturnType;
        method->paramList = paramList;

        hashtable_add(methodsTable, func->name, method);

        Array* out;
        array_new(&out);
        hashtable_get_keys(methodsTable, &out);

        hashtable_size(methodsTable);

        //add variable to local variables table
        list_new(&method->localVariablesTable);

        if(paramList != NULL) {
            struct narg *param = paramList->first;
            while( param != NULL ) {
                addUtf8ToConstantsTable(param->id);
                struct SemanticType* paramSemanticType = (struct SemanticType*) malloc(sizeof(struct SemanticType));
                paramSemanticType->typeName = param->expr->semanticType->typeName;
                char* paramTypeStr = convertTypeToString(paramSemanticType);
                addUtf8ToConstantsTable(paramTypeStr);
                isOk = addParamToLocalVarsTable(param->id, paramSemanticType, method);
                if (isOk) {
                    param = param->next;
                }
                else {
                    printf("Variable with the same name in function %s has been defined\n", func->name);
                }
            }
        }
        isOk = isOk && check_stmt(func->body, method);
    } else {

    }
    return isOk;
}

bool check_args(struct nargs *_args, struct Method* method, char* functionName) {
    struct narg *arg = _args->first;
    bool isOk = true;
    while ( arg != NULL && isOk ) {
        isOk = check_expr(arg->expr, method);
        arg = arg->next;
    }
    return isOk;
}

//return type should be one of the following:	INT, FLOAT, STRING, VOID,// INT[], FLOAT[], STRING[]
//the format is (), (INT|FLOAT|STRING), INT, FLOAT, STRING
struct  SemanticType*  getFunctionReturnType(struct nfunc* functionDecl, struct Method* method) {
    struct SemanticType* semanticType = (struct SemanticType*) malloc(sizeof(struct SemanticType));
    semanticType->typeName = UNKNOWN_TYPE;
    if (functionDecl->return_var != NULL) {
        check_expr(functionDecl->return_var, method);
        semanticType->typeName = functionDecl->return_var->semanticType->typeName;
    }
    else {
        semanticType->typeName = VOID_TYPE_NAME;
    }
    return semanticType;
}


/* ----------------------------------------------- functions to work with local vars --------------------------- */

struct LocalVariable* findActiveLocalVariableByScope(List* variablesTable, char* varName, int scope) {
    struct LocalVariable* result = (struct LocalVariable*) malloc(sizeof(struct LocalVariable));
    int size = list_size(variablesTable);
    bool found = false;
    for (i = 0; !found && i < size; ++i) {
        list_get_at(variablesTable, i, &result);
        if (result->scope == scope && strcmp(result->name, varName) == 0 && result->isActive) {
            found = true;
        }
    }
    if (found) {
        return result;
    }
    return NULL;
}

//for tree printing purpose
struct LocalVariable* findLocalVariableByScope(List* variablesTable, char* varName, int scope) {
    struct LocalVariable* result = (struct LocalVariable*) malloc(sizeof(struct LocalVariable));
    int size = list_size(variablesTable);
    bool found = false;
    for (i = 0; !found && i < size; ++i) {
        list_get_at(variablesTable, i, &result);
        if (result->scope == scope && strcmp(result->name, varName) == 0) {
            found = true;
        }
    }

    if (found) {
        return result;
    }
    return NULL;
}

void cloneVariable(struct LocalVariable* dest, struct LocalVariable* source) {
    dest->id = source->id;
    dest->isActive = source->isActive;
    dest->isMutable = source->isMutable;
    strcpy(dest->name, source->name);
    dest->scope = source->scope;
    dest->semanticType = (struct SemanticType*) malloc(sizeof(struct SemanticType));
    dest->semanticType->typeName = source->semanticType->typeName;
    dest->semanticType->idNum = source->semanticType->idNum;
}

struct LocalVariable* findActiveLocalVariableById(List* variablesTable, char* varName) {
    struct LocalVariable* result = NULL;
    struct LocalVariable* variable = NULL;
    int size = list_size(variablesTable);
    bool found = false;
    int minScope = 0;
    for (i = 0; i < size; ++i) {
        list_get_at(variablesTable, i, &variable);
        //this check is strong enough to detect the needed variable
        if (strcmp(variable->name, varName) == 0 && variable->isActive ) {
            result = variable;
        }
    }

    return result;
}

bool addVarSpecToLocalVarsTable(struct nvar* varSpec, struct Method* method) {
    bool isOk = true;
    struct id_in_list* id = varSpec->idList->first;
    struct expression* type = varSpec->return_value;
    if( type == NULL ) type = varSpec->result;
    struct SemanticType* semanticType = (struct SemanticType*) malloc(sizeof(struct SemanticType));
    semanticType->idNum = type->semanticType->idNum;
    semanticType->typeName = type->semanticType->typeName;

    // не понимаю???
    char* typenameStr = convertTypeToString(semanticType);
    addUtf8ToConstantsTable(typenameStr);

    while (id != NULL && isOk) {
        struct LocalVariable* variable = addVariableToLocalVarsTable(id->id, semanticType, method, true);
        addUtf8ToConstantsTable(id->id);
        isOk = variable != NULL;
        if (isOk) {
            //id->idNum = variable->id; не знаю зачем
            id = id->next;
        }
    }

    return isOk;
}

bool addConstSpecToLocalVarsTable(struct nval* constSpec, struct Method* method) {
    bool isOk = true;
    struct id_in_list* id = constSpec->ids_list->first;

    struct SemanticType* type = constSpec->expr;
    struct SemanticType* semanticType = (struct SemanticType*) malloc(sizeof(struct SemanticType));
    semanticType->typeName = type->typeName;
    char* typeNameStr = convertTypeToString(semanticType);
    addUtf8ToConstantsTable(typeNameStr);
    while (id != NULL && isOk) {
        // add utf8 to constant table
        addUtf8ToConstantsTable(id->id);
        isOk = (addVariableToLocalVarsTable(id->id, semanticType, method, false) != NULL );
        id = id->next;
    }
    return isOk;
}

bool addParamToLocalVarsTable(char* paramName, struct SemanticType* semanticType, struct Method* method) {
    bool isOk = true;
    List* variablesTable = method->localVariablesTable;
    struct LocalVariable* localVariable = findActiveLocalVariableByScope(variablesTable, paramName, 1);
    if (localVariable == NULL) {
        localVariable = (struct LocalVariable*)malloc(sizeof(struct LocalVariable));
        localVariable->name = paramName;
        localVariable->semanticType = semanticType;
        localVariable->id = list_size(variablesTable) + 1;
        localVariable->scope = 1;
        list_add(variablesTable, localVariable);
    }
    else {
        isOk = false;
    }
    return isOk;
}

//TODO: transform the multiple assignment to single assignment
struct LocalVariable* addVariableToLocalVarsTable(char* id, struct SemanticType* semanticType, struct Method* method, bool isMutable) {
    struct LocalVariable* localVariable = NULL;
    if( method == NULL) {

    } else {
        if (findActiveLocalVariableByScope(method->localVariablesTable, id, scope) == NULL) {
            localVariable = (struct LocalVariable*) malloc(sizeof(struct LocalVariable));
            localVariable->id = list_size(method->localVariablesTable) + 1;
            localVariable->scope = scope;
            localVariable->name = id;
            localVariable->isActive = true;
            localVariable->isMutable = isMutable;
            localVariable->semanticType = semanticType;
            list_add(method->localVariablesTable, localVariable);
        }
        else {
            printf("Variable with the same name %s exists in the table\n", id);
        }
    }
    return localVariable;
}

struct Method* getMethod( char* methodName) {
    struct Method* method = NULL; // (struct Method*) malloc(sizeof(struct Method));
    if (hashtable_get(methodsTable, methodName, &method) == CC_OK) {
        return method;
    }
    return NULL;
}

struct Field* getField(struct Class* class, char* fieldName) {
    struct Field* field = NULL; // (struct Field*) malloc(sizeof(struct Field));
    if (hashtable_get(class->fieldsTable, fieldName, &field) == CC_OK) {
        return field;
    }
    return NULL;
}

char* convertTypeToString(struct SemanticType* type) {
    //non array variable
    switch (type->typeName) {
    case STRING_TYPE_NAME:
        return "Ljava/lang/String;";
    case INT_TYPE_NAME:
        return "I";
    case FLOAT32_TYPE_NAME:
        return "F";
    case VOID_TYPE_NAME:
        return "V";
    default:
        return "UNKNOWN";
    }
}

//Here, propose that the semantic of  param list has been check
char* createMethodDescriptor(struct nargs* paramList, char* returnTypeStr) {
    char* result = (char*)malloc(100 * sizeof(char));
    if (paramList == NULL) {
        strcpy(result, "()");
        strcpy(result + 2, returnTypeStr);
    }
    else {
        struct narg* param = paramList->first;
        char* ptr = result;
        strcpy(ptr, "(");
        ptr += 1;
        while (param != NULL) {
            struct SemanticType* semanticType = (struct SemanticType*)malloc(sizeof(struct SemanticType));
            semanticType->typeName = param->expr->semanticType->typeName;
            char* typeNameStr = convertTypeToString(semanticType);
            strcpy(ptr, typeNameStr);
            ptr += strlen(typeNameStr);
            param = param->next;
        }
        strcpy(ptr, ")");
        ptr += 1;
        strcpy(ptr, returnTypeStr);
    }
    return result;
}


/* ----------------------------------------------- functions to work with constant vars ------------------------ */

struct Constant* addUtf8ToConstantsTable(char* utf8) {
    struct Constant* constant = NULL;
    bool found = false;
    int size = list_size(constantsTable);
    for (i = 0; !found && i < size; ++i) {
        list_get_at(constantsTable, i, &constant);
        if (constant->type == CONSTANT_Utf8 && !strcmp(constant->utf8, utf8)) {
            found = true;
        }
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
    int size = list_size(constantsTable);
    for (i = 0; !found && i < size; ++i) {
        list_get_at(constantsTable, i, &constant);
        if (constant->type == CONSTANT_String && constant->const1 == constUtf8) {
            found = true;
        }
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
    int size = list_size(constantsTable);
    for (i = 0; !found && i < size; ++i) {
        list_get_at(constantsTable, i, &constant);
        if (constant->type == CONSTANT_Integer && constant->intValue == value) {
            found = true;
        }
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
    int size = list_size(constantsTable);
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
    int size = list_size(constantsTable);
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
    int size = list_size(constantsTable);
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

struct Constant* addFieldRefToConstantsTable(char* fieldName, char* typeName) {
    //field ref: 1. class 2. name and type
    struct Constant* const1 = constantClass;
    struct Constant* const2 = addNameAndTypeToConstantsTable(fieldName, typeName);
    bool found = false;
    int size = list_size(constantsTable);
    struct Constant* constant = NULL;
    for (i = 0; !found && i < size; ++i) {
        list_get_at(constantsTable, i, &constant);
        if (constant->type  == CONSTANT_Fieldref && constant->const1 == const1 && constant->const2 == const2) {
            found = true;
        }
    }
    if (!found) {
        constant = (struct Constant*) malloc(sizeof(struct Constant));
        constant->type = CONSTANT_Fieldref;
        constant->const1 = const1;
        constant->const2 = const2;
        constant->id = size + 1;
        list_add(constantsTable, constant);
    }
    return constant;
}

struct Constant* addMethodRefToConstantsTable(char* methodName, char* methodDescriptor) {
    struct Constant* constant = NULL;
    struct Constant* const1 = constantClass;
    struct Constant* const2 = addNameAndTypeToConstantsTable(methodName, methodDescriptor);
    int size = list_size(constantsTable);
    bool found = false;
    for (i = 0; !found && i < size; ++i) {
        list_get_at(constantsTable, i, &constant);
        if (constant->type == CONSTANT_Methodref && constant->const1 == const1 && constant->const2 == const2) {
            found = true;
        }
    }
    if (!found) {
        constant = (struct Constant*) malloc(sizeof(struct Constant));
        constant->type = CONSTANT_Methodref;
        constant->const1 = const1;
        constant->const2 = const2;
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

/* ------------------------------------------------ printf tables with data -------------------------------------*/

void printLocalVariablesTable(struct Method* method) {
    if( method == NULL ) return;
    printf("\t\t\t\tMethod %s \n", method->constMethodref->const2->const1->utf8);
    List* localVariablesTable = method->localVariablesTable;
    int size = list_size(localVariablesTable);
    struct LocalVariable* localVariable = NULL;
    printf("ID\tNAME\t\tTYPE\n");
    for (i = 0; i < size; ++i) {
        list_get_at(localVariablesTable, i, &localVariable);
        printf("%d\t%s\t\t\t%s\n", localVariable->id, localVariable->name, convertTypeToString(localVariable->semanticType));
    }
    printf("\n");
}


void printConstant(struct Constant* constant) {
    switch (constant->type) {
    case CONSTANT_Integer:
        printf("%d\tINT\t\t\t%d\n",constant->id, constant->intValue );
        break;
    case CONSTANT_Float:
        printf("%d\tFLOAT\t\t%f\n", constant->id, constant->floatValue);
        break;
    case CONSTANT_Utf8:
        printf("%d\tUTF-8\t\t%s\n", constant->id, constant->utf8);
        break;
    case CONSTANT_NameAndType:
        printf("%d\tName&Type\t%d,%d\n", constant->id, constant->const1->id, constant->const2->id);
        break;
    case CONSTANT_Class:
        printf("%d\tClass\t\t%d\n", constant->id, constant->const1->id);
        break;
    case CONSTANT_Fieldref:
        printf("%d\tFieldRef\t%d,%d\n", constant->id, constant->const1->id, constant->const2->id);
        break;
    case CONSTANT_Methodref:
        printf("%d\tMethodRef\t%d,%d\n", constant->id, constant->const1->id, constant->const2->id);
        break;
    case CONSTANT_String:
        printf("%d\tString\t\t%d\n", constant->id, constant->const1->id);
        break;
    }
}

void printConstantsTable() {
    FILE* file = freopen("/home/ivan/qt-creator-projects/scala-compiler/tests/constants.txt", "w", stdout);
    int size = list_size(constantsTable);

    printf("					CONSTANTS TABLE\n");
    printf("ID\tTYPE\t\tVALUE\n\n");
    for (i = 0; i < size; ++i) {
        struct Constant* constant = NULL;
        list_get_at(constantsTable, i, &constant);
        //printf("%d\n", constant->id);
        printConstant(constant);
    }

    printMethodsTable();

    fclose(stdout);
}

void printMethodsTable() {
    printf("\n\n/*****************************************************************/\n");
    printf("					METHODS TABLE\n\n");
    HashTableIter i;
    hashtable_iter_init(&i, methodsTable);
    while (hashtable_iter_has_next(&i)) {
        TableEntry *e;
        hashtable_iter_next(&i, &e);
        struct Method* method = (struct Method*)e->value;
        printLocalVariablesTable(method);
    }
}

