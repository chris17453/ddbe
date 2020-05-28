#include "../../../include/errors.h"
#include "../../../include/tokens.h"
#include "../../../include/structure.h"
#include <math.h>
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
                    token_set_type(tokens,TOKEN_LIMIT_START,tokens->position-3);
                    token_set_type(tokens,TOKEN_LIMIT_LENGTH,tokens->position-1);
                    token_add_type(tokens,TOKEN_LIMIT_START,tokens->position-3);
                    token_add_type(tokens,TOKEN_LIMIT_START,tokens->position-1);
                    token_add_type_range(tokens,TOKEN_LIMIT,pos);

                    token_delete(tokens,tokens->position-2);
                    token_delete(tokens,tokens->position-3);

                    return 1;
                }                
            } else {
                token_set_type(tokens,TOKEN_LIMIT_LENGTH,tokens->position-1);
                token_add_type(tokens,TOKEN_LIMIT_LENGTH,tokens->position-1);
                token_add_type_range(tokens,TOKEN_LIMIT,pos);
                token_delete(tokens,tokens->position-2);
                return 1;
            }
        }
    }
    tokens->position=pos;
    return 0;
}
