#include "../../../include/errors.h"
#include "../../../include/structure.h"

int expr_join(token_array_t *tokens,int depth){
    ++depth;
    int pos=tokens->position;
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

    if(!expr_identifier(tokens,depth)){
        ghost(ERR_INVALID_JOIN_IDENTITY);
    }

    if(expr_alias(tokens)) {

    }
    /*
    if(compare_token(tokens,0,TOKEN_AS)){
        if(compare_token(tokens,0,TOKEN_ALPHA)){
            token_add_type(tokens,TOKEN_ALIAS,tokens->position-1);

        } else {
            ghost(ERR_INVALID_JOIN_ALIAS);
        }
    }*/

    if(compare_token(tokens,0,TOKEN_ON)) {
        if(expr_expr(tokens,depth)){
            token_add_type_range(tokens,TOKEN_JOIN,pos);

            return 1;
        } else {
            ghost(ERR_JOIN_WITHOUT_EXPR);
        }
    } else {
        ghost(ERR_JOIN_WITHOUT_ON);
    }
    return 0;
}
    