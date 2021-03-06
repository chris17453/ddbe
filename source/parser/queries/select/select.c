#include "../../../include/errors.h"
#include "../../../include/tokens.h"
#include "../../../include/structure.h"
#include "../../../include/select.h"

#define EXPRESSION_GROUP_BY 3
#define EXPRESSION_ORDER_BY 4
#define EXPRESSION_COLUMN   5


expression_t * process_column_list(token_array_t *tokens,int *index);
int free_table_def(table_def_t *table_def);



/* Function: duplicate_token
 * -----------------------
 * returns a copy of a token_t from an token_array_t
 * 
 * returns: token_t if the token and index are valid
 *          returns zero (NULL) otherwise
 */
token_t * duplicate_token(token_array_t *tokens,int index){
    token_t *src=token_at(tokens,index);
    // if its a bad token.. dont duplicate
    if(src==0) return 0;
    token_t *dst=safe_malloc(sizeof(token_t),1); 
    dst->depth  =src->depth;
    dst->type   =src->type;
    dst->value  =copy_token_value_at(tokens,index);
    for(int i=0;i<TOKEN_MAX_DEPTH;i++) dst->expr[i]=src->expr[i];
    return dst;
}

/* Function: token_at
 * -----------------------------
 * return the token at a given index within a token_array_t
 * 
 * returns: a token if the token index is valid
 *          and the token is not 0 (NULL)
 *          returns zero (NULL) otherwise
 */
token_t * token_at(token_array_t *tokens,int index){
    if(valid_token_index(tokens,index)){
        return &tokens->array[index];
    }
    return 0;
}// end func

/* Function: copy_token_value_at
 * -----------------------------
 * clone a token value
 * 
 * returns: cloned token value if the token index is valid
 *          and the value is non 0 (NULL)
 *          returns zero (NULL) otherwise
 */
char * copy_token_value_at(token_array_t *tokens,int index){
    if(valid_token_index(tokens,index)){
        char *value=tokens->array[index].value;
        if(value) return strdup(value);
    }
    printf("ERROR: COPYING INVALID POSITION %d\n",index);
    return 0;
} // end func

/* Function: add_expr
 * -----------------------------
 * append an expression to another expression
 * 
 * returns: 1 if successfull 
 *          returns zero (NULL) otherwise
 */
int add_expr(expression_t *expression,expression_t *item){
    if(item==0) return 0;

    if(expression->expression==0) {
        expression->expression=item;
        expression->expression_tail=item;
    } else {
        expression->expression_tail->expression=item;
        expression->expression_tail=item;
    }
    return 1;
}

/* Function: process_alias
 * -----------------------
 * returns a alias
 * 
 * returns: cloned token value if:
 *               TOKEN_ALIAS
 *          is matched
 *          index pointer is incremented +1 on match
 *          returns zero (NULL) otherwise
 */
char * process_alias(token_array_t *tokens,int *index){
    char *alias=0;
    switch(token_at(tokens,*index)->type) {
        case TOKEN_ALIAS: alias=copy_token_value_at(tokens,*index); 
                          ++*index; break;
    }
    return alias;
}// end func

/* Function: process_identifier
 * -----------------------
 * returns an identifier_t struct
 * 
 * returns: identifier_t if:
 *            TOKEN_QUALIFIER  or  
 *            TOKEN_SOURCE,TOKEN_QUALIFIER
 *          match is found
 *          index pointer is incremented +n on match
 *          returns zero (NULL) otherwise
 */
identifier_t * process_identifier(token_array_t *tokens,int *index){
    identifier_t *ident=0;
    switch(token_at(tokens,*index)->type) {
        case TOKEN_QUALIFIER:   ident=safe_malloc(sizeof(identifier_t),1);
                                ident->qualifier=copy_token_value_at(tokens,*index);
                                ident->source   =copy_token_value_at(tokens,*index+1);
                                *index+=2;
                                break;

        case TOKEN_SOURCE:         
                            ident=safe_malloc(sizeof(identifier_t),1);
                            ident->qualifier=0;
                            ident->source   =copy_token_value_at(tokens,*index);
                            ++*index;
                            break;
    }
    return ident;
} // end func

