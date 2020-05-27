#include "../../include/errors.h"
#include "../../include/tokens.h"
#include "../../include/structure.h"

// alpha or alpha.alpha
int expr_where(token_array_t *tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"WHERE EXPR","IN");
    #endif

    int pos=tokens->position;

    if(compare_token(tokens,0,TOKEN_WHERE)){
        if(expr_expr(tokens,depth,sel)){
            token_add_type_range(tokens,TOKEN_WHERE,pos);

            return 1;
        }

    }
    tokens->position=pos;
    return 0;
}

/*
where_expr_t * where_expr_new(){
        int length=tokens->position-pos-1;
        printf("length %d",length);
        sel->where=safe_malloc(sizeof(where_expr_t),1);

        sel->where->tokens=&tokens->array[pos+1];
        sel->where->length=length;
        sel->where->ordinal=1;
        ++sel->where_length;


    }        


*/