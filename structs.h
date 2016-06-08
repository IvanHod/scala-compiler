#ifndef STRUCTS
#define STRUCTS

#include <stdbool.h>

struct Root *root;

enum expr_type{
    id,
    Int,
    Bool,
    Float,
    Char,
    String,
    HardString,
    Null,
    Type,
    assigment,
    equal,
    no_equal,
    no,
    more,
    more_eq,
    less,
    less_eq,
    plus,
    plus_eq,
    usub,
    sub,
    sub_eq,
    mul,
    mul_eq,
    DIV,
    div_eq,
    div_residue,
    unarOR,
    _or,
    or_equal,
    unarXOR,
    _xor,
    unarAND,
    _and,
    and_equal,
    call_func,
    prefix_inc,
    prefix_dec,
    postfix_inc,
    postfix_dec,
    point,
    println_s,
    println
};

struct hardString {
    char* left;
    struct expression *expr;
    char* right;
    struct hardString *next;
};

struct expression {
    enum expr_type type;
    struct expression *left;
    struct expression *rigth;
    struct expression_list *expr_list;
    bool boolean;
    int Int;
    float Float;
    char Char;
    char *String;
    struct hardString *hString;
    struct expression *next;
};

struct expression_list {
    struct expression *first;
    struct expression *last;
};

enum statement_type {
    EXPR_LIST,
    STMT_LIST,
    NIF,
    NIF_LOOP,
    LOOP,
    NMATCH,
    NVAR,
    NVAL,
    NFUNC,
    NCLASS,
    NOBJECT
};

enum modifires {
    MOD_PUBLIC,
    MOD_PROTECTED,
    MOD_PRIVATE
};

struct statement {
    enum statement_type type;
    enum modifires mod;
    struct expression_list *expr_list;
    struct statement_list *stmt_list;
    struct nif *_if;
    struct loop *_loop;
    struct nvar *to_print_var;
    struct nval *to_print_val;
    struct nfunc *to_print_func;
    struct match *to_print_match;
    struct nclass *to_print_class;
    struct nobject *to_print_object;
    struct statement *next;
};

struct statement_list {
    struct statement *first;
    struct statement *last;
};

struct Root {
    char* id;
    struct statement_list *stmt_list;
};

struct nif {
    struct expression *expr;
    struct statement *to_then;
    struct statement *to_else;
};

struct nif_loop {
    struct expression *expr;
    struct nif_loop * next;
};

struct nargs {
    struct narg *first;
    struct narg *last;
};

struct narg {
    char* id;
    struct expression *expr;
    struct narg *next;
};

enum type_cicle {
    CICLE_FOR,
    CICLE_WHILE
};

struct loop {
    enum type_cicle type;
    char *id;
    struct nif_loop *if_loop;
    struct expression *expr_1;
    struct expression *expr_2;
    struct expression *expr_while;
    struct statement *stmt;
};

struct nvar {
    struct id_list    *id_list;
    struct expression *return_value;
    struct expression *result;
    struct expression *array_expr_1;
    struct expression *array_expr_2;
    struct expression_list *array_expr_list;
};

struct nval {
    struct id_list *ids_list;
    struct expression *expr;
    struct expression *return_value;
};

struct nfunc {
    char *name;
    struct args *_args;
    struct expression *return_var;
    struct statement *body;
};

struct nclass {
    char* name;
    struct nargs *args;
    struct statement_list *stmt_list;
};

struct nobject {
    char * name;
    struct statement_list *stmt_list;
};

struct id_list {
    char* id;
    struct id_list *next;
};

struct case_list {
    struct one_case* first;
    struct one_case* last;
};

struct one_case {
    struct expression *condition;
    struct expression *perfomance;
    struct one_case *next;
};

struct match {
    struct expression *id;
    struct case_list *list;
};

/*------------------------------------ semantic structs ----------------------------------------- */

enum TypeNames {
    IDENTIFIER_TYPE_NAME,
    FLOAT32_TYPE_NAME,
    INT_TYPE_NAME,
    STRING_TYPE_NAME,
    BOOL_TYPE_NAME,
    VOID_TYPE_NAME,
    UNKNOWN_TYPE
};

struct SemanticType {
    enum TypeNames typeName;
    int idNum;
};

#endif
