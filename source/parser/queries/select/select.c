#include "../../../include/errors.h"
#include "../../../include/tokens.h"

int expr_select(token_array_t *tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"SELECT","IN");
    #endif

    int pos=tokens->position;
    if(compare_token(tokens,0,TOKEN_SELECT)){
        // optional
        if(compare_token(tokens,0,TOKEN_DISTINCT)){
            goop(depth,"SELECT","DISTINCT");
        }
        // not optional
        if(expr_select_expr(tokens,depth)){
            goop(depth,"SELECT","FOUND select expression");
        } else {
            goop(depth,"UHOH","erms?");
            tokens->position=pos;
            return 0;
        }
        
        if(expr_from(tokens,depth)){
            if(expr_where(tokens,depth)){
            }
            if(expr_limit(tokens,depth)){
            }
        }
        return 1;
    }
    tokens->position=pos;

    return 0;
}
