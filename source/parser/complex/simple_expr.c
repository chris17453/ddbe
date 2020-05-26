#include "../../include/errors.h"
#include "../../include/tokens.h"

// simple_expr:
//    literal
//    | + simple_expr
//  | - simple_expr
//  | ~ simple_expr
//  | ! simple_expr

typedef struct simple_expr_t{
    int   sign;
    int   type;
    void *ptr;


} simple_expr_t;
/*
void add_simple_expr(token_array_t *tokens){
    simple_expr_t *simple_expr=safe_malloc(sizeof(simple_expr_t),1);
    simple_expr->sign=0;
    simple_expr->type=tokens->object_type;
    simple_expr->ptr=tokens->object;
    
    tokens->object=simple_expr;
    tokens->object_type=simple_expr;

}
*/


int expr_simple_expr(token_array_t* tokens,int depth){
    ++depth;
    int pos=tokens->position;
    #ifdef PARSE_ENTRANCE
    goop(depth,"SIMP-EXP","IN");
    #endif

    

    if(expr_identifier(tokens,depth)) {
       // add_simple_expr(tokens);
        return 1;
    }
    
    if(expr_litteral(tokens,depth)) {
       // add_simple_expr(tokens);
        return 1;
    }
    
    
    //if(expr_sub_query(tokens,depth)) return 1;
    if(compare_token(tokens,0,TOKEN_MINUS)) {
       if(expr_simple_expr(tokens,depth)) {
          // (simple_expr_t*)tokens->object.sign=TOKEN_MINUS;
           return 1;
       }
    }
    if(compare_token(tokens,0,TOKEN_PLUS)) {
        if(expr_simple_expr(tokens,depth)) {
          // (simple_expr_t*)tokens->object.sign=TOKEN_PLUS;
           return 1;
        }
        
    }
    
    tokens->position=pos;
    return 0;
}



