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
    ++tokens->position;

    if(compare_token(tokens,0,TOKEN_ALPHA)){
        goop(depth,"join identity",tokens->array[tokens->position].value);    
    } else {
        ghost(ERR_INVALID_JOIN_IDENTITY);
    }

    goop(depth,"join pre AS",tokens->array[tokens->position].value);
    if(compare_token(tokens,0,TOKEN_AS)){
            goop(depth,"join as",tokens->array[tokens->position].value);
        if(compare_token(tokens,0,TOKEN_ALPHA)){
            goop(depth,"join alias",tokens->array[tokens->position].value);
        } else {
            ghost(ERR_INVALID_JOIN_ALIAS);
        }
    }

    goop(depth,"join pre on",tokens->array[tokens->position].value);
    if(compare_token(tokens,0,TOKEN_ON)) {
        goop(depth,"join on",tokens->array[tokens->position].value);
        if(expr_expr(tokens,depth,sel)){
            tokens->object=0;
            return 1;
        } else {
            ghost(ERR_JOIN_WITHOUT_EXPR);
        }
    } else {
        ghost(ERR_JOIN_WITHOUT_ON);
    }
    goop(depth,"join fail",tokens->array[tokens->position].value);

    return 0;
}
    