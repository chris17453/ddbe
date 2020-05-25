#include "../../../include/errors.h"
#include "../../../include/tokens.h"
#include "../../../include/structure.h"

// alpha or alpha.alpha
int expr_order_by(token_array_t *tokens,int depth,select_t* sel){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"order_by","IN");
    #endif
    int pos=tokens->position;
    
    if(compare_token(tokens,0,TOKEN_ORDER)){
        if(compare_token(tokens,0,TOKEN_BY)){
            goop(depth,"ORDER BY","START");

            int looper=1;
            int expr_list=0;
            int pos2=0;

            while(looper){
                pos2=tokens->position;
                if(!expr_identifier(tokens,depth,sel)){
                    //no select epression
                    break;
                }
                if(compare_token(tokens,0,TOKEN_ASC)) {
                
                } else 
                    if(compare_token(tokens,0,TOKEN_DESC)) {

                    }  else {
                        looper=0;
                        tokens->position=pos2;
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
                printf("%d order by columns",expr_list);
                return 1;
            }

        
        }
    } //end order by        
    tokens->position=pos;
    return 0;
}