/* Function: process_litteral
 * -----------------------
 * returns an litteral token match
 * 
 * returns: token_t if :
 *            TOKEN_NULL, TOKEN_HEX, TOKEN_BINARY
 *            TOKEN_STRING, TOKEN_NUMERIC, TOKEN_REAL
 *          match is found
 *          index pointer is incremented +n on match
 *          returns zero (NULL) otherwise
 */
token_t * process_litteral(token_array_t *tokens,int *index){
    token_t *token=token_at(tokens,*index);
    token_t *temp_token=0;
    switch(token->type) {
        case TOKEN_NULL   :
        case TOKEN_HEX    :
        case TOKEN_BINARY :
        case TOKEN_STRING :
        case TOKEN_NUMERIC:
        case TOKEN_REAL   : temp_token=duplicate_token(tokens,*index); 
                            if (temp_token) ++*index;
                            return temp_token;
    }
    return 0;
} // end func

/* Function: process_simple_expr
 * -----------------------
 * returns an litteral or identity type with flags
 * 
 * returns: expression_t if :
 *            litteral
 *            identifier_t 
 *          match is found
 *          flags for preapended -+ set
 *          index pointer is incremented +n on match
 *          returns zero (NULL) otherwise
 */
expression_t * process_simple_expr(token_array_t *tokens,int *index){
    expression_t *expr=0;
    int mode=0;

    switch(token_at(tokens,*index)->type) {
            case TOKEN_MINUS: mode=-1; ++*index; break;
            case TOKEN_PLUS : mode= 1; ++*index; break;
    }

    token_t *litteral=process_litteral(tokens,index);
    if(litteral) {
        expr=safe_malloc(sizeof(expression_t),1);
        if (mode== 1) expr->positive=1;
        if (mode==-1) expr->negative=1;
        expr->literal=litteral;
        expr->mode=2;
    } else {
        identifier_t *ident=process_identifier(tokens,index);
        if(ident) {
            expr=safe_malloc(sizeof(expression_t),1);
            if (mode== 1) expr->positive=1;
            if (mode==-1) expr->negative=1;
            expr->mode=1;
            expr->identifier=ident;
        }
    }

   return expr;
} // end func

/* Function: process_bit_expr
 * -----------------------
 * match a nested simple expressions with 
 *          bit operation glue or pass a 
 *          simle expression
 * 
 * returns: nested expression_t if a  matched
 *          index pointer is incremented +n on match
 *          returns zero (NULL) otherwise
 */
expression_t * process_bit_expr(token_array_t *tokens,int *index){
    expression_t *expr=0;
    expression_t *temp_expr=0;

    expr=process_simple_expr(tokens,index);
    
    if(expr){
        int loop=1;
        while(loop) {
            int operator=token_at(tokens,*index)->type;
            switch(operator) {
                case TOKEN_BIT_OR : 
                case TOKEN_BIT_AND : 
                case TOKEN_SHIFT_LEFT :
                case TOKEN_SHIFT_RIGHT :
                case TOKEN_PLUS : 
                case TOKEN_MINUS : 
                case TOKEN_MULTIPLY :
                case TOKEN_DIVIDE : 
                case TOKEN_MODULUS :  ++*index;
                                    expression_t *expr2=process_simple_expr(tokens,index);
                                    //debug_expr(expr2,10);
                                    if(expr2) expr2->operator=operator;
                                    if(!add_expr(expr,expr2)){
                                        --*index;
                                        loop=0;
                                    } 
                                    break;
                default: loop=0; break;

            }
        }
    }
    return expr;
} // end func

