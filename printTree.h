#ifndef PRINTTREE
#define PRINTTREE

#include "structs.h"

void printf_root();
void printf_stmt_list(struct statement_list *);
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

void printf_root() {
    printf_stmt_list(root->stmt_list);
}

void printf_stmt_list(struct statement_list *_stmt_list) {
    struct statement_list stmt_list = _stmt_list;
    while ( stmt_list != NULL ) {
        printf_stmt(stmt_list.stmt);
        stmt_list = stmt_list.next;
    }
}

void printf_stmt(struct statement *stmt) {
    switch (stmt->type) {
    case EXPR_LIST  :
        printf_expr_list(stmt->expr_list);
        break;
    case STMT_LIST  :
        printf_stmt_list(stmt->stmt_list);
        break;
    case NIF:
        printf_if(stmt->_if);
        break;
    case NIF_LOOP   :
        printf_if_loop(stmt->loop);
        break;
    case NVAR       :
        printf_if_loop(stmt->to_print_var);
        break;
    case NVAL       :
        printf_if_loop(stmt->to_print_val);
        break;
    case NFUNC      :
        printf_if_loop(stmt->to_print_func);
        break;
    case NCLASS     :
        printf_if_loop(stmt->to_print_class);
        break;
    }
}


void printf_expr_list(struct expression_list *_expr_list) {
    struct expression_list expr_list = _expr_list;
    while ( expr_list != NULL ) {
        printf_expr(expr_list.expr);
        expr_list = expr_list.next;
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
    struct nif_loop if_loop = _if_loop;
    while ( if_loop != NULL ) {
        printf_expr(if_loop.expr);
        if_loop = if_loop.next;
    }
}

void printf_var(struct nvar *var) {
    printf_id_list(var->id_list);
    printf("return value - %s\n", var->return_value);
    printf_expr(var->result);
    printf_expr(var->array_expr_1);
    printf_expr(var->array_expr_2);
}

void printf_val(struct nval* val) {
    printf_id_list(val->id_list);
    printf("return value - %s\n", val->return_value);
}

void printf_func(struct nfunc *func) {
    printf("function %s\n", func->name);
    printf_args(func->_args);
    printf_stmt(func->body);
    printf_expr(func->return_var);
}

void printf_class(struct nclass *_class) {
    printf("class %s\n", _class->name);
    printf_args(_class->args);
    printf_stmt_list(_class->stmt_list);
}

void printf_expr(struct expression *expr) {
    switch(expr->type) {
    case id:
        printf("%s\n", expr->String);
        break;
    case Int:
        printf("%d\n", expr->Int);
        break;
    case Float:
        printf("%f\n", expr->Float);
        break;
    case Char:
        printf("%c\n", expr->Char);
        break;
    case String:
        printf("%d\n", expr->String);
        break;
    case Null:
        printf("NULL\n");
        break;
    case Type:
        printf("%d\n", expr->Int);
        break;
    case assigment:
        printf("=\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case equal:
        printf("==\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case no_equal:
        printf("!=\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case no:
        printf("=\n");
        printf_expr(expr->left);
        break;
    case more:
        printf(">\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case more_eq:
        printf(">=\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case less:
        printf("<\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case less_eq:
        printf("<=\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case plus:
        printf("+\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case plus_eq:
        printf("+=\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case usub:
        printf("-\n");
        printf_expr(expr->left);
        break;
    case sub:
        printf("-\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case sub_eq:
        printf("-=\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case mul:
        printf("*=\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case mul_eq:
        printf("*=\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case div_eq:
        printf("/=\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case div_residue:
        printf("%=\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case unarOR:
        printf("|\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case _or:
        printf("||\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case or_equal:
        printf("|=\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case unarXOR:
        printf("&\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case _xor:
        printf("^\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case unarAND:
        printf("&\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case _and:
        printf("&&\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case and_equal:
        printf("&=\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case call_func:
        printf("function %s\n", expr->String);
        printf_expr_list(expr->expr_list);
        break;
    case prefix_inc:
        printf("++\n");
        printf_expr(expr->left);
        break;
    case prefix_dec:
        printf("--\n");
        printf_expr(expr->left);
        break;
    case postfix_inc:
        printf("++\n");
        printf_expr(expr->left);
        break;
    case postfix_dec:
        printf("--\n");
        printf_expr(expr->left);
        break;
    case point:
        printf(".\n");
        printf_expr(expr->left);
        printf_expr(expr->rigth);
        break;
    case println: break;
    }
}

void printf_id_list(struct id_list *_id_list) {
    struct id_list *idList =_id_list;
    while ( idList != NULL ) {
        printf("id: %s", idList->id);
        idList = idList->next;
    }
}

void printf_args(struct nargs *_args) {
    struct nargs *args = _args;
    while ( args != NULL ) {
        printf("id: %s", args->id);
        printf_expr(args->expr);
        args = args->next;
    }
}

#endif
