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
                        set_litteral(tokens,tokens->position);
                        ++tokens->position; 
                        return 1;
    }
    return 0;
}

void set_litteral(token_array_t *tokens,int index){
    goop(0,"Litteral",tokens->array[index].value);
    tokens->object=(void *)tokens->array[index];
    tokens->object_type=TOKEN_LITTERAL;

}