/* Function: process_expr_list
 * -----------------------
 * match a list of simple_expressions
 * 
 * Ex: (x,y,1,'bob',0xFF,0b0101)
 * 
 * returns: nested list of expression_t if matched
 *          index pointer is incremented +n on match
 *          returns zero (NULL) otherwise
 */
expression_t * process_expr_list(token_array_t *tokens,int *index){
    expression_t *expr=0;
    int start_point=*index;

    switch(token_at(tokens,*index)->type) {
        case TOKEN_PAREN_LEFT: ++*index;
        default: return 0;
    }

    int loop=1;
    expression_t *temp_expr=0;    
    while(loop){
        
        temp_expr=process_simple_expr(tokens,index);
        if(temp_expr==0) {
            break;
        }
        
        temp_expr->list=1;
        add_expr(expr,temp_expr);
        

        switch(token_at(tokens,*index)->type) {
            case TOKEN_LIST_DELIMITER: ++*index;
            default: loop=0;
        }
    }

    switch(token_at(tokens,*index)->type) {
        case TOKEN_PAREN_RIGHT: ++*index;
        default: *index=start_point; free_expression(expr); return 0;
    }
    
    return expr;
} // end func

/* Function: process_predicate
 * -----------------------
 * match an bit_expression with a "list" or pass 
 *          an bit_expression
 * 
 * returns: nested list of expression_t if matched
 *          index pointer is incremented +n on match
 *          returns zero (NULL) otherwise
 */
expression_t * process_predicate(token_array_t *tokens,int *index){
    expression_t *expr=0;

    expr=process_bit_expr(tokens,index);

   /*
    if(expr){
        int mode=0;
        switch(token_at(tokens,*index)->type) {
            case TOKEN_IN     : mode=1;  
            case TOKEN_NOT_IN : mode=-1; 
                                ++*index;
                                  if(add_expr(expr,process_expr_list(tokens,index))){
                                      if(mode== 1) expr->not_in=1;
                                      else if(mode==-1) expr->in=1;
                                  } else {
                                      --*index;
                                  }
                                  break;
        }
    }*/
    return expr;
} // end func

/* Function: process_boolean_primary
 * -----------------------
 * match a nested predicate with comparitor glue
 *          or pass an predicate
 * 
 * returns: nested list of expression_t if matched
 *          index pointer is incremented +n on match
 *          returns zero (NULL) otherwise
 */
expression_t * process_boolean_primary(token_array_t *tokens,int *index){
    expression_t *expr=0;
    expr=process_predicate(tokens,index);
    if(expr){
        int token=token_at(tokens,*index)->type;
        switch(token) {
            case TOKEN_IS_NOT_NULL:
            case TOKEN_IS_NULL    : ++*index; expr->comparitor=token; break;
            
            case TOKEN_NULL_EQ    : 
            case TOKEN_LESS_EQ    :
            case TOKEN_GREATER_EQ :
            case TOKEN_LESS       :
            case TOKEN_GREATER    :
            case TOKEN_NOT_EQ     :
            case TOKEN_ASSIGNMENT : ++*index;
                                    expression_t *expr2=process_predicate(tokens,index);
                                    //debug_expr(expr2,10);
                                    if(expr2) expr2->comparitor=token; 
                                    if(add_expr(expr,expr2)){

                                    } else { 
                                        printf("WARNING %d %s %s\n",*index,token_type(token_at(tokens,*index)->type),token_at(tokens,*index)->value);
                                        --*index;
                                    }
                                    break;
        }
    }
    return expr;
} // end func

/* Function: process_expression
 * -----------------------
 * match a nested expression at the TOP MOST LEVEL
 * 
 * returns: nested list of expression_t if matched
 *          index pointer is incremented +n on match
 *          returns zero (NULL) otherwise
 */
