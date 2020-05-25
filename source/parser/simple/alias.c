#include "../../include/errors.h"
#include "../../include/tokens.h"

// alpha or alpha.alpha
int expr_alias(token_array_t *tokens,int depth,char *target){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"ALIASEXPR","IN");
    #endif
    
    
    int pos=tokens->position;
    if(compare_token(tokens,0,TOKEN_AS))
        if(compare_token(tokens,0,TOKEN_ALPHA)){        
            printf("COMP");
            *target=tokens->array[pos].value;
            printf("COMP3");
            //target=&tokens->array[tokens->position-1].value;
         //   set_alias(tokens,tokens->position-1,target);
            return 1;
        }

    tokens->position=pos;
    return 0;
}


void set_alias(token_array_t *tokens,int index,char *target){
    goop(0,"ALIAS",tokens->array[index].value);
    //target=*tokens->array[index].value;
    //tokens->alias=tokens->array[index].value;
}