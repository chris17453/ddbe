#include "../../include/errors.h"
#include "../../include/tokens.h"

int expr_expr_list(token_array_t *tokens,int depth){
    #ifdef PARSE_ENTRANCE
    goop(depth,"expr-list","IN");
    #endif

    if(compare_token(tokens,0,TOKEN_PAREN_LEFT)){
        int expr_list=0;
        int looper=1;
        while(looper){
            if(!expr_core(tokens,depth)) {
                //no select epression
                break;
            }
            ++expr_list;
            // if we have a delimiter.. loop else exit
            if(!compare_token(tokens,0,TOKEN_LIST_DELIMITER)) {
                looper=0;
            } else {
            }
        } //end looper

        if(expr_list>0) {
            if(compare_token(tokens,0,TOKEN_PAREN_RIGHT)){
                return 1;
            }
        }         
    }
    return 0;
}
