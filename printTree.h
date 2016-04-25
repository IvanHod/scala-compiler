#ifndef PRINTTREE
#define PRINTTREE

#include <stdio.h>
#include "structs.h"
FILE *writeFile;

void printf_root();
void printf_stmt_list(struct statement_list *);
void printf_stmt(struct statement*);
void printf_expr_list(struct expression_list *);
void printf_expr(struct expression*);
void printf_if(struct nif*);
void printf_if_loop(struct nif_loop*);
void printf_var(struct nvar*);
void printf_val(struct nval*);
void printf_func(struct nfunc*);
void printf_class(struct nclass*);
void printf_id_list(struct id_list *id_list);
void printf_args(struct nargs *args);

void writeStr(char* str_1) {
    fwrite(str_1, strlen(str_1), 1, writeFile);
}

char* strcat_2(char* str_1, char* str_2) {
    char *result = (char *)malloc(sizeof(str_1) + sizeof(str_2));
    strcpy(result, str_1);
    strcat(result, str_2);
    return result;
}

char* strcat_3(char* str_1, char* str_2, char* str_3) {
    char *result = (char *)malloc(sizeof(str_1) + sizeof(str_2) + sizeof(str_3));
    strcpy(result, str_1);
    strcat(result, str_2);
    strcat(result, str_3);
    return result;
}

