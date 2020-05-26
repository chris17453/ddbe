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


    if(tokens->target==0) {
        ghost(ERR_TOKEN_TARGET_NULL);
    }
    
    identifier_t *ident=safe_malloc(sizeof(identifier_t),1);
    if(valid_token_index(tokens,index1)){
        token_add_type(TOKEN_IDENTIFIER,index1);
        token_type(TOKEN_QUALIFIER,index2);
        ident->qualifier   =tokens->array[index1].value;
    } 
    if(valid_token_index(tokens,index2)){
        token_add_type(TOKEN_IDENTIFIER,index2);
        token_type(TOKEN_SOURCE,index2);
        ident->source=tokens->array[index2].value;
    }
    tokens->object=ident;
    tokens->object_type=TOKEN_IDENTIFIER;
    return;
}