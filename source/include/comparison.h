#include "tokens.h"

#if !defined(_COMPARISON_H_)
    #define _COMPARISON_H_ 1

    int expr_comparison_operator(token_array_t *tokens,int depth);
    void set_comparison(token_array_t *tokens,int index);

#endif