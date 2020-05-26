#include "tokens.h"
#include "structure.h"

#if !defined(_EXPR_H_)
    #define _EXPR_H_ 1

    int expr_core(token_array_t *tokens,int depth);
    int expr_expr(token_array_t* tokens,int depth,select_t *sel);

#endif