expression_t * process_expression(token_array_t *tokens,int *index){
    expression_t *expr=0;
    // NOT
    
    
    int not=0;
    switch(token_at(tokens,*index)->type) {
            case TOKEN_NOT : ++*index; not=1; break;
    }

    expr=process_boolean_primary(tokens,index);
    if(expr) {
        expr->not=not;
        int token=token_at(tokens,*index)->type;
        switch(token) {
            case TOKEN_SHORT_AND :
            case TOKEN_SHORT_OR  :
            case TOKEN_AND       : 
            case TOKEN_OR        : ++*index;
                                if(add_expr(expr,process_expression(tokens,index))){
                                    expr->comparitor=token;
                                } else {
                                    --*index;
                                }
                                break;
        } //end switch
    } //end if

    return expr;
} //end func

/* Function: process_group_column_list
 * -----------------------------
 * gather a group by column list
 * 
 * returns: a nested linked list of 
 *          expression_t if matched
 *          returns zero (NULL) otherwise
 */
expression_t * process_group_column_list(token_array_t *tokens,int *index){
    expression_t *expr=0;
    expression_t *expr2=0;
    identifier_t *ident=0;
    int loop=1;
    while(loop) {
        ident=process_identifier(tokens,index);
        
        if(ident) {
            expr2=safe_malloc(sizeof(expression_t),1); 
            expr2->identifier=ident;
            expr2->mode=EXPRESSION_GROUP_BY;
            if(expr==0) {
                expr=expr2;
            } 
            else {
                add_expr(expr,expr2);
            }

            if(token_at(tokens,*index)->type!=TOKEN_LIST_DELIMITER) {
                loop=0;
            } else {
                ++*index;
            }
        } else {
            loop=0;
        }
    }
    return expr;
}

/* Function: process_order_column_list
 * -----------------------------
 * gather a order by column list
 * 
 * returns: a nested linked list of 
 *          expression_t if matched
 *          returns zero (NULL) otherwise
 */
expression_t * process_order_column_list(token_array_t *tokens,int *index){
    expression_t *expr=0;
    expression_t *expr2=0;
    identifier_t *ident=0;
    int loop=1;
    printf ("ORDER BY\n");
    while(loop) {
        ident=process_identifier(tokens,index);
        if(ident) {
            printf("Ident column");
            int token=token_at(tokens,*index)->type;
            switch(token){
                case TOKEN_ASC:  
                case TOKEN_DESC: ++*index; 
                                expr2=safe_malloc(sizeof(expression_t),1); 
                                expr2->direction=token;
                                expr2->identifier=ident;
                                expr2->mode=EXPRESSION_ORDER_BY;
                                if(expr==0) {
                                    expr=expr2;
                                    printf("replacing EXP\n");
                                } 
                                else {
                                    add_expr(expr,expr2);
                                    printf("adding to EXP\n");
                                }
                                break;
                default: expr2=safe_malloc(sizeof(expression_t),1); 
                                expr2->direction=TOKEN_ASC;
                                expr2->identifier=ident;
                                expr2->mode=EXPRESSION_ORDER_BY;
                                if(expr==0) {
                                    expr=expr2;
                                    printf("replacing EXP\n");
                                } 
                                else {
                                    add_expr(expr,expr2);
                                    printf("adding to EXP\n");
                                }
                                break;
            }//end switch            
            if(token_at(tokens,*index)->type!=TOKEN_LIST_DELIMITER) {
                loop=0;
            } else {
                ++*index;
            }
        } else {
            loop=0;
        }
    }
    return expr;
}

/* Function: process_select
 * -----------------------
 * process
 * 
 * returns: nothing. All output is via stdio
 */
