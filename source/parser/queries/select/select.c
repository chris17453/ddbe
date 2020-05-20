#include "../../../include/errors.h"
#include "../../../include/tokens.h"
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
        //tokens->object=sel;
        tokens->target=&sel->from;

        // optional
        if(compare_token(tokens,0,TOKEN_DISTINCT)){
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
        
        if(expr_from(tokens,depth)){
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

void select_debug(select_t *select){
    printf("SELECT_DEBUG\n");
    printf("  has_alias      =%d\n"   ,select->has_alias);
    printf("  has_columns    =%d\n"   ,select->has_columns);
    printf("  has_group      =%d\n"   ,select->has_group);
    printf("  has_order      =%d\n"   ,select->has_order);
    printf("  has_where      =%d\n"   ,select->has_where);
    printf("  alias          =%s\n"   ,select->alias);
    printf("  distinct       =%d\n"   ,select->distinct);
    printf("  from.qualifier =%s\n"   ,select->from.qualifier);
    printf("  from.source    =%s\n"   ,select->from.source);
    printf("  limit_length   =%d\n"   ,select->limit_length);
    printf("  limit_start    =%d\n"   ,select->limit_start);
}

select_t *select_new(){
    select_t *select=(select_t*)malloc(sizeof(select_t));
    select->alias="";
    select->distinct=-1;
    select->from.qualifier="";
    select->from.source="";
    select->has_alias=-1;
    select->has_columns=-1;
    select->has_group=-1;
    select->has_order=-1;
    select->has_where=-1;
    select->limit_length=-1;
    select->limit_start=-1;
    return select;
}



