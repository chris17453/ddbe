#include "../../include/errors.h"
#include "../../include/tokens.h"

// alpha or alpha.alpha
int expr_where(token_array_t *tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"WHERE EXPR","IN");
    #endif

    int pos=tokens->position;

    if(compare_token(tokens,0,TOKEN_WHERE)){
        if(expr_expr(tokens,depth)){        
            return 1;
        }
    }
    tokens->position=pos;
    return 0;
}