select_t * process_select(token_array_t *tokens,int *start){
    //if(*start>=tokens->top) return;
    int limit_length=0;
    int limit_start=0;
    int loop=1;
    
    
    select_t *select=safe_malloc(sizeof(select_t),1);
    select->distinct=0;
    select->columns=0;
    select->from=0;
    select->join=0;
    select->where=0;
    select->order=0;
    select->group=0;
    select->has_limit_length=0;
    select->has_limit_start=0;
    select->limit_start=0;
    select->limit_length=0;
    select->column_length=0;
    select->join_length=0;
    

    // switch        
    switch(token_at(tokens,*start)->type){
        case TOKEN_SELECT:   ++*start; break;
    }//end switch                

    // distinct
    switch(token_at(tokens,*start)->type){
        case TOKEN_DISTINCT: select->distinct=1;         
                             ++*start;
                             break;
    }//end switch                


    // select list
    loop=1;
    int index=0;
    while(loop){
        data_column_t *dc;
        identifier_t *ident;
        switch(token_at(tokens,*start)->type){
            // litterals
            case TOKEN_STRING:
            case TOKEN_NUMERIC:
            case TOKEN_HEX:
            case TOKEN_BINARY:
            case TOKEN_REAL:
            case TOKEN_NULL: add_data_column(select);
                             dc=&select->columns[select->column_length-1];
                             dc->type=token_at(tokens,*start)->type;
                             dc->ordinal=index;
                             dc->object=token_at(tokens,*start)->value;
                             ++index;
                             ++*start;
                             dc->alias=process_alias(tokens,start);
                             break;



            case TOKEN_QUALIFIER:      
                                       ident=process_identifier(tokens,start);
                                       add_data_column(select);
                                       dc=&select->columns[select->column_length-1];
                                       dc->ordinal=index;
                                       dc->type=TOKEN_IDENTIFIER;
                                       dc->object=ident;
                                       ++index;
                                       dc->alias=process_alias(tokens,start);
                                       break;

            case TOKEN_SOURCE:         
                                       ident=process_identifier(tokens,start);
                                       add_data_column(select);
                                       dc=&select->columns[select->column_length-1];
                                       dc->ordinal=index;
                                       dc->type=TOKEN_IDENTIFIER;
                                       dc->object=ident;
                                       ++index;
                                       dc->alias=process_alias(tokens,start);
                                       break;
            default: loop=0;
        }
        switch(token_at(tokens,*start)->type){
            case TOKEN_LIST_DELIMITER: ++*start; 
                                       break;
            default: loop=0;
        }

        // end switch
    } // end while



    // from
    switch(token_at(tokens,*start)->type){
        case TOKEN_FROM:     ++*start;
                            select->from=process_identifier(tokens,start);
                            select->alias=process_alias(tokens,start);
                            break;
    }// end switch
    
    
    // join
    loop=1;
    index=0;
    while(loop){
        join_t *join;
        switch(token_at(tokens,*start)->type){
            case TOKEN_JOIN:
            case TOKEN_LEFT_JOIN:
            case TOKEN_RIGHT_JOIN:
            case TOKEN_FULL_OUTER_JOIN:
            case TOKEN_INNER_JOIN: 
                                        ++*start;
                                        add_join(select);
                                        join_t *join=&select->join[select->join_length-1];
                                        join->identifier=process_identifier(tokens,start);
                                        join->alias=process_alias(tokens,start);
                                        switch(token_at(tokens,*start)->type){
                                            case TOKEN_ON: ++*start; 
                                                           join->expression=process_expression(tokens,start);
                                                           break;
                                        }//end switch                

                                        
                                        

                                        break;

            default: loop=0; 
                     break;
        }
    }


    // where
    loop=1;
    index=0;
    while(loop){
        switch(token_at(tokens,*start)->type){
            case TOKEN_WHERE: ++*start;
                        select->where=process_expression(tokens,start);
                        break;
            default: loop=0; 
                     break;
        }
    }

    switch(token_at(tokens,*start)->type){
        case TOKEN_GROUP_BY: ++*start; 
                                select->group=process_group_column_list(tokens,start); 
                                break;
    }

    switch(token_at(tokens,*start)->type){
        case TOKEN_ORDER_BY: ++*start; 
                                select->order=process_order_column_list(tokens,start); 
                                break;
    }

    // limit
    loop=1;
    while(loop){
        switch(token_at(tokens,*start)->type){
            case TOKEN_LIMIT_START: select->has_limit_start=1;
                                    select->limit_start=atoi(token_at(tokens,*start)->value);
                                    ++*start;
                                    break;
            case TOKEN_LIMIT_LENGTH: select->has_limit_length=1;
                                     select->limit_length=atoi(token_at(tokens,*start)->value);    
                                     ++*start;
                                     break;
            default: loop=0; break;
        }//end switch
    }

    
  select_print(select);
  return select;
}

