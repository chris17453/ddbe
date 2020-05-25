#include "../../../include/errors.h"
#include "../../../include/tokens.h"
#include "../../../include/structure.h"

//identifier or identifier as alias
int expr_column_expr(token_array_t *tokens,int depth,select_t *sel){
    ++depth;

    #ifdef PARSE_ENTRANCE
    goop(depth,"COLEXPR","IN");
    #endif

    int pos=tokens->position;
    if(expr_simple_expr(tokens,depth)){
        int index=sel->column_length;
        select_add_column(sel);
        sel->columns[index].type=1;
        sel->columns[index].ordinal=index;
        
        sel->columns[index].alias=0;
        sel->columns[index].alias="F";
        if(expr_alias(tokens,depth,sel->columns[index].alias)){ //sel->columns[sel->column_length].alias
            //sel->columns[index].alias="BOB";
            goop(depth,"alias","found");
        }
        return 1;
    }
    tokens->position=pos;
    return 0;
}