#include "../../../include/errors.h"
#include "../../../include/tokens.h"

// alpha or alpha.alpha
int expr_from(token_array_t *tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"fromEXPR","IN");
    #endif
    int pos=tokens->position;
    
    if(compare_token(tokens,0,TOKEN_FROM)){
        if(expr_identifier(tokens,depth)){        
            
            if(expr_alias(tokens,depth)) {
            }
            
            set_from(tokens);

            return 1;
        }
    }
    tokens->position=pos;
    return 0;
}

void set_from(token_array_t *tokens){
    //tokens->dataset.identity=tokens->alias;
    //tokens->dataset->identifier.source=tokens->identifier.source;
    //tokens->dataset->identifier.qualifier=tokens->identifier.qualifier;
}
