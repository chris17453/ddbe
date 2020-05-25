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
        select_add_column(sel);
        sel->columns[sel->column_length].type=1;
        sel->columns[sel->column_length].ordinal=sel->column_length;
        sel->columns[sel->column_length].alias="BOBO";
        if(expr_alias(tokens,depth,sel->columns[sel->column_length].alias)){
            goop(depth,"alias","found");
        }
        ++sel->column_length;
        return 1;
    }
    tokens->position=pos;
    return 0;
}