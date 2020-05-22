#include "include/errors.h"
#include "include/gramar.h"



gramar_t *gramar_create(token_array_t *tokens,int start,int end){
    int length=end-start;
    if(length<=0 || start<0 || end<0 || end<start) {
        ghost(ERR_OUT_OF_BOUNDS);
    }
    gramar_t * gramar = ( gramar_t*)malloc(sizeof(gramar_t)); 
    gramar->length = length; 
    gramar->tokens = &tokens->array[start];
    
    return tokens; 
} 
