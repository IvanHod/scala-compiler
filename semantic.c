#include "semantic.h"


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
    struct Constant* constClassName = addUtf8ToConstantsTable(CLASS_NAME);
    //add class to constants table
    constantClass = addClassToConstantsTable(CLASS_NAME);

    //
    bool isOk = true;
    struct DeclarationList* declList = program->declList;

    if (declList != NULL) {
        struct Declaration* decl = declList->firstDecl;
        while (decl != NULL && isOk) {
            if (decl->declType == CONST_DECL) {
                isOk = checkSemanticConstDecl(decl->constDecl, NULL);
            }
            else if (decl->declType == VAR_DECL) {
                isOk = checkSemanticVarDecl(decl->varDecl, NULL);
            }
            else if (decl->declType == FUNC_DECL) {
                isOk = checkSemanticFunctionDecl(decl->funcDecl);
            }
            decl = decl->nextDecl;
        }
    }

    //TODO: yield an error if function main not found
    //check semantic main function (function without parameters)
    return isOk;
}


void check_stmt_list(struct statement_list *_stmt_list) {
    struct statement *stmt = _stmt_list->first;
    while ( stmt != NULL ) {
        check_stmt(stmt);
        stmt = stmt->next;
    }
}

void check_expr_list(struct expression_list *_expr_list) {
    struct expression *expr = _expr_list->first;
    while ( expr != NULL ) {
        expr = expr->next;
    }
}

void check_stmt(struct statement *stmt) {
    if( stmt == NULL ) return;
    switch (stmt->type) {
    case EXPR_LIST  :
        check_expr_list(stmt->expr_list);
        break;
    case STMT_LIST  :
        check_stmt_list(stmt->stmt_list);
        break;
    case NIF    :
        check_if(stmt->_if);
        break;
    case LOOP   :
        check_loop(stmt->_loop);
        break;
    case NVAR   :
        check_var(stmt->to_print_var);
        break;
    case NVAL   :
        check_val(stmt->to_print_val);
        break;
    case NFUNC  :
        check_func(stmt->to_print_func);
        break;
    case NMATCH :

        break;
    }
}

void check_expr(struct expression *expr) {
    if( expr == NULL ) return;
    char str[50];
    switch(expr->type) {
    case id:
        break;
    case Int:
        break;
    case Bool:
        break;
    case Float:
        break;
    case Char:
        break;
    case String:
        break;
    case Null:
        break;
    case Type:
        printf_expr(expr->left);
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
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case usub:
        printf_expr(expr->left);
        break;
    case sub:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case sub_eq:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case mul:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case mul_eq:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case DIV:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case div_eq:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
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
        printf_expr(expr->left);
        printf_expr(expr->rigth);
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
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case call_func:
        printf_expr_list(expr->expr_list);
        break;
    case prefix_inc:
        printf_expr(expr->left);
        break;
    case prefix_dec:
        printf_expr(expr->left);
        break;
    case postfix_inc:
        printf_expr(expr->left);
        break;
    case postfix_dec:
        printf_expr(expr->left);
        break;
    case point:
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case println:
        printf_expr_list(expr->expr_list);
        break;
    case println_s:
        printfHardString(expr->hString);
        break;
    }
}

void check_if( struct nif *_nif ) {
    check_expr(_nif->expr);
    check_stmt(_nif->to_then);
    check_stmt(_nif->to_else);
}

void check_loop(struct loop* _loop) {
    switch (_loop->type) {
    case CICLE_FOR:
        check_expr(_loop->expr_1);
        check_expr(_loop->expr_2);
        check_stmt(_loop->stmt);
        break;
    case CICLE_WHILE:
        check_expr(_loop->expr_while);
        check_stmt(_loop->stmt);
        break;
    }
}

void check_var(struct nvar *var) {
    check_id_list(var->id_list);
    check_expr(var->result);
    check_expr(var->return_value);
}

void check_val(struct nval* val) {
    check_id_list(val->ids_list);
}

void check_func(struct nfunc *func) {
    check_args(func->_args);ё
    check_expr(func->return_var);
    check_stmt(func->body);
}

void check_id_list(struct id_list *_id_list) {
    struct id_list *idList =_id_list;
    while ( idList != NULL ) {
        idList = idList->next;
    }
}

void check_args(struct nargs *_args) {
    struct narg *arg = _args->first;
    while ( arg != NULL ) {
        check_expr(arg->expr);
        arg = arg->next;
    }
}
