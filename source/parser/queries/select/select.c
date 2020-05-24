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
            goop(depth,"SELECT","DISTINCT");
        }
        // not optional
        if(expr_select_expr(tokens,depth)){
            goop(depth,"SELECT","FOUND select expression");
        } else {
            goop(depth,"UHOH","erms?");
            tokens->position=pos;
            return 0;
        }
        goop(depth,"SELECT","BEFORE FROM");

        tokens->target=*sel->from;

        if(expr_from(tokens,depth)){
            goop(depth,"SELECT","BEFORE WHERE");
            if(expr_where(tokens,depth)){
            }
            if(expr_limit(tokens,depth)){
            }
        }
        select_debug(sel);
        return 1;
    }
    
    tokens->position=pos;

    return 0;
}


