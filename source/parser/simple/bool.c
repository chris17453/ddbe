#include "../../include/errors.h"
#include "../../include/tokens.h"

// is the next expression a bool
int expr_bool(token_array_t* tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"BOOL,","IN");
    #endif

    if (tokens->array[tokens->position].type==TOKEN_TRUE || 
        tokens->array[tokens->position].type==TOKEN_FALSE || 
        tokens->array[tokens->position].type==TOKEN_UNKNOWN){
            set_bool(tokens,tokens->position);
            ++tokens->position;
            return 1; 
    }
    return 0;
}


void set_bool(token_array_t *tokens,int index){
    goop(0,"bool",tokens->array[index].value);
}