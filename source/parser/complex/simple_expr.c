#include "../../include/errors.h"
#include "../../include/tokens.h"

// simple_expr:
//    literal
//    | + simple_expr
//  | - simple_expr
//  | ~ simple_expr
//  | ! simple_expr


int expr_simple_expr(token_array_t* tokens,int depth){
    ++depth;
    int pos=tokens->position;
    #ifdef PARSE_ENTRANCE
    goop(depth,"SIMP-EXP","IN");
    #endif

    
    if(expr_identifier(tokens,depth)) return 1;
    
    if(expr_litteral(tokens,depth)) return 1;
    
/*    
    //if(expr_sub_query(tokens,depth)) return 1;
    if(compare_token(tokens,0,TOKEN_MINUS)) {
//        goop(depth,"MINUS","");
        if(expr_simple_expr(tokens,depth)) return 1;
        tokens->position=pos;
    }
    if(compare_token(tokens,0,TOKEN_PLUS)) {
//        goop(depth,"PLUS","");
        if(expr_simple_expr(tokens,depth)) return 1;
        tokens->position=pos;
    }
*/
    return 0;
}

