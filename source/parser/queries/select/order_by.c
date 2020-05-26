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
    
    if(compare_token(tokens,0,TOKEN_ORDER_BY)){
            int looper=1;
            int expr_list=0;
            
            tokens->object=0;
            while(looper){
                if(!expr_identifier(tokens,depth)){
                    //no select epression
                    break;
                }
                if(compare_token(tokens,0,TOKEN_ASC)) {
                    add_order_column(sel);
                    sel->order[sel->order_length-1].identity=tokens->object;
                    sel->order[sel->order_length-1].direction=TOKEN_ASC;
                    sel->order[sel->order_length-1].ordinal=expr_list;
                
                } else 
                    if(compare_token(tokens,0,TOKEN_DESC)) {
                        add_order_column(sel);
                        sel->order[sel->order_length-1].identity=tokens->object;
                        sel->order[sel->order_length-1].direction=TOKEN_DESC;
                        sel->order[sel->order_length-1].ordinal=expr_list;

                    }  else {
                        add_order_column(sel);
                        sel->order[sel->order_length-1].identity=tokens->object;
                        sel->order[sel->order_length-1].direction=TOKEN_ASC;
                        sel->order[sel->order_length-1].ordinal=expr_list;
                    }


                ++expr_list;


                // if we have a delimiter.. loop else exit
                if(!compare_token(tokens,0,TOKEN_LIST_DELIMITER)) {
                    looper=0;
                } else {
                }
            } //end looper
            
            if(expr_list>0) {
                return 1;
            }
    } //end order by        
    tokens->position=pos;
    return 0;
}
