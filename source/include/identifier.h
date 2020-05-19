#include "tokens.h"

#if !defined(_IDENTIFIER_H_)
    #define _IDENTIFIER_H_ 1

    int expr_identifier(token_array_t *tokens,int depth);
    void set_identifier(token_array_t *tokens,int index1,int index2);

#endif