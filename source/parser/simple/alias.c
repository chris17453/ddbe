#include "../../include/errors.h"
#include "../../include/tokens.h"

// alpha or alpha.alpha
int expr_alias(token_array_t *tokens,int depth,char **target){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"ALIASEXPR","IN");
    #endif
    
    
    int pos=tokens->position;
    if(compare_token(tokens,0,TOKEN_AS))
        if(compare_token(tokens,0,TOKEN_ALPHA)){        
            token_add_type(tokens,TOKEN_ALIAS,tokens->position-1);
            return 1;
        }

    tokens->position=pos;
    return 0;
}


