#include "../../include/errors.h"
#include "../../include/tokens.h"

int compare_token(token_array_t *tokens,int optional,int token){
    if(tokens->position>tokens->length) return 0;

    if(tokens->array[tokens->position].type==token) {
        ++tokens->position;
        return 1;
    }
    if(optional==1) {
        return 1;
    }
    return 0;
}

