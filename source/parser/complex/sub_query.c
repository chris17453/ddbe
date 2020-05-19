#include "../../include/errors.h"
#include "../../include/tokens.h"

int expr_sub_query(token_array_t* tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"sub_query","IN");
    #endif

    int pos=tokens->position;

    if(tokens->array[tokens->position].type==TOKEN_PAREN_LEFT){
        ++tokens->position;
        if(expr_select(tokens,depth)){
            if(tokens->array[tokens->position].type==TOKEN_PAREN_RIGHT){
                ++tokens->position;
                return 1;
            }
        }
    }
    
    tokens->position=pos;
    return 0;
}
