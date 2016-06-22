#ifndef PRINTTREE
#define PRINTTREE

#include <stdio.h>
#include "structs.h"
#include "semantic.h"
FILE *writeFile;

void printf_root();
void printf_stmt_list(struct statement_list *);
void printf_stmt(struct statement*);
void printf_expr_list(struct expression_list *);
void printf_expr(struct expression*);
void printf_if(struct nif*);
void printf_if_loop(struct nif_loop*);
void printf_loop(struct loop*);
void printf_var(struct nvar*);
void printf_val(struct nval*);
void printf_func(struct nfunc*);
void printf_class(struct nclass*);
void printf_id_list(struct id_list *id_list);
void printf_args(struct nargs *args);
void printf_match(struct match * _match);
void printfHardString(struct hardString *str);

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
    if( str_2 != NULL && strlen(str_2) > 0 )
        strcat(result, str_2);
    if( strlen(str_3) > 0 )
        strcat(result, str_3);
    return result;
}

char* replace(char* str, char* in, char* out) {
    char *outStr = (char *)malloc(sizeof(str) + sizeof(out));
    char *strIn = strstr(str, in);
    if( strIn != NULL ) {
        int endSimbol = strIn - str;
        strncpy(outStr, str, endSimbol);
        strcat(outStr, out);

    } else
        strcpy(outStr, str);
    return outStr;
}

