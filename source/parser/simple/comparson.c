#include "../../include/errors.h"
#include "../../include/tokens.h"

int expr_comparison_operator(token_array_t *tokens,int depth){
    #ifdef PARSE_ENTRANCE
    goop(depth,"comparison operator,","IN");
    #endif

    ++depth;
    
    switch(tokens->array[tokens->position].type){
        case TOKEN_LESS_EQ:
        case TOKEN_GREATER_EQ:
        case TOKEN_LESS:
        case TOKEN_GREATER:
        case TOKEN_NOT_EQ:
        case TOKEN_ASSIGNMENT: token_add_type(tokens,TOKEN_COMPARITOR,tokens->position);
                               ++tokens->position; 
                               return 1;
        default:
            return 0;
    }
}

