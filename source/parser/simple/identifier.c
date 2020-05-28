#include "../../include/errors.h"
#include "../../include/tokens.h"
#include "../../include/structure.h"

// alpha or alpha.alpha
int expr_identifier(token_array_t *tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"identifier","in");
    #endif

    if(compare_token(tokens,0,TOKEN_ALPHA)){
        int pos=tokens->position;
        if(compare_token(tokens,0,TOKEN_DOT)){
            if(compare_token(tokens,0,TOKEN_ALPHA)){
                create_identifier(depth,tokens,tokens->position-3,tokens->position-1);
                return 1;
            } else{
                tokens->position=pos;
                return 0;
            }
        } 
        
        create_identifier(depth,tokens,-1,tokens->position-1);
        
        return 1;
    }
    return 0;
}

void create_identifier(int depth,token_array_t *tokens,int index1,int index2){
    #ifdef PARSE_ENTRANCE
    goop(0,"create_identifier","in");
    #endif


    if(valid_token_index(tokens,index1)){
        token_add_type(tokens,TOKEN_IDENTIFIER,index1);
        token_delete(tokens,index1+1);
        --index2;
        token_set_type(tokens,TOKEN_QUALIFIER,index1);
    } 
    if(valid_token_index(tokens,index2)){
        token_add_type(tokens,TOKEN_IDENTIFIER,index2);
        token_set_type(tokens,TOKEN_SOURCE,index2);
    }
    return;
}