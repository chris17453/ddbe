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
        
        // optional
        if(compare_token(tokens,0,TOKEN_DISTINCT)){
            token_add_type(tokens,TOKEN_SELECT,tokens->position-1);
        }
        // not optional
        if(expr_select_expr(tokens,depth)){
            token_add_type(tokens,TOKEN_SELECT,tokens->position-1);

        } else {
            tokens->position=pos;
            return 0;
        }

        
        if(expr_from(tokens,depth)){
            
            while(expr_join(tokens,depth)){
                //goop(depth,"join","GOT ONE");
            }


            if(expr_where(tokens,depth)){
            }
            if(expr_group_by(tokens,depth)){
            }
            if(expr_order_by(tokens,depth)){
            }
            if(expr_limit(tokens,depth)){
            }
        }
        select_debug();
        return 1;
    }
    
    tokens->position=pos;

    return 0;
}