void printf_root() {
    writeStr("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    writeStr("<root>\n");
    printf_stmt_list(root->stmt_list);
    writeStr("</root>\n");
}

void printf_stmt_list(struct statement_list *_stmt_list) {
    if( _stmt_list == NULL ) return;
    writeStr("<statement_list>\n");
    struct statement *stmt = _stmt_list->first;
    while ( stmt != NULL ) {
        printf_stmt(stmt);
        stmt = stmt->next;
    }
    writeStr("</statement_list>\n");
}

void printf_stmt(struct statement *stmt) {
    if( stmt == NULL ) return;
    switch (stmt->type) {
    case EXPR_LIST  :
        writeStr("<statement type=\"expr_list\">\n");
        printf_expr_list(stmt->expr_list);
        break;
    case STMT_LIST  :
        writeStr("<statement type=\"stmt_list\">\n");
        printf_stmt_list(stmt->stmt_list);
        break;
    case NIF:
        writeStr("<statement type=\"if\">\n");
        printf_if(stmt->_if);
        break;
    case NIF_LOOP:
        break;
    case LOOP   :
        writeStr("<statement type=\"loop\">\n");
        printf_loop(stmt->_loop);
        break;
    case NVAR       :
        writeStr("<statement type=\"var\">\n");
        printf_var(stmt->to_print_var);
        break;
    case NVAL       :
        writeStr("<statement type=\"val\">\n");
        printf_val(stmt->to_print_val);
        break;
    case NFUNC      :
        writeStr("<statement type=\"function\">\n");
        printf_func(stmt->to_print_func);
        break;
    case NMATCH      :
        writeStr("<statement type=\"match\">\n");
        printf_match(stmt->to_print_match);
        break;
    case NCLASS     :
        writeStr("<statement type=\"class\">\n");
        printf_class(stmt->to_print_class);
        break;
    case NOBJECT    :
        writeStr( strcat_3("<statement type=\"object\" id=\"", stmt->to_print_object->name, "\">\n"));
        printf_stmt_list(stmt->to_print_object->stmt_list);
        break;
    }
    writeStr("</statement>\n");
}

void printf_match(struct match * _match) {
    writeStr("<match>\n");
    printf_expr(_match->id);
    struct one_case *_case = _match->list->first;
    while ( _case != NULL) {
        writeStr("<case>\n");
        printf_expr(_case->condition);
        printf_expr(_case->perfomance);
        writeStr("</case>\n");
        _case = _case->next;
    }
    writeStr("</match>\n");
}


void printf_expr_list(struct expression_list *expr_list) {
    if(expr_list == NULL) return;
    struct expression *expr = expr_list->first;
    writeStr("<expression_list>\n");
    while ( expr != NULL ) {
        printf_expr(expr);
        expr = expr->next;
    }
    writeStr("</expression_list>\n");
}

void printf_if( struct nif *_nif) {
    if( _nif->expr != NULL ) {
        printf_expr(_nif->expr);
    }
    if( _nif->to_then != NULL ) {
        printf_stmt(_nif->to_then);
    }
    if( _nif->to_else != NULL ) {
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

void printf_loop(struct loop* _loop) {
    switch (_loop->type) {
    case CICLE_FOR:
        writeStr(strcat_3("<loop type=\"FOR\" var=\">", _loop->id, "\">\n") );
        printf_expr(_loop->expr_1);
        printf_expr(_loop->expr_2);
        printf_stmt(_loop->stmt);
        break;
    case CICLE_WHILE:
        writeStr( "<loop type=\"WHILE\">" );
        printf_expr(_loop->expr_while);
        printf_stmt(_loop->stmt);
        break;
    }
    writeStr("</loop>\n");
}

void printf_var(struct nvar *var) {
    writeStr( "<var>\n");
    printf_id_list(var->idList);
    printf_expr(var->result);
    printf_expr(var->return_value);
    /*printf_expr_list(var->array_expr_list);
    printf_expr(var->array_expr_1);
    printf_expr(var->array_expr_2);*/
    writeStr("</var>\n");
}

void printf_val(struct nval* val) {
    writeStr("<val>\n");
    printf_id_list(val->ids_list);
    writeStr("</val>\n");
}

void printf_func(struct nfunc *func) {
    writeStr( strcat_3("<function id=\"", func->name, "\">\n"));
    printf_args(func->_args);
    printf_expr(func->return_var);
    printf_stmt_list(func->body);
    writeStr("</function>\n");
}

void printf_class(struct nclass *_class) {
    writeStr( strcat_3("<class id=\"", _class->name, "\">\n"));
    printf_args(_class->args);
    printf_stmt_list(_class->stmt_list);
    writeStr("</class>\n");
}

void printf_expr(struct expression *expr) {
    if( expr == NULL ) return;
    char str[50];
    switch(expr->type) {
    case id:
        writeStr( strcat_3("<expression type=\"ID\" id=\"", expr->String, "\">\n"));
        break;
    case Int:
        sprintf(str, "%d", expr->Int);
        writeStr( strcat_3("<expression type=\"const_int\" value=\"", str, "\">\n"));
        break;
    case Bool:
        sprintf(str, "%d", expr->boolean);
        writeStr( strcat_3("<expression type=\"const_bool\" value=\"", str, "\">\n"));
        break;
    case Float:
        sprintf(str, "%f", expr->Float);
        writeStr( strcat_3("<expression type=\"const_float\" value=\"", str, "\">\n"));
        break;
    case Char:
        printf("%c\n", expr->Char);
        break;
    case String:
        writeStr( strcat_3("<expression type=\"const_string\" value=\"", expr->String, "\">\n"));
        break;
    case HardString:
            break;
    case Null:
        writeStr("<expression type=\"NULL\">\n");
        break;
    case Type:
        writeStr("<expression type=\"Array[]\">\n");
        printf_expr(expr->left);
        break;
    case assigment:
        writeStr("<expression type=\"=\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case equal:
        writeStr("<expression type=\"==\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case no_equal:
        writeStr("<expression type=\"!=\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case no:
        writeStr("<expression type=\"!\">\n");
        printf_expr(expr->left);
        break;
    case more:
        writeStr("<expression type=\"MORE\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case more_eq:
        writeStr("<expression type=\"MORE_EQ\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case less:
        writeStr("<expression type=\"LESS\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case less_eq:
        writeStr("<expression type=\"LESS_EQ\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case plus:
        writeStr("<expression type=\"+\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case plus_eq:
        writeStr("<expression type=\"+=\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case usub:
        writeStr("<expression type=\"-(unar)\">\n");
        printf_expr(expr->left);
        break;
    case sub:
        writeStr("<expression type=\"-\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case sub_eq:
        writeStr("<expression type=\"-=\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case mul:
        writeStr("<expression type=\"*\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case mul_eq:
        writeStr("<expression type=\"*=\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case DIV:
        writeStr("<expression type=\"/\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case div_eq:
        writeStr("<expression type=\"/=\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case div_residue:
        writeStr("<expression type=\"%=\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case unarOR:
        writeStr("<expression type=\"UNAR_OR\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case _or:
        writeStr("<expression type=\"OR\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case or_equal:
        writeStr("<expression type=\"|=\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case unarXOR:
        writeStr("<expression type=\"^\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case _xor:
        writeStr("<expression type=\"^^\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case unarAND:
        writeStr("<expression type=\"UNAR_AND\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case _and:
        writeStr("<expression type=\"AND\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case and_equal:
        writeStr("<expression type=\"AND_EQ\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case call_func:
        writeStr(strcat_3("<expression type=\"call_function\" id=\"", expr->String, "\">\n"));
        printf_expr_list(expr->expr_list);
        break;
    case prefix_inc:
        writeStr("<expression type=\"++expr\">\n");
        printf_expr(expr->left);
        break;
    case prefix_dec:
        writeStr("<expression type=\"--expr\">\n");
        printf_expr(expr->left);
        break;
    case postfix_inc:
        writeStr("<expression type=\"expr++\">\n");
        printf_expr(expr->left);
        break;
    case postfix_dec:
        writeStr("<expression type=\"expr--\">\n");
        printf_expr(expr->left);
        break;
    case point:
        writeStr("<expression type=\".\">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case println:
        writeStr("<expression type=\"printfln\">\n");
        printf_expr_list(expr->expr_list);
        break;
    case println_s:
        writeStr("<expression type=\"printfln\">\n");
        printfHardString(expr->hString);
        break;
    }
    if( expr->semanticType != NULL )
        writeStr(strcat_3("<semantic_type type=\"", convertTypeToString(expr->semanticType) ,"\" />\n"));
    writeStr("</expression>\n");
}

void printfHardString(struct hardString *str) {
    char tmp[50];
    strcpy(tmp, strcat_3(
                "<printf type=\"s_string\" left=\"",
                str->left,
                "\" right=\""
            ));
    writeStr( strcat_3( tmp, str->right, "\">" ) );
    printf_expr(str->expr);
    writeStr("</printf>\n");
    if( str->next != NULL )
        printfHardString(str->next);
}

void printf_id_list(struct id_list *_id_list) {
    writeStr("<id_list>\n");
    struct id_in_list *idList =_id_list->first;
    while ( idList != NULL ) {
        writeStr(strcat_3("<id value=\"", idList->id, "\"/>\n"));
        idList = idList->next;
    }
    writeStr("</id_list>\n");
}

void printf_args(struct nargs *_args) {
    if( _args == NULL ) return;
    struct narg *arg = _args->first;
    while ( arg != NULL ) {
        writeStr( strcat_3("<arg id=\"", arg->id, "\">\n"));
        printf_expr(arg->expr);
        writeStr("</arg>\n");
        arg = arg->next;
    }
}

#endif
