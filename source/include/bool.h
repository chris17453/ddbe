#include "tokens.h"

#if !defined(_BOOL_H_)
    #define _BOOL_H_ 1
    
    int expr_bool(token_array_t* tokens,int depth);
    void set_bool(token_array_t *tokens,int index);


#endif