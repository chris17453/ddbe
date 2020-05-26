#include "../../../include/errors.h"
#include "../../../include/structure.h"

int expr_join(token_array_t *tokens,int depth,select_t *sel){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"expr_join","IN");
    #endif


    switch(tokens->array[tokens->position].type){
        case TOKEN_JOIN: break;
        case TOKEN_LEFT_JOIN: break;
        case TOKEN_RIGHT_JOIN: break;
        case TOKEN_INNER_JOIN: break;
        case TOKEN_FULL_OUTER_JOIN: break;
        default: return 0;
    }
    tokens->position++;

    if(compare_token(tokens,0,TOKEN_AS)){
        if(compare_token(tokens,0,TOKEN_ALPHA)){
            goop(depth,"join alias",tokens->array[tokens->position].value);
        } else {
            ghost(ERR_INVALID_JOIN_ALIAS);
        }
    }

    if(compare_token(tokens,0,TOKEN_ON)) {
        if(expr_expr(tokens,depth,sel)){
            return 1;
        } else {
            ghost(ERR_JOIN_WITHOUT_EXPR);
        }
    } else {
        ghost(ERR_JOIN_WITHOUT_ON);
    }
    return 0;
}
    