/* Function: select_free
 * -----------------------------
 * free the data structure of a select_t
 * 
 * returns: 1 for success or 0 (NULL) for failure
 */
int free_select(select_t *select) {
    // free resources
    for(int i=0;i<select->column_length;i++) {
        switch(select->columns[i].type){

            case TOKEN_STRING:
            case TOKEN_NUMERIC:
            case TOKEN_HEX:
            case TOKEN_BINARY:
            case TOKEN_REAL:
            case TOKEN_NULL: break;
            case TOKEN_IDENTIFIER: free_ident(select->columns[i].object); 
            break;
        }
        free_string(select->columns[i].alias);
//        if(select.columns->type)
  //      select.columns->object;
    }
    free(select->columns);
    if(select->from) {
        free_ident(select->from);
        free_string(select->alias);
    }
    if(select->join) {
        for(int i=0;i<select->join_length;i++) {
            free_ident(select->join[i].identifier);
            free_expression(select->join[i].expression);
            free_string(select->join[i].alias);
        }
        free(select->join);
    }
    free_expression(select->where);
    free_expression(select->group);
    free_expression(select->order);
    free(select);
    return 0;
}

/* Function: free_string
 * -----------------------------
 * free the data structure of a char*
 * 
 * returns: 1 for success or 0 (NULL) for failure
 */
int free_string(char *data){
    if(data) free(data);
 return 1;
}

/* Function: free_expression
 * -----------------------------
 * free the data structure of a expression_t
 * 
 * returns: 1 for success or 0 (NULL) for failure
 */
int free_expression(expression_t *expr){
 expression_t *expr_ptr=expr;
 while(expr_ptr){
     if(expr_ptr->literal) free_litteral(expr_ptr->literal);
     if(expr_ptr->identifier) free_ident(expr_ptr->identifier);
     expression_t * last=expr_ptr;
     expr_ptr=expr_ptr->expression;
     free(last);
 }
 return 1;
}

/* Function: free_ident
 * -----------------------------
 * free the data structure of a identifier_t
 * 
 * returns: 1 for success or 0 (NULL) for failure
 */
int free_ident(identifier_t *ident){
    if(ident){
        if(ident->qualifier) {
            free(ident->qualifier);
        }
        if(ident->source) {
            free(ident->source);
        }
        free(ident);
    }
    return 1;
}

/* Function: free_litteral
 * -----------------------------
 * free the data structure of a token_t
 * 
 * returns: 1 for success or 0 (NULL) for failure
 */
int free_litteral(token_t *token){
    if(token){
        if(token->value) {
            free(token->value);
        }
        free(token);
    }
    return 1;
}

/* Function: select_print
 * -----------------------
 * visibly print the select data structure
 * 
 * returns: nothing. All output is via stdio
 */
