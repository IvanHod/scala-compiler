//#ifndef STRUCTS
//#define STRUCTS

struct Root *root;

enum _expr_type{
    ID,
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
    div,
    div_residue,
    unarOR,
    OR,
    unarXOR,
    XOR,
    unarAND,
    AND,
    call_func,
    prefix_inc,
    prefix_dec,
    postfix_inc,
    postfix_dec
};

struct expression {
    enum expr_type type;
    struct expression *left;
    struct expression *rigth;
    struct expression *next;
    //bool boolean;
    int Conts_int;
    float Const_float;
    char Char;
    char *String;
};

struct expression_list {
    struct expression *expr;
    struct expression_list *next;
};

struct statement {
    struct expression_list *to_print_expr;
    struct statement_list *to_ptint_stmt;
    struct nif *to_print_if;
    struct nfor *to_print_for;
    struct nwhile *to_print_while;
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
};

struct nif {
    struct expression *expr;
    struct nif *else_if;
    struct statement *to_then;
    struct statement *to_else;
};

struct nif_loop {

};

enum for_type {
    general,
    foreach
};

struct nfor {
    enum for_type type;

};

struct nargs {

};

struct nwhile {

};

struct nvar {

};

struct nval {

};

struct nfunc {

};

struct nclass {

};

struct ids_list {

};
