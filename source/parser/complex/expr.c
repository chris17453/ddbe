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
int expr_expr(token_array_t* tokens,int depth,select_t *sel){
    ++depth;
    int pos=tokens->position;
    #ifdef PARSE_ENTRANCE
    goop(depth,"expr","..\n\n");
    #endif
    // not expr
    int res=compare_token(tokens,0,TOKEN_NOT);
    if (res) {
        if(expr_core(tokens,depth)){
            add_where_expr(sel);
            where_expr_t *where=&sel->where[sel->where_length-1];
            where->NOT=1;
            where->ordinal=sel->where_length-1;
            where->length=tokens->position-pos-1;
            where->tokens=&tokens->array[pos+1];

            return 1;
        } else {
            return 0;
        }
    }
    tokens->position=pos;

    //expr
    if(expr_core(tokens,depth)) {
        pos=tokens->position;
        //expr [and/or] expr
        switch(tokens->array[pos].type){
            case TOKEN_SHORT_AND : break;
            case TOKEN_SHORT_OR  : break;
            //case TOKEN_XOR       : break;
            case TOKEN_AND       : break;
            case TOKEN_OR        : break;
            default: return 1; //already passed core.. PEACE OUT
        }
        goop(depth,"expression binder",tokens->array[pos].value);

        //Tru for 2nd expr_core... but if not.. reset position and PEACE OUT!
        //if(!expr_core(tokens,depth)){
        add_where_expr(sel);
        where_expr_t *where=&sel->where[sel->where_length-1];
        where->comparitor=tokens->array[tokens->position].type;
        where->ordinal=sel->where_length-1;
        where->length=tokens->position-pos;
        where->tokens=&tokens->array[pos+1];

        ++tokens->position;

        if(!expr_expr(tokens,depth,sel)){
            tokens->position=pos;
        }
        goop(depth,"expr SUCCESS","word up... good job sir.");

        return 1;
    }
    goop(depth,"expression FEIL","--");
    return 0;
}