void select_print(select_t *select){
    // DEBUGGING INFORMATION

    printf("SELECT\n");
    if (select->distinct) printf("HAS DISTINCT\n");
    if (select->columns){

        for(int i=0;i<select->column_length;i++) {
            switch(select->columns[i].type){

                case TOKEN_STRING:
                case TOKEN_NUMERIC:
                case TOKEN_HEX:
                case TOKEN_BINARY:
                case TOKEN_REAL:
                case TOKEN_NULL:  printf("%s-%s ALIAS %s%d\n",
                                                        token_type(select->columns[i].type),
                                                        (char*)select->columns[i].object ,
                                                        select->columns[i].alias ,
                                                        select->columns[i].ordinal);
                                  break;
                case TOKEN_IDENTIFIER: 
                                  printf("%s- %s.%s ALIAS %s,%d\n",token_type(select->columns[i].type),
                                                            ((identifier_t *)select->columns[i].object)->qualifier ,
                                                            ((identifier_t *)select->columns[i].object)->source ,
                                                            select->columns[i].alias ,
                                                            select->columns[i].ordinal );
                                    break;
            }
        }
    }
   

     if (select->from) {
        printf("FROM\n");
        if(select->from->qualifier) {
            printf("%s.",select->from->qualifier);
        }
        if(select->from->source) {
            printf("%s",select->from->source);
            if(select->alias) printf(" ALIAS: %s ",select->alias);
            printf("\n");
        }
        
    }

    if (select->join) {
        printf("JOIN %d\n",select->join_length);
        for(int i=0;i<select->join_length;i++){
            if(select->join[i].identifier) {
                if(select->join[i].identifier->qualifier) {
                    printf("%s.",select->join[i].identifier->qualifier);
                }
                if(select->join[i].identifier->source) {
                    printf("%s ",select->join[i].identifier->source);
                }
                if(select->join[i].alias) printf("ALIAS: %s",select->join[i].alias);
                printf("\n");
            }
            debug_expr(select->join[i].expression,0);
        }
        
    }
    if(select->where) {
        printf(" ---WHERE---");
        debug_expr(select->where,0);
    }
    if(select->group) {
        printf(" ---GROUP---");
        debug_expr(select->group,0);
    }
    if(select->order) {
        printf(" ---ORDER---");
        debug_expr(select->order,0);
    }


    if (select->has_limit_start) printf("LIMIT_START:   %d\n",select->limit_start);
    if (select->has_limit_length) printf("LIMIT_LENGTH : %d\n",select->limit_length);
}

/* Function: debug_expr
 * -----------------------
 * visibly print the nested expresison_t data structure
 * 
 * returns: nothing. All output is via stdio
 */
void debug_expr(expression_t *expr,int depth){
    if(expr==0) {
        printf ("Expression NULL\n");
        return;
    }
    char *pad="";
    if(depth>0) pad=safe_malloc(depth+1,1);

    for(int i=0;i<depth;i++) pad[i]=' ';

    printf("%s- expr:\n",pad);
    printf("%s  - mode:   %d ",pad,expr->mode);
    printf("%s  - list:   %d ",pad,expr->list);
    printf("%s  - not:    %d ",pad,expr->not);
    printf("%s  - not_in: %d ",pad,expr->not_in);
    printf("%s  - in:     %d \n",pad,expr->in);
    printf("%s  - direction:  %s ",pad,token_type(expr->direction));
    printf("%s  - negative:   %d ",pad,expr->negative);
    printf("%s  - positive:   %d ",pad,expr->positive);
    printf("%s  - comparitor: %s ",pad,token_type(expr->comparitor));
    printf("%s  - operator:   %s \n",pad,token_type(expr->operator));
    if(expr->identifier){
        printf("%s - Identifier: %s.%s\n",pad,expr->identifier->qualifier,expr->identifier->source);
    }
    if(expr->literal) {
        printf("%s - Litteral: [%s] '%s'\n",pad,token_type(expr->literal->type),expr->literal->value);
    }

    if(depth>0) free(pad);
    if(expr->expression) debug_expr(expr->expression,depth+1);
    printf("\n");

}


int free_table_def(table_def_t *table_def){

    free(table_def);
}

