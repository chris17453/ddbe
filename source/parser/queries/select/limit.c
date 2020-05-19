#include "../../../include/errors.h"
#include "../../../include/tokens.h"

// alpha or alpha.alpha
int expr_limit(token_array_t *tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"limit EXPR","IN");
    #endif

    int start=0;
    int length=0;
    int pos=tokens->position;
    if(compare_token(tokens,0,TOKEN_LIMIT)){
        if(compare_token(tokens,0,TOKEN_NUMERIC)){
            if(compare_token(tokens,0,TOKEN_LIST_DELIMITER)){
                if(compare_token(tokens,0,TOKEN_NUMERIC)){
                    goop(depth,"LIMIT start",tokens->array[tokens->position-3].value);
                    goop(depth,"LIMIT length",tokens->array[tokens->position-1].value);
                    return 1;
                }                
            } else {
                goop(depth,"LIMIT length",tokens->array[tokens->position-1].value);
                return 1;
            }
        }
    }
    tokens->position=pos;
    return 0;
}
