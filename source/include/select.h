#include "tokens.h"

#if !defined(_SELECT_EXPR_H_)
    #define _SELECT_EXPR_H_ 1


    // helpers
    token_t       * token_at                 (token_array_t *tokens,int  index);
    token_t       * duplicate_token          (token_array_t *tokens,int  index);
    char          * copy_token_value_at      (token_array_t *tokens,int  index);
    int             add_expr                 (expression_t *expression,expression_t *item);
    // workers
    char          * process_alias            (token_array_t *tokens,int *index);
    token_t       * process_litteral         (token_array_t *tokens,int *index);
    expression_t  * process_simple_expr      (token_array_t *tokens,int *index);
    expression_t  * process_bit_expr         (token_array_t *tokens,int *index);
    expression_t  * process_expr_list        (token_array_t *tokens,int *index);
    expression_t  * process_predicate        (token_array_t *tokens,int *index);
    expression_t  * process_boolean_primary  (token_array_t *tokens,int *index);
    expression_t  * process_expression       (token_array_t *tokens,int *index);
    expression_t  * process_group_column_list(token_array_t *tokens,int *index);
    expression_t  * process_order_column_list(token_array_t *tokens,int *index);
    select_t      * process_select           (token_array_t *tokens,int *start);
    // cleaners
    int             free_select              (select_t *select) ;
    int             free_string              (char *data);
    int             free_expression          (expression_t *expr);
    int             free_ident               (identifier_t *ident);
    int             free_litteral            (token_t *token);
    // debuggers
    void            select_print             (select_t *select);
    void            debug_expr               (expression_t *expr,int depth);

    
    void select_debug();
#endif  
