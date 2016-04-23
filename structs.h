#ifndef STRUCTS
#define STRUCTS

struct Root *root;

enum expr_type{
    id,
    Int,
    Float,
    Char,
    String,
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
    println
};

struct expression {
    enum expr_type type;
    struct expression *left;
    struct expression *rigth;
    struct expression *next;
    struct expression_list *expr_list;
    int boolean;
    int Int;
    float Float;
    char Char;
    char *String;
};

struct expression_list {
    struct expression *expr;
    struct expression_list *next;
};

enum statement_type {
    EXPR_LIST,
    STMT_LIST,
    NIF,
    NIF_LOOP,
    LOOP,
    NVAR,
    NVAL,
    NFUNC,
    NCLASS
};

struct statement {
    enum statement_type type;
    struct expression_list *expr_list;
    struct statement_list *stmt_list;
    struct nif *_if;
    struct nif_loop *loop;
    struct nvar *to_print_var;
    struct nval *to_print_val;
    struct nfunc *to_print_func;
    struct nclass *to_print_class;
};

struct statement_list {
    struct statement *stmt;
    struct statement_list *next;
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
    struct expression *expr;
    struct nargs *next;
    char* id;
};

enum type_cicle {
    cicle_for,
    cicle_while
};

struct loop {
    enum type_cicle type;
    struct nif_loop *if_loop;
    struct expression *expr_1;
    struct expression *expr_2;
    struct expression *expr_while;
    struct statement *stmt;
};

struct nvar {
    struct id_list *id_list;
    char *return_value;
    struct expression* result;
    struct expression *array_expr_1;
    struct expression *array_expr_2;
    struct expression *expr;
};

struct nval {
    struct id_list *ids_list;
    struct expression *expr;
    char *return_value;
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

struct id_list {
    char* id;
    struct id_list *next;
};

#endif
