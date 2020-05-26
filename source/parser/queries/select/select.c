#include "../../../include/errors.h"
#include "../../../include/tokens.h"
#include "../../../include/structure.h"
#include "../../../include/select.h"

int expr_select(token_array_t *tokens,int depth){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"SELECT","IN");
    #endif

    int pos=tokens->position;
    int number_of_expressions=0;
    if(compare_token(tokens,0,TOKEN_SELECT)){
        select_t *sel=select_new();
        tokens->object=sel;
        
        // optional
        if(compare_token(tokens,0,TOKEN_DISTINCT)){
            set_distinct(sel);
        }
        // not optional
        if(expr_select_expr(tokens,depth,sel)){
        } else {
            tokens->position=pos;
            return 0;
        }

        
        if(expr_from(tokens,depth,sel)){
            
            while(expr_join(tokens,depth,sel)){
                goop(depth,"join","GOT ONE");
            }


            if(expr_where(tokens,depth,sel)){
            }
            if(expr_group_by(tokens,depth,sel)){
            }
            if(expr_order_by(tokens,depth,sel)){
            }
            if(expr_limit(tokens,depth,sel)){
            }
        }
        select_debug(sel);
        return 1;
    }
    
    tokens->position=pos;

    return 0;
}


