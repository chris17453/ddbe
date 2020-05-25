#include "tokens.h"
#include "structure.h"

#if !defined(_FROM_H_)
    #define _FROM_H_ 1
    int expr_from(token_array_t *tokens,int depth,select_t *sel);
    void set_from(token_array_t *tokens,select_t* sel);

#endif