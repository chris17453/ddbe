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
        goop(depth,"SELECT","BEFORE FROM-ident");

        if(expr_identifier(tokens,depth)){        
            goop(depth,"SELECT","BEFORE FROM-alias");
            
            if(expr_alias(tokens,depth,sel->alias)) {
                goop(depth,"FORM","Found Alias");
            }
            
            set_from(tokens);

            return 1;
        }
    }
    tokens->position=pos;
    return 0;
}

void set_from(token_array_t *tokens){
    //tokens->selectidentifier->source=tokens->identifier.source;
    //tokens->identifier-qualifier=tokens->identifier.qualifier;
}
