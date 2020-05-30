#include "../../include/errors.h"
#include "../../include/tokens.h"
#include "../../include/structure.h"

//    boolean_primary IS [NOT] {TRUE | FALSE | UNKNOWN}
//  | boolean_primary
int expr_core(token_array_t *tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"expr_core","in");
    #endif
    if(expr_boolean_primary(tokens,depth)){
        int pos=tokens->position;
        // IS [NOT] {TRUE | FALSE | UNKNOWN}
        if(compare_token(tokens,0,TOKEN_IS)){
            if(compare_token(tokens,0,TOKEN_NOT)){
            }
            
            if (expr_bool(tokens,depth)) {
                return 1;
            }
        }
        // well that is not true blah didnt work out.. reset position and PEACE OUT
        tokens->position=pos;
        return 1;
    }
    // its not this.. peace out!
    return 0;
}


// EXPRESSION:
//     expr OR expr
//   | expr || expr
//   | expr XOR expr
//   | expr AND expr
//   | expr && expr
//   | NOT expr
//   | ! expr
//   | boolean_primary IS [NOT] {TRUE | FALSE | UNKNOWN}
//   | boolean_primary
int expr_expr(token_array_t* tokens,int depth){
    ++depth;
    int pos=tokens->position;
    #ifdef PARSE_ENTRANCE
    goop(depth,"expr","..\n\n");
    #endif
    // not expr
    int is_not=compare_token(tokens,0,TOKEN_NOT);


    //expr
    int pos2=tokens->position;
    if(expr_core(tokens,depth)) {
        int pos3=tokens->position;
        //expr [and/or] expr


        switch(tokens->array[tokens->position].type){
            case TOKEN_SHORT_AND :
            case TOKEN_SHORT_OR  :
            //case TOKEN_XOR       : break;
            case TOKEN_AND       : 
            case TOKEN_OR        :  break;
            default: 
                     token_add_type_range(tokens,TOKEN_EXPR,pos);
                     return 1; //already passed core.. PEACE OUT
        }
        //goop(depth,"expression binder",tokens->array[tokens->position].value);

        //Tru for 2nd expr_core... but if not.. reset position and PEACE OUT!
        //if(!expr_core(tokens,depth)){

        ++tokens->position;

        if(!expr_expr(tokens,depth)){
            tokens->position=pos3;
        }
        token_add_type_range(tokens,TOKEN_EXPR,pos3);

        //goop(depth,"expr SUCCESS","word up... good job sir.");

        return 1;
    } 
    tokens->position=pos;
    return 0;
}
