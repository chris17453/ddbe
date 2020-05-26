#include "../../include/errors.h"
#include "../../include/tokens.h"

// is the next expression a litteral 
int expr_litteral(token_array_t* tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"LITTERAL","IN");
    #endif

    switch(tokens->array[tokens->position].type){
        case TOKEN_NULL:
        case TOKEN_HEX:
        case TOKEN_BINARY:
        case TOKEN_STRING:
        case TOKEN_NUMERIC:
        case TOKEN_REAL: 
                        
                        //return create_litteral(tokens,tokens->position);
                        ++tokens->position; 
                        return 1;
    }
    return 0;
}
/*
void create_litteral(token_array_t *tokens,int index){
    #ifdef PARSE_ENTRANCE
    goop(0,"create_litteral","in");
    #endif

    token_t token;
    token.type=&tokens->array[index].type;
    token.value=strdup(&tokens->array[index].value)
    return token;
    //tokens->object=&tokens->array[index];
    //tokens->object_type=TOKEN_LITTERAL;
}

typedef struct fragment{
    token_t *tokens;
    int length;
    int type;
};

token_array{}
*/