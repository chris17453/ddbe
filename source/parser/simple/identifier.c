#include "../../include/errors.h"
#include "../../include/tokens.h"

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
                set_identifier(tokens,tokens->position-3,tokens->position-1);
                return 1;
            } else{
                tokens->position=pos;
                return 0;
            }
        } 
        
        set_identifier(tokens,-1,tokens->position-1);
        
        return 1;
    }
    return 0;
}

void set_identifier(token_array_t *tokens,int index1,int index2){
    char *db;
    char *table;
    if(tokens->target==0) {
        ghost(ERR_TOKEN_TARGET_NULL);
    }
    identifier_t *ident=tokens->target;

    if(valid_token_index(tokens,index1)){
        db   =tokens->array[index1].value;
    } 
    if(valid_token_index(tokens,index2)){
        table=tokens->array[index2].value;
    }
    if(index1>-1){
        ident->qualifier=db;
        goop(0,"identity",db);
    } else {
        ident->qualifier="";
    }
    goop(0,"identity",table);
    ident->source=table;
}