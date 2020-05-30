#include "../../../include/errors.h"
#include "../../../include/tokens.h"
#include "../../../include/structure.h"
#include "../../../include/select.h"


token_t       * token_at               (token_array_t *tokens,int  index);
token_t       * duplicate_token        (token_array_t *tokens,int  index);
char          * copy_token_value_at    (token_array_t *tokens,int  index);
int             add_expr               (expression_t *expression,expression_t *item);
char          * process_alias          (token_array_t *tokens,int *index);
token_t       * process_litteral       (token_array_t *tokens,int *index);
expression_t  * process_simple_expr    (token_array_t *tokens,int *index);
expression_t  * process_bit_expr       (token_array_t *tokens,int *index);
expression_t  * process_expr_list      (token_array_t *tokens,int *index);
expression_t  * process_predicate      (token_array_t *tokens,int *index);
expression_t  * process_boolean_primary(token_array_t *tokens,int *index);
expression_t  * process_expression     (token_array_t *tokens,int *index);
void            debug_expr             (expression_t *expr,int depth);


void            process_select(token_array_t *tokens,int start,int end);
int             select_free(select_t select) ;
void            free_expression(expression_t *expr);
void            select_print(select_t select);

/* Function: expr_select
 * -----------------------------
 * process a length of tokens from a token_array_t 
 *          and build a select object;
 * 
 * returns: select_t object if a match is found
 *          returns zero (NULL) otherwise
 */int expr_select(token_array_t *tokens,int depth){
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
        process_select(tokens,pos,tokens->position);
        return 1;
    }// end token select if
    tokens->position=pos;
    return 0;
}// end func

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

int add_expr(expression_t *expression,expression_t *item){
    printf("ADDING\n");
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
    } else {
        identifier_t *ident=process_identifier(tokens,index);
        expr=safe_malloc(sizeof(expression_t),1);
        if(ident) {
            if (mode== 1) expr->positive=1;
            if (mode==-1) expr->negative=1;
        }
        expr->identifier=ident;
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

    expr=process_simple_expr(tokens,index);
    if(expr){
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
                                  if(add_expr(expr,process_bit_expr(tokens,index))){
                                      printf("GOT  A OPERATOR... \n");
                                      expr->operator=operator;
                                  } else { 
                                      printf("HAD A OPERATOR... \n");
                                      --*index;
                                  }
                                  break;

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
    }
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
                                    if(add_expr(expr,process_predicate(tokens,index))){
                                        expr->expression_tail->comparitor=token; 
                                    } else { 
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

/* Function: process_select
 * -----------------------
 * process
 * 
 * returns: nothing. All output is via stdio
 */
void process_select(token_array_t *tokens,int start,int end){
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
    select.join=0;
    select.where=0;
    select.order=0;
    select.group=0;
    select.has_limit_length=0;
    select.has_limit_start=0;
    select.limit_start=0;
    select.limit_length=0;
    select.column_length=0;
    select.join_length=0;
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
                                        ++i;
                                        add_join(&select);
                                        join_t *join=&select.join[select.join_length-1];
                                        join->identifier=process_identifier(tokens,&i);
                                        join->alias=process_alias(tokens,&i);
                                        switch(token_at(tokens,i)->type){
                                            case TOKEN_ON: ++i; 
                                                           join->expression=process_expression(tokens,&i);
                                                           if(join->expression==0)  {
                                                               printf("NO JOIN EXPRESSION\n");
                                                           }
                                                           break;
                                        }//end switch                

                                        
                                        

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

/* Function: select_free
 * -----------------------------
 * free the data structure of a select_t
 * 
 * returns: 1 for success or 0 (NULL) for failure
 */
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

void free_expression(expression_t *expr){

}
/* Function: select_print
 * -----------------------
 * visibly print the select data structure
 * 
 * returns: nothing. All output is via stdio
 */
void select_print(select_t select){
    // DEBUGGING INFORMATION

    if (select.distinct) printf("DISTINCT\n");
    if (select.columns){

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
        printf("JOIN %d\n",select.join_length);
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
            debug_expr(select.join[i].expression,0);
        }
        
    }

    if (select.has_limit_start) printf("LIMIT_START:   %d\n",select.limit_start);
    if (select.has_limit_length) printf("LIMIT_LENGTH : %d\n",select.limit_length);
}


void debug_expr(expression_t *expr,int depth){
    
    char *pad="";
    if(depth>0) pad=safe_malloc(depth+1,1);

    for(int i=0;i<depth;i++) pad[i]=' ';

    printf("%s- expr:\n",pad);
    printf("%s  - mode:       %d \n",pad,expr->mode);
    printf("%s  - list:       %d \n",pad,expr->list);
    printf("%s  - not:        %d \n",pad,expr->not);
    printf("%s  - not_in:     %d \n",pad,expr->not_in);
    printf("%s  - in:         %d \n",pad,expr->in);
    printf("%s  - negative:   %d \n",pad,expr->negative);
    printf("%s  - positive:   %d \n",pad,expr->positive);
    printf("%s  - comparitor: %s \n",pad,token_type(expr->comparitor));
    printf("%s  - operator:   %s \n",pad,token_type(expr->operator));
    if(expr->identifier){
        printf("%s - Identifier: %s.%s\n",pad,expr->identifier->qualifier,expr->identifier->source);
    }
    if(expr->literal) {
        printf("%s - Litteral: [%s] '%s'\n",pad,token_type(expr->literal->type),expr->literal->value);
    }

    if(depth>0) free(pad);
    if(expr->expression) debug_expr(expr->expression,depth+1);

}