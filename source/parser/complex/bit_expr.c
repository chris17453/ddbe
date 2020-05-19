#include "../../include/errors.h"
#include "../../include/tokens.h"

// bit_expr:
//     bit_expr | bit_expr
//   | bit_expr & bit_expr
//   | bit_expr << bit_expr
//   | bit_expr >> bit_expr
//   | bit_expr + bit_expr
//   | bit_expr - bit_expr
//   | bit_expr * bit_expr
//   | bit_expr / bit_expr
//   | bit_expr DIV bit_expr
//   | bit_expr MOD bit_expr
//   | bit_expr % bit_expr
//#   | bit_expr ^ bit_expr  /?????
//#   | bit_expr + interval_expr
//#   | bit_expr - interval_expr
//   | simple_expr
int expr_bit_expr(token_array_t *tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"bit-expr","in");
    #endif

    if(expr_simple_expr(tokens,depth)){
        int pos=tokens->position;
        goop(depth,"bit-exp","got a simple");
        switch(tokens->array[tokens->position].type){
            case TOKEN_BIT_OR : break;
            case TOKEN_BIT_AND : break;
            case TOKEN_SHIFT_LEFT : break;
            case TOKEN_SHIFT_RIGHT : break; 
            case TOKEN_PLUS :  break;
            case TOKEN_MINUS : break;
            case TOKEN_MULTIPLY : break;
            case TOKEN_DIVIDE : break;
            case TOKEN_MODULUS : break;
            default: 
                goop(depth,"bit-exp",tokens->array[tokens->position].value);
                goop(depth,"bit-exp","No bit operation");
                return 1;
        }
        goop(depth,"bit-exp",tokens->array[tokens->position].value);
        goop(depth,"bit-exp","got a bit thing");

        ++tokens->position;
        if(expr_simple_expr(tokens,depth)) {
            goop(depth,"bit-exp","got a simple");
            return 1;
        }
        tokens->position=pos;
        return 1;
    }
    goop(depth,"bit-exp","got NOTHING");
        
    return 0;
}


