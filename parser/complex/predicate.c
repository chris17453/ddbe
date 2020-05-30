#include "../../include/errors.h"
#include "../../include/tokens.h"

// predicate:
//#     bit_expr [NOT] IN (subquery)
//#   | bit_expr [NOT] IN (expr [, expr] ...)
//#   | bit_expr [NOT] BETWEEN bit_expr AND predicate
//#   | bit_expr [NOT] LIKE simple_expr [ESCAPE simple_expr]
//#   | bit_expr [NOT] REGEXP bit_expr
//   | bit_expr
int expr_predicate(token_array_t *tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"predicate","in");
    #endif

    if(expr_bit_expr(tokens,depth)){
        
        int pos=tokens->position;
/*        // optional
        goop(depth,"predicate",tokens->array[tokens->position].value);

        if(compare_token(tokens,0,TOKEN_NOT)){

        }
        if(compare_token(tokens,0,TOKEN_IN)){
        }//end token in
*/

        //#   | bit_expr [NOT] IN (expr [, expr] ...)
        tokens->position=pos;

        return 1;
    }
    goop(depth,"predicate","NOTHING");
        
    return 0;
}

