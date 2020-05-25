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
                if(!expr_identifier(tokens,depth)){
                    goop(depth,"ORDER-BY","NO IDENTIFIER");
                    //no select epression
                    break;
                }
                if(compare_token(tokens,0,TOKEN_ASC)) {
                    add_order_column(sel);
                    sel->order[sel->order_length-1].identity=&tokens->object;
                    sel->order[sel->order_length-1].direction=TOKEN_ASC;
                
                } else 
                    if(compare_token(tokens,0,TOKEN_DESC)) {
                        add_order_column(sel);
                        sel->order[sel->order_length-1].identity=&tokens->object;
                        sel->order[sel->order_length-1].direction=TOKEN_DESC;

                    }  else {
                        add_order_column(sel);
                        sel->order[sel->order_length-1].identity=&tokens->object;
                        sel->order[sel->order_length-1].direction=TOKEN_ASC;
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
