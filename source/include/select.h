#include "tokens.h"

#if !defined(_SELECT_EXPR_H_)
    #define _SELECT_EXPR_H_ 1

    int expr_select(token_array_t *tokens,int depth);
    select_t *select_new();
    
    void select_debug(select_t *select);
#endif  
