
#include "tokens.h"

#if !defined(_STRUCTURE_H_)
    #define _STRUCTURE_H_ 1



typedef struct identifier_t {
    char * qualifier;
    char * source;
} identifier_t;

typedef struct  data_column_t{
    int    type;
    int    ordinal;
    void * object;
    char * alias;
} data_column_t;

typedef struct order_column_t {
    identifier_t * identifier;
    int direction;
    int ordinal;
} order_column_t;

typedef struct group_column_t {
    identifier_t * identifier;
    int ordinal;
} group_column_t;

typedef struct expression_t{
    int mode;
    int list;                 // expr list
    int not;                  // expr 
    int not_in;               // predicate 
    int in;                   // predicate
    int negative;             // simple expr  
    int positive;             // simple expr             
    int comparitor;           // expression
    int operator;             // bit_expr
    
    //this can be a LITTERAL OR a IDENTITY
    identifier_t *identifier;
    token_t      *literal;
    struct expression_t *expression;
} expression_t;



typedef struct join_t {
    int             type;
    identifier_t  * identifier;
    char          * alias;
    expression_t  * expression;
    int             expression_length;
    int             ordinal;
} join_t;


typedef struct where_expr_t {
    int NOT;
    token_t * tokens;
    int       comparitor;
    int       length;
    int       ordinal;
} where_expr_t;

typedef struct comparison_t {
    identifier_t * identity;
    int            comparitor;
    //identifier_t *identity;
} comparison_t;


/****

identity comparitor expr
identity not in list
identity in list

 *****/


typedef struct select_t{
        // sub elements
        data_column_t   * columns;
        identifier_t    * from;
        join_t          * join;
        where_expr_t    * where;
        order_column_t  * order;
        group_column_t  * group;
        
        // elements
        char          * alias;
        int             distinct;
        int             limit_start;
        int             limit_length;

        // internal
        int             column_length;     
        int             join_length;     
        int             where_length;     
        int             order_length;     
        int             group_length;     
        int             has_limit_length;
        int             has_limit_start;
    } select_t;
    

    select_t       * select_new();
    data_column_t  * data_column_list_init(int length);
    void             add_data_column(select_t *obj);
    void             data_column_init(data_column_t *column);
    join_t          *join_list_init(int length);
    void             join_init(join_t *join);
    void             add_join(select_t *obj);

    order_column_t * order_column_list_init(int length);
    void             order_column_init(order_column_t *column);
    void             add_order_column(select_t *obj);
    group_column_t * group_column_list_init(int length);
    void             group_column_init(group_column_t *column);
    void             add_group_column(select_t *obj);
    where_expr_t   * where_expr_list_init(int length);
    void             where_expr_init(where_expr_t *exp);
    void             add_where_expr(select_t *obj);

    void             set_distinct(select_t *obj);
    void             debug(token_array_t *tokens);

#endif