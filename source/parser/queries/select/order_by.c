#include "../../../include/errors.h"
#include "../../../include/tokens.h"
#include "../../../include/structure.h"

// alpha or alpha.alpha
int expr_order_by(token_array_t *tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"order_by","IN");
    #endif
    int pos=tokens->position;
    
    if(compare_token(tokens,0,TOKEN_ORDER_BY)){
            int looper=1;
            int expr_list=0;
            
            while(looper){
                if(!expr_identifier(tokens,depth)){
                    //no select epression
                    break;
                }
                if(compare_token(tokens,0,TOKEN_ASC)) {
                } else 
                    if(compare_token(tokens,0,TOKEN_DESC)) {
                    }  else {
                    }


                ++expr_list;


                // if we have a delimiter.. loop else exit
                if(!compare_token(tokens,0,TOKEN_LIST_DELIMITER)) {
                    looper=0;
                } else {
                }
            } //end looper
            
            if(expr_list>0) {
                token_add_type_range(tokens,TOKEN_ORDER_BY,pos);
                return 1;
            }
    } //end order by        
    tokens->position=pos;
    return 0;
}
