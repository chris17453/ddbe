#include "tokens.h"

#if !defined(_ALIAS_H_)
    #define _ALIAS_H_ 1

    int expr_alias(token_array_t *tokens,int depth);
    void set_alias(token_array_t *tokens,int index);

#endif