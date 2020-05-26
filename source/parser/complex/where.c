#include "../../include/errors.h"
#include "../../include/tokens.h"
#include "../../include/structure.h"

// alpha or alpha.alpha
int expr_where(token_array_t *tokens,int depth,select_t *sel){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"WHERE EXPR","IN");
    #endif

    int pos=tokens->position;

    if(compare_token(tokens,0,TOKEN_WHERE)){
        if(expr_expr(tokens,depth)){
            printf("OMG YES");

            sel->where=safe_malloc(sizeof(where_expr_t),2);
            sel->where->tokens=&tokens->array[pos+1];
            sel->where->length=1;
            sel->where->ordinal=1;
            return 1;
        }
        printf("OMG NO");

    }
    tokens->position=pos;
    return 0;
}

