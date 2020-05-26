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
            int length=tokens->position-pos-1;
            printf("length %d",length);
            sel->where=safe_malloc(sizeof(where_expr_t),1);

            sel->where->tokens=&tokens->array[pos+1];
            sel->where->length=length;
            sel->where->ordinal=1;
            ++sel->where_length;
            return 1;
        }
        printf("OMG NO");

    }
    tokens->position=pos;
    return 0;
}

