#include "../../../include/errors.h"
#include "../../../include/tokens.h"

//identifier or identifier as alias
int expr_column_expr(token_array_t *tokens,int depth){
    ++depth;

    #ifdef PARSE_ENTRANCE
    goop(depth,"COLEXPR","IN");
    #endif

    int pos=tokens->position;
    if(expr_simple_expr(tokens,depth)){
        
        if(expr_alias(tokens,depth)) 
            return 1;
        return 1;
    }
    tokens->position=pos;
    return 0;
}