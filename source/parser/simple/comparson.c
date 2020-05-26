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
        case TOKEN_ASSIGNMENT: set_comparison(tokens,tokens->position);
                               ++tokens->position; 
                               return 1;
        default:
            return 0;
    }
}

void set_comparison(token_array_t *tokens,int index){
    #ifdef PARSE_ENTRANCE
    goop(depth,"set_Comparison,","IN");
    #endif
    //goop(0,"Comparison",tokens->array[index].value);
}