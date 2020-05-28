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
        }
        // not optional
        if(expr_select_expr(tokens,depth)){
        } else {
            tokens->position=pos;
            return 0;
        }

        
        if(expr_from(tokens,depth)){
            while(expr_join(tokens,depth)){
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
        token_add_type_range(tokens,TOKEN_SELECT,pos);
        build_select(tokens,pos,tokens->position);
        return 1;
    }
    
    tokens->position=pos;

    return 0;
}

token_t * token_at(token_array_t *tokens,int index){
    return &tokens->array[index];
}

void build_select(token_array_t *tokens,int start,int end){
    for(int i=start;i<end;i++) {
        token_t *t=token_at(tokens,i);
        if(t->type==TOKEN_SELECT) { printf("ITs a select\n");  }
        if(t->type==TOKEN_FROM) { printf("HAS FROM\n");  }
        if(t->type==TOKEN_WHERE) { printf("HAS WHERE\n");  }
        if(t->type==TOKEN_GROUP_BY) { printf("HAS GROUP_BY\n");  }
        if(t->type==TOKEN_ORDER_BY) { printf("HAS ORDER_BY\n");  }
        if(t->type==TOKEN_LIMIT) { printf("HAS LIMIT\n");  }
        

    }
    
}
