#include "../include/errors.h"
#include "../include/tokens.h"
#include "../include/debug.h"

/*
queries_t *q_array(unsigned length){
    queries_t* q = (queries_t*)malloc(sizeof(queries_t)); 
    q->length = length; 
    q->position=-1;
    q->items = (queries_t*)malloc(q->length * sizeof(token_array_t));
    return q; 
}

void q_close(queries_t * q){
    free(q->items);
}

void q_push(queries_t * q, token_array_t *item) { 
    if (q->position == q->length - 1)  {
        ghost(ERR_TOKENS_FULL);
        return; 
    }
    ++q->position;
    q->items[q->position] = *item; 
} 
  

*/

