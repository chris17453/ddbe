#include "../../../include/errors.h"
#include "../../../include/tokens.h"
#include "../../../include/structure.h"
#include <math.h>
// alpha or alpha.alpha
int expr_limit(token_array_t *tokens,int depth,select_t *sel){
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

                    sel->limit_start=atoi(tokens->array[tokens->position-3].value);
                    sel->limit_length=atoi(tokens->array[tokens->position-1].value);
                    sel->has_limit_start=1;
                    sel->has_limit_length=1;
                    
                    return 1;
                }                
            } else {
                sel->limit_length=atoi(tokens->array[tokens->position-1].value);
                sel->has_limit_length=1;
                return 1;
            }
        }
    }
    tokens->position=pos;
    return 0;
}
