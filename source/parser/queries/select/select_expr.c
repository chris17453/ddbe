#include "../../../include/errors.h"
#include "../../../include/tokens.h"
#include "../../../include/structure.h"

int expr_select_expr(token_array_t *tokens,int depth){
    ++depth;
    int pos=tokens->position;
    #ifdef PARSE_ENTRANCE
    goop(depth,"SEL-EXPR","IN");
    #endif

    int looper=1;

    int expr_list=0;

    while(looper){
        if(!expr_column_expr(tokens,depth,sel)) {
            //no select epression
            break;
        }
        ++expr_list;
        //print_token_range(tokens,"SELECT_EXPR SRC",pos2,tokens->position);

        // if we have a delimiter.. loop else exit
        if(!compare_token(tokens,0,TOKEN_LIST_DELIMITER)) {
            looper=0;
        } else {
        }
    } //end looper
    if(expr_list>0) {
        token_add_type_range(tokens,TOKEN_SELECT_EXPR,pos);

        return 1;
    }
    return 0;
}
    