void printf_root() {
    writeStr("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    writeStr("<root>");
    printf_stmt_list(root->stmt_list);
    writeStr("</root>");
}

void printf_stmt_list(struct statement_list *_stmt_list) {
    writeStr("<statement_list>");
    struct statement_list *stmt_list = _stmt_list;
    while ( stmt_list != NULL ) {
        printf_stmt(stmt_list->stmt);
        stmt_list = stmt_list->next;
    }
    writeStr("</statement_list>");
}

void printf_stmt(struct statement *stmt) {
    switch (stmt->type) {
    case EXPR_LIST  :
        writeStr("<statement type=\"expr_list\">");
        printf_expr_list(stmt->expr_list);
        break;
    case STMT_LIST  :
        writeStr("<statement type=\"stmt_list\">");
        printf_stmt_list(stmt->stmt_list);
        break;
    case NIF:
        writeStr("<statement type=\"if\">");
        printf_if(stmt->_if);
        break;
    case NIF_LOOP   :
        writeStr("<statement type=\"if_loop\">");
        printf_if_loop(stmt->loop);
        break;
    case NVAR       :
        writeStr("<statement type=\"var\">");
        printf_var(stmt->to_print_var);
        break;
    case NVAL       :
        writeStr("<statement type=\"val\">");
        printf_val(stmt->to_print_val);
        break;
    case NFUNC      :
        writeStr("<statement type=\"function\">");
        printf_func(stmt->to_print_func);
        break;
    case NCLASS     :
        writeStr("<statement type=\"class\">");
        printf_if_loop(stmt->to_print_class);
        break;
    }
    writeStr("</statement>");
}


void printf_expr_list(struct expression_list *_expr_list) {
    struct expression_list *expr_list = _expr_list;
    while ( expr_list != NULL ) {
        printf_expr(expr_list->expr);
        expr_list = expr_list->next;
    }
}

void printf_if( struct nif *_nif) {
    if( _nif->expr != NULL ) {
        printf_expr(_nif->expr);
    }
    if( _nif->to_then != NULL ) {
        printf_stmt(_nif->to_then);
    }
    if( _nif->to_then != NULL ) {
        printf_stmt(_nif->to_else);
    }
}

void printf_if_loop(struct nif_loop *_if_loop) {
    struct nif_loop *if_loop = _if_loop;
    while ( if_loop != NULL ) {
        printf_expr(if_loop->expr);
        if_loop = if_loop->next;
    }
}

void printf_var(struct nvar *var) {
    writeStr( "<var>");
    printf_id_list(var->id_list);
    printf_expr(var->result);
    printf_expr(var->array_expr_1);
    printf_expr(var->array_expr_2);
    writeStr("</var>");
}

void printf_val(struct nval* val) {
    writeStr("<val>");
    printf_id_list(val->ids_list);
    writeStr("</val>");
}

void printf_func(struct nfunc *func) {
    writeStr( strcat_3("<function id=\"", func->name, "\">"));
    printf_args(func->_args);
    printf_expr(func->return_var);
    printf_stmt(func->body);
    writeStr("</function>");
}

void printf_class(struct nclass *_class) {
    writeStr( strcat_3("<class id=\"", _class->name, "\">"));
    printf_args(_class->args);
    printf_stmt_list(_class->stmt_list);
    writeStr("</class>");
}

void printf_expr(struct expression *expr) {
    if( expr == NULL ) return;
    char str[10];
    switch(expr->type) {
    case id:
        writeStr( strcat_3("<expression type=\"ID\" id=\"", expr->String, "\">"));
        break;
    case Int:
        sprintf(expr->Int, "%d", str);
        writeStr( strcat_3("<expression type=\"const_int\" value=\"", str, "\">"));
        break;
    case Float:
        sprintf(str, "%f", expr->Float);
        writeStr( strcat_3("<expression type=\"const_float\" value=\"", str, "\">"));
        break;
    case Char:
        printf("%c\n", expr->Char);
        break;
    case String:
        writeStr( strcat_3("<expression type=\"const_string\" value=\"", expr->String, "\">"));
        break;
    case Null:
        writeStr("<expression type=\"NULL\">");
        break;
    case Type:
        writeStr("<expression type=\"Array[]\">");
        printf_expr(expr->left);
        break;
    case assigment:
        writeStr("<expression type=\"=\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case equal:
        writeStr("<expression type=\"==\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case no_equal:
        writeStr("<expression type=\"!=\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case no:
        writeStr("<expression type=\"!\">");
        printf_expr(expr->left);
        break;
    case more:
        writeStr("<expression type=\">\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case more_eq:
        writeStr("<expression type=\">=\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case less:
        writeStr("<expression type=\"<\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case less_eq:
        writeStr("<expression type=\"<=\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case plus:
        writeStr("<expression type=\"+\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case plus_eq:
        writeStr("<expression type=\"+=\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case usub:
        writeStr("<expression type=\"-(unar)\">");
        printf_expr(expr->left);
        break;
    case sub:
        writeStr("<expression type=\"-\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case sub_eq:
        writeStr("<expression type=\"-=\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case mul:
        writeStr("<expression type=\"*\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case mul_eq:
        writeStr("<expression type=\"*=\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case div_eq:
        writeStr("<expression type=\"/=\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case div_residue:
        writeStr("<expression type=\"%=\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case unarOR:
        writeStr("<expression type=\"|\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case _or:
        writeStr("<expression type=\"||\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case or_equal:
        writeStr("<expression type=\"|=\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case unarXOR:
        writeStr("<expression type=\"^\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case _xor:
        writeStr("<expression type=\"^^\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case unarAND:
        writeStr("<expression type=\"&\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case _and:
        writeStr("<expression type=\"&&\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case and_equal:
        writeStr("<expression type=\"&=\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case call_func:
        writeStr("<expression type=\"call_function\">");
        printf_expr_list(expr->expr_list);
        break;
    case prefix_inc:
        writeStr("<expression type=\"++expr\">");
        printf_expr(expr->left);
        break;
    case prefix_dec:
        writeStr("<expression type=\"--expr\">");
        printf_expr(expr->left);
        break;
    case postfix_inc:
        writeStr("<expression type=\"expr++\">");
        printf_expr(expr->left);
        break;
    case postfix_dec:
        writeStr("<expression type=\"expr--\">");
        printf_expr(expr->left);
        break;
    case point:
        writeStr("<expression type=\".\">");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case println: break;
    }
    writeStr("</expression>");
}

void printf_id_list(struct id_list *_id_list) {
    writeStr("<id_list>");
    struct id_list *idList =_id_list;
    while ( idList != NULL ) {
        writeStr(strcat_3("<id value=\"", idList->id, "\"/>"));
        idList = idList->next;
    }
    writeStr("</id_list>");
}

void printf_args(struct nargs *_args) {
    struct nargs *args = _args;
    writeStr("<args>");
    while ( args != NULL ) {
        printf_expr(args->expr);
        args = args->next;
    }
    writeStr("</args>");
}

#endif
