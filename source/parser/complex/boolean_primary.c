#include "../../include/errors.h"
#include "../../include/tokens.h"

int expr_boolean_primary(token_array_t *tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"boolean_primary","in");
    #endif
    //predicate 
    if(expr_predicate(tokens,depth)){
        int pos=tokens->position;

        // IS [NOT] NULL
        if(compare_token(tokens,0,TOKEN_IS))
            if(compare_token(tokens,1,TOKEN_NOT))
                if(compare_token(tokens,0,TOKEN_NULL))
                    return 1;
        tokens->position=pos;

        // ok that didnt work out lets do the next test RESET
        // <=> predicate
        if(compare_token(tokens,0,TOKEN_NULL_EQ)){
            return expr_predicate(tokens,depth);
        }
        tokens->position=pos;


        //comparison_operator predicate
        if(expr_comparison_operator(tokens,depth)){
            goop(depth,"boolean primary","--->");
            if(expr_predicate(tokens,depth)){
                goop(depth,"boolean primary","<-----Got a comparitor");
                return 1;
            }

        }
        tokens->position=pos;
        return 1;
    // comparison_operator {ALL | ANY} (subquery)
    }
    return 0;
}
