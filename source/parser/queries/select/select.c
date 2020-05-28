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


int match_pattern(token_array_t *tokens,int *pattern,int position){
    int length=&pattern[0];
    
    for(int p=0;p<length;p++){
        int new_pos=p+position;
        if (valid_token_index(tokens,new_pos)) {
            if (pattern[p]!=token_at(tokens,new_pos)->type){
                return 0;
            }
        } else {
            return 0;
        }
    }
    return 1;
}

void build_select(token_array_t *tokens,int start,int end){
    int limit_length=0;
    int limit_start=0;
    int loop=1;
    int i=start;
    int identity_2 []={3,TOKEN_ALPHA,TOKEN_LIST_DELIMITER,TOKEN_NUMERIC};
    int limit_range[]={3,TOKEN_NUMERIC,TOKEN_LIST_DELIMITER,TOKEN_NUMERIC};
    int limit[]      ={1,TOKEN_NUMERIC};
    
    
    select_t select;
    select.columns=0;
    select.from=0;
    select.where=0;
    select.order=0;
    select.group=0;

    // switch        
    switch(token_at(tokens,i)->type){
        case TOKEN_SELECT:   ++i; break;
        default:             printf("ERROR");
    }//end switch                

    // distinct
    switch(token_at(tokens,i)->type){
        case TOKEN_DISTINCT: select.distinct=1;         
                             ++i;
                             break;
        default:             printf("NO DISTINCT");
                             break;
    }//end switch                

  /*                                
                                 break;
            case TOKEN_WHERE:    break;
            case TOKEN_GROUP_BY: break;
            case TOKEN_ORDER_BY: break;
*/            

    // FROM
    switch(token_at(tokens,i)->type){
        case TOKEN_FROM:     ++i;
                            identifier_t *ident=safe_malloc(sizeof(identifier_t),1);
                            if(token_at(tokens,i)->type==TOKEN_QUALIFIER) {
                                ident->qualifier=token_at(tokens,i)->value;
                                i+=2;
                                ident->source=token_at(tokens,i)->value;
                            } else {
                                if(token_at(tokens,i)->type==TOKEN_SOURCE) {
                                ident->source=token_at(tokens,i)->value;
                                }
                            }
                            ++i;
                            select.from=ident;
                            break;

        default:    printf("NO FROM");
                    break;
    }// end switch
    


    loop=1;
    while(loop){
        switch(token_at(tokens,i)->type){
            case TOKEN_LIMIT_START: select.has_limit_start=1;
                                    select.limit_start=atoi(token_at(tokens,i)->value);
                                    i++;
                                    break;
            case TOKEN_LIMIT_LENGTH: select.has_limit_length=1;
                                     select.limit_length=atoi(token_at(tokens,i)->value);    
                                     i++;
                                     break;
            default: loop=0; break;
        }//end switch
    }


    
    // DEBUGGING INFORMATION

    if (select.distinct) printf("DISTINCT\n");
    if (select.from) {
        printf("FROM");
        if(select.from->qualifier) {
            printf("%s.",select.from->qualifier);
        }
        if(select.from->source) {
            printf("%s\n",select.from->source);
        }
        
    }
    if (select.has_limit_start) printf("LIMIT_START:   %d\n",select.limit_start);
    if (select.has_limit_length) printf("LIMIT_LENGTH : %d\n",select.limit_length);
    
}
