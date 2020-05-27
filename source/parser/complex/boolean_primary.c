#include "../../include/errors.h"
#include "../../include/tokens.h"

int expr_boolean_primary(token_array_t *tokens,int depth){
    ++depth;
    int pos=tokens->position;
    #ifdef PARSE_ENTRANCE
    goop(depth,"boolean_primary","in");
    #endif
    //predicate 
    if(expr_predicate(tokens,depth)){
        int pos2=tokens->position;

        // IS [NOT] NULL
        if(compare_token(tokens,0,TOKEN_IS))
            if(compare_token(tokens,1,TOKEN_NOT))
                if(compare_token(tokens,0,TOKEN_NULL))
                    token_add_type_range(tokens,TOKEN_BOOLEAN_PRIMARY,pos);

                    return 1;
        tokens->position=pos;

        // ok that didnt work out lets do the next test RESET
        // <=> predicate
        if(compare_token(tokens,0,TOKEN_NULL_EQ)){
             if(expr_predicate(tokens,depth)){
                    token_add_type_range(tokens,TOKEN_BOOLEAN_PRIMARY,pos);

                 return 1;
             }
        }
        tokens->position=pos2;


        //comparison_operator predicate
        if(expr_comparison_operator(tokens,depth)){
            if(expr_predicate(tokens,depth)){
                token_add_type_range(tokens,TOKEN_BOOLEAN_PRIMARY,pos);
                return 1;
            }

        }
        tokens->position=pos2;
        return 1;
    // comparison_operator {ALL | ANY} (subquery)
    }
    return 0;
}
