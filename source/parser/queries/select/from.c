#include "../../../include/errors.h"
#include "../../../include/tokens.h"
#include "../../../include/structure.h"

// alpha or alpha.alpha
int expr_from(token_array_t *tokens,int depth,select_t* sel){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"fromEXPR","IN");
    #endif
    int pos=tokens->position;
    
    if(compare_token(tokens,0,TOKEN_FROM)){
        if(expr_identifier(tokens,depth)){        
            sel->from=tokens->object;
            if(expr_alias(tokens,depth,&sel->alias)) {
            }
            
            set_from(tokens,sel);

            return 1;
        }
    }
    tokens->position=pos;
    return 0;
}

void set_from(token_array_t *tokens,select_t* sel){
    //tokens->selectidentifier->source=tokens->identifier.source;
    //tokens->identifier-qualifier=tokens->identifier.qualifier;
}
