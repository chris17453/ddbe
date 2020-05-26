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
    tokens->object_type=0;
    
    if(expr_simple_expr(tokens,depth)){
        int index=sel->column_length;
        
        add_data_column(sel);
        sel->columns[index].type=tokens->object_type;
        sel->columns[index].object=tokens->object;
        sel->columns[index].ordinal=index;
        sel->columns[index].alias=0;

        if(expr_alias(tokens,depth,&sel->columns[index].alias)){
           // goop(depth,"alias","found");
        }
        return 1;
    }
    tokens->position=pos;
    return 0;
}