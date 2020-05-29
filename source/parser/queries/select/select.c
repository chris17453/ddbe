#include "../../../include/errors.h"
#include "../../../include/tokens.h"
#include "../../../include/structure.h"
#include "../../../include/select.h"

void build_select(token_array_t *tokens,int start,int end);
int select_free(select_t select) ;
void select_print(select_t select);

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

/*
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
*/

identifier_t * process_identifier(token_array_t *tokens,int *index){
    identifier_t *ident=0;
    switch(token_at(tokens,*index)->type) {
        case TOKEN_QUALIFIER:   ident=safe_malloc(sizeof(identifier_t),1);
                                ident->qualifier=token_at(tokens,*index)->value;
                                ident->source   =token_at(tokens,*index+1)->value;
                                *index+=2;
                                break;

        case TOKEN_SOURCE:         
                            ident=safe_malloc(sizeof(identifier_t),1);
                            ident->qualifier=0;
                            ident->source   =token_at(tokens,*index)->value;
                            ++*index;
                            break;
    }
    return ident;
}

char *process_alias(token_array_t *tokens,int *index){
    char *alias=0;
    switch(token_at(tokens,*index)->type) {
        case TOKEN_ALIAS: alias=token_at(tokens,*index)->value; 
                          ++*index; break;
    }

    return alias;
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
    select.has_limit_length=0;
    select.has_limit_start=0;
    select.limit_start=0;
    select.limit_length=0;
    select.column_length=0;
    select.where_length=0;
    select.order_length=0;
    select.group_length=0;


    // switch        
    switch(token_at(tokens,i)->type){
        case TOKEN_SELECT:   ++i; break;
    }//end switch                

    // distinct
    switch(token_at(tokens,i)->type){
        case TOKEN_DISTINCT: select.distinct=1;         
                             ++i;
                             break;
    }//end switch                


    // select list
    loop=1;
    int index=0;
    while(loop){
        data_column_t *dc;
        identifier_t *ident;
        switch(token_at(tokens,i)->type){
            // litterals
            case TOKEN_STRING:
            case TOKEN_NUMERIC:
            case TOKEN_HEX:
            case TOKEN_BINARY:
            case TOKEN_REAL:
            case TOKEN_NULL: add_data_column(&select);
                             dc=&select.columns[select.column_length-1];
                             dc->type=token_at(tokens,i)->type;
                             dc->ordinal=index;
                             dc->object=token_at(tokens,i)->value;
                             ++index;
                             ++i;
                             dc->alias=process_alias(tokens,&i);
                             break;



            case TOKEN_QUALIFIER:      
                                       ident=process_identifier(tokens,&i);
                                       add_data_column(&select);
                                       dc=&select.columns[select.column_length-1];
                                       dc->ordinal=index;
                                       dc->type=TOKEN_IDENTIFIER;
                                       dc->object=ident;
                                       ++index;
                                       dc->alias=process_alias(tokens,&i);
                                       break;

            case TOKEN_SOURCE:         
                                       ident=process_identifier(tokens,&i);
                                       add_data_column(&select);
                                       dc=&select.columns[select.column_length-1];
                                       dc->ordinal=index;
                                       dc->type=TOKEN_IDENTIFIER;
                                       dc->object=ident;
                                       ++index;
                                       dc->alias=process_alias(tokens,&i);
                                       break;
            default: loop=0;
        }
        switch(token_at(tokens,i)->type){
            case TOKEN_LIST_DELIMITER: ++i; 
                                       break;
            default: loop=0;
        }

        // end switch
    } // end while


    // from
    switch(token_at(tokens,i)->type){
        case TOKEN_FROM:     ++i;
                            select.from=process_identifier(tokens,&i);
                            select.alias=process_alias(tokens,&i);
                            break;
    }// end switch
    

    printf("join \n");
    // join
    loop=1;
    index=0;
    while(loop){
        join_t *join;
        switch(token_at(tokens,i)->type){
            case TOKEN_JOIN:
            case TOKEN_LEFT_JOIN:
            case TOKEN_RIGHT_JOIN:
            case TOKEN_FULL_OUTER_JOIN:
            case TOKEN_INNER_JOIN: 
                                        printf("PRE ADD\n");
                                        ++i;
                                        add_join(&select);
                                        join_t *join=&select.join[select.join_length-1];
                                        join->identifier=process_identifier(tokens,&i);
                                        join->alias=process_alias(tokens,&i);

                                        break;

            default: loop=0; 
                     break;
        }
    }


  /*                                
                                 break;
            case TOKEN_WHERE:    break;
            case TOKEN_GROUP_BY: break;
            case TOKEN_ORDER_BY: break;
*/            

    // limit
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


    
  select_print(select);
  select_free(select);
}

int select_free(select_t select) {
    // free resources
    for(int i=0;i<select.column_length;i++) {
        switch(select.columns[i].type){

            case TOKEN_STRING:
            case TOKEN_NUMERIC:
            case TOKEN_HEX:
            case TOKEN_BINARY:
            case TOKEN_REAL:
            case TOKEN_NULL: break;
            case TOKEN_IDENTIFIER: free(select.columns[i].object); break;
        }
//        if(select.columns->type)
  //      select.columns->object;
    }
    free(select.columns);
    if(select.from) free(select.from);
    for(int i=0;i<select.join_length;i++){
        if(select.join[i].identifier){
            free(select.join[i].identifier);
        }

    }
    free(select.join);
    return 0;
}

void select_print(select_t select){
    // DEBUGGING INFORMATION

    if (select.distinct) printf("DISTINCT\n");
    if(select.columns){

        for(int i=0;i<select.column_length;i++) {
            switch(select.columns[i].type){

                case TOKEN_STRING:
                case TOKEN_NUMERIC:
                case TOKEN_HEX:
                case TOKEN_BINARY:
                case TOKEN_REAL:
                case TOKEN_NULL:  printf("%s-%s ALIAS %s%d\n",
                                                        token_type(select.columns[i].type),
                                                        (char*)select.columns[i].object ,
                                                        select.columns[i].alias ,
                                                        select.columns[i].ordinal);
                                  break;
                case TOKEN_IDENTIFIER: 
                                  printf("%s- %s.%s ALIAS %s,%d\n",token_type(select.columns[i].type),
                                                            ((identifier_t *)select.columns[i].object)->qualifier ,
                                                            ((identifier_t *)select.columns[i].object)->source ,
                                                            select.columns[i].alias ,
                                                            select.columns[i].ordinal );
                                    break;
            }
        }
    }
   

     if (select.from) {
        printf("FROM\n");
        if(select.from->qualifier) {
            printf("%s.",select.from->qualifier);
        }
        if(select.from->source) {
            printf("%s",select.from->source);
            if(select.alias) printf(" ALIAS: %s ",select.alias);
            printf("\n");
        }
        
    }

    if (select.join) {
        printf("JOIN\n");
        for(int i=0;i<select.join_length;i++){
            if(select.join[i].identifier) {
                if(select.join[i].identifier->qualifier) {
                    printf("%s.",select.join[i].identifier->qualifier);
                }
                if(select.join[i].identifier->source) {
                    printf("%s ",select.join[i].identifier->source);
                }
                if(select.join[i].alias) printf("ALIAS: %s",select.join[i].alias);
                printf("\n");
            }
        }
        
    }

    if (select.has_limit_start) printf("LIMIT_START:   %d\n",select.limit_start);
    if (select.has_limit_length) printf("LIMIT_LENGTH : %d\n",select.limit_length);
}