token_t* match_key_numeric_value(token_array_t *tokens,int *index,int key){
    if(token_at(tokens,*index)->type==key) {
        ++*index;
        token_t *token=token_at(tokens,*index);
            if(token->type!=TOKEN_NUMERIC) { 
                --*index;
                return 0;
            }
        ++*index;
        return token->type;
    }
    return 0;
}

token_t* match_key_bool_value(token_array_t *tokens,int *index,int key){
    if(token_at(tokens,*index)->type==key) {
        ++*index;
        token_t *token=token_at(tokens,*index);
            if(token->type!=TOKEN_TRUE && token->type==TOKEN_FALSE) { 
                --*index;
                return 0;
            }
        ++*index;
        return token->type;
    }
    return 0;
}

char* match_key_string_value(token_array_t *tokens,int *index,int key){
    char *value=0;
    if(token_at(tokens,*index)->type==key) {
        ++*index;
        token_t *token=token_at(tokens,*index);
        if(token->type!=TOKEN_STRING) {
            --*index;
            return 0;
        }
        token=copy_token__at(tokens,*index);
        ++*index;
    }
    return value;
}

expression_t * process_column_list(token_array_t *tokens,int *index){
    expression_t *expr=0;
    expression_t *expr2=0;
    identifier_t *ident=0;
    int loop=1;
    while(loop) {
        ident=process_identifier(tokens,index);
        if(ident) {
            expr2=safe_malloc(sizeof(expression_t),1); 
            expr2->identifier=ident;
            expr2->mode=EXPRESSION_COLUMN;
            if(expr==0) {
                expr=expr2;
            } 
            else {
                add_expr(expr,expr2);
            }

            if(token_at(tokens,*index)->type!=TOKEN_LIST_DELIMITER) {
                loop=0;
            } else {
                ++*index;
            }
        } else {
            loop=0;
        }
    }
    return expr;
}

table_def_t * process_create_table(token_array_t *tokens,int *start){
    table_def_t *table_def=safe_malloc(sizeof(table_def),1);
   
   
   // switch        
    switch(token_at(tokens,*start)->type){
        case TOKEN_CREATE_TABLE: ++*start; break;
    }//end switch                

    table_def->identifier=process_identifier(tokens,start);

    if(table_def->identifier==0) {  free_table_def(table_def); return 0; }

    table_def->columns=process_column_list(tokens,start);
    if(table_def->columns==0) {  free_table_def(table_def); return 0; }

    table_def->file            =match_key_string_value(tokens,start,TOKEN_FILE          ,0);  
    table_def->fifo            =match_key_string_value(tokens,start,TOKEN_FIFO          ,0);  // optional
    // repo related items a group.... entire group is optional..
    table_def->repo            =match_key_string_value(tokens,start,TOKEN_REPO          ,0);  
    table_def->url             =match_key_string_value(tokens,start,TOKEN_URL           ,0);
    table_def->account         =match_key_string_value(tokens,start,TOKEN_ACCOUNT       ,0); 
    table_def->password        =match_key_string_value(tokens,start,TOKEN_PASSWORD      ,0);
    table_def->repo_path       =match_key_string_value(tokens,start,TOKEN_REPO_PATH     ,0);
    table_def->repo_base       =match_key_string_value(tokens,start,TOKEN_REPO_BASE     ,0);
    table_def->push_on_commit  =match_key_bool_value  (tokens,start,TOKEN_PUSH_ON_COMMIT,1);  // optional
    table_def->pull_on_read    =match_key_bool_value  (tokens,start,TOKEN_PULL_ON_COMMIT,1);  // optional  
    
    table_def->strict          =match_key_bool_value  (tokens,start,TOKEN_STRICT        ,0);  // optional
    table_def->column          =match_key_string_value(tokens,start,TOKEN_COLUMN        ,DEFAULT_COLUMN_DEIMITER);  // optional
    //table_def->array           =match_key_string_value(tokens,start,TOKEN_ARRAY         ,DEFAULT_ARRAY_DEIMITER);  // optional

    
    return table_def;
}