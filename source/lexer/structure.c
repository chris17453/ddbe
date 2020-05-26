#include "include/core.h"
#include "include/structure.h"
#include "include/errors.h"
#include <stdio.h>
#include <string.h>


select_t *select_new(){
    select_t *obj=safe_malloc(sizeof(select_t),1);
    // init data structure
    // sub elements
    obj->columns          =0;
    obj->from             =0;
    obj->where            =0;
    obj->group            =0;
    obj->order            =0;

    // elements
    obj->alias            =0;
    obj->distinct         =0;
    obj->limit_start      =0;
    obj->limit_length     =0;
    
    // internal
    obj->column_length    =0;
    obj->where_length     =0;
    obj->order_length     =0;
    obj->group_length     =0;
    obj->has_limit_length =0;
    obj->has_limit_start  =0;
    return obj;
}


// init a list of columns with 
data_column_t *data_column_list_init(int length){
    printf("data->columns malloc \n");
    data_column_t *columns =safe_malloc(sizeof(data_column_t),length);
    return columns;
}

void data_column_init(data_column_t *column){
    column->alias=0;
    column->object=0;
    column->ordinal=-1;
    column->type=-1;
}
// init a list of columns with 

void add_data_column(select_t *obj){
    // columns... create, copy, destroy old, replace
    // create
    data_column_t *new_columns=data_column_list_init(obj->column_length+1);
    // if existing items exist
    if(obj->columns!=0) {
        // copy
        int data_size=sizeof(data_column_t)*obj->column_length;
        memcpy(new_columns,obj->columns,data_size);
        // destroy old
        free(obj->columns);
    }

    // replace
    obj->columns=new_columns;
    //init the newest column
    data_column_init(&obj->columns[obj->column_length]);
    ++obj->column_length;
}

// init a list of columns with 
order_column_t *order_column_list_init(int length){
    printf("order->columns malloc \n");
    order_column_t *columns =safe_malloc(sizeof(order_column_t),length);
    return columns;
}

void order_column_init(order_column_t *column){
    column->direction=TOKEN_ASC;
    column->identity=0;
    column->ordinal=0;
}

void add_order_column(select_t *obj){
    // columns... create, copy, destroy old, replace
    // create
    order_column_t *new_columns=order_column_list_init(obj->order_length+1);
    // if existing items exist
    if(obj->order!=0) {
        // copy
        int data_size=sizeof(order_column_t)*obj->order_length;
        memcpy(new_columns,obj->order,data_size);
        // destroy old
        free(obj->order);
    }

    // replace
    obj->order=new_columns;
    //init the newest column
    order_column_init(&obj->order[obj->order_length]);
    ++obj->order_length;
}

// init a list of columns with 
group_column_t *group_column_list_init(int length){
    printf("group->columns malloc \n");
    group_column_t *columns =safe_malloc(sizeof(group_column_t),length);
    return columns;
}

void group_column_init(group_column_t *column){
    column->ordinal=0;
    column->identity=0;
}

void add_group_column(select_t *obj){
    // columns... create, copy, destroy old, replace
    // create
    group_column_t *new_columns=group_column_list_init(obj->group_length+1);
    // if existing items exist
    if(obj->group!=0) {
        // copy
        int data_size=sizeof(group_column_t)*obj->group_length;
        memcpy(new_columns,obj->group,data_size);
        // destroy old
        free(obj->group);
    }

    // replace
    obj->group=new_columns;
    //init the newest column
    group_column_init(&obj->group[obj->group_length]);
    ++obj->group_length;
}


// init a list of columns with 
where_expr_t *where_expr_list_init(int length){
    printf("where->expr malloc \n");
    where_expr_t *expr =safe_malloc(sizeof(where_expr_t),length);
    return expr;
}

void where_expr_init(where_expr_t *exp){
    exp->NOT=0;
    exp->length=0;
    exp->comparitor=0;
    exp->ordinal=0;
    exp->tokens=0;
}

void add_where_expr(select_t *obj){
    // columns... create, copy, destroy old, replace
    // create
    where_expr_t *new_expr=where_expr_list_init(obj->where_length+1);
    // if existing items exist
    if(obj->group!=0) {
        // copy
        int data_size=sizeof(where_expr_t)*obj->where_length;
        memcpy(new_expr,obj->where,data_size);
        // destroy old
        free(obj->where);
    }

    // replace
    obj->where=new_expr;
    //init the newest expr
    where_expr_init(&obj->where[obj->where_length]);
    ++obj->where_length;
}


void set_distinct(select_t *obj){
    obj->distinct=1;
}

void select_debug(select_t *obj){
    printf("\nSELECT_DEBUG\n");
    
    char *has_limit_start;
    char *has_limit_length;
    char *has_from;
    char *has_alias;
    char *has_columns;
    char *has_group;
    char *has_order;
    char *has_where;

    if(obj->alias  ==0) has_alias  ="NO"; else has_alias  ="YES";
    if(obj->columns==0) has_columns="NO"; else has_columns="YES";
    if(obj->group  ==0) has_group  ="NO"; else has_group  ="YES";
    if(obj->order  ==0) has_order  ="NO"; else has_order  ="YES";
    if(obj->where  ==0) has_where  ="NO"; else has_where  ="YES";
    if(obj->from   ==0) has_from   ="NO"; else has_from   ="YES";
    if(obj->limit_start ==0) has_limit_start ="NO"; else has_limit_start ="YES";
    if(obj->limit_length==0) has_limit_length="NO"; else has_limit_length="YES";

    printf(" -flags-----------\n");
    printf("   has_from        =%s\n"   ,has_from);
    printf("   has_alias       =%s\n"   ,has_alias);
    printf("   has_columns     =%s\n"   ,has_columns);
    printf("   has_group       =%s\n"   ,has_group);
    printf("   has_order       =%s\n"   ,has_order);
    printf("   has_where       =%s\n"   ,has_where);
    printf("   has_limit_start =%s\n"   ,has_limit_start);
    printf("   has_limit_length=%s\n"   ,has_limit_length);
    printf("   column length   =%d\n"   ,obj->column_length);
    printf("   order length    =%d\n"   ,obj->order_length);
    printf("   group length    =%d\n"   ,obj->group_length);
    printf("   where length    =%d\n"   ,obj->where_length);
    printf(" -values----------\n");
    
    printf("   alias           =%s\n"   ,obj->alias);
    printf("   distinct        =%d\n"   ,obj->distinct);
    if(obj->from){
        printf("   from qualifier  =%s\n"   ,obj->from->qualifier);
        printf("   from source     =%s\n"   ,obj->from->source);
    }
    printf("   limit_length    =%d\n"   ,obj->limit_length);
    printf("   limit_start     =%d\n"   ,obj->limit_start);
    
    
    if (obj->columns!=0){
        printf(" -data-columns----------\n");
        for(int i=0;i<obj->column_length;i++){
            printf("   alias: %s ,",obj->columns[i].alias);
            printf(" type: %s ,",token_type(obj->columns[i].type));
            if (obj->columns[i].type==TOKEN_IDENTIFIER) {
               identifier_t *ident=(identifier_t*)obj->columns[i].object;
               printf("Qualifier: %s,Source: %s",ident->qualifier,ident->source);
            }
            if (obj->columns[i].type==TOKEN_LITTERAL) {
                token_t *token=(token_t*)obj->columns[i].object;
                //printf(" sub type: %s ,");

                printf("-%s,val: %s",token_type(token->type),token->value );

            }
            printf(" ordinal: %d \n",obj->columns[i].ordinal);
        }
    }
    if (obj->where!=0){
        printf(" -where----------\n");
        for(int i=0;i<obj->where_length;i++){
            int *sub_len=(where_expr_t*)obj->where[i].length;
            for(int w=0;w<sub_len;w++) {
                token_t *token=&obj->where[i].tokens[w];
                printf("     %s,val: %s\n",token_type(token->type),token->value );
            }
        }
    }
    
    if (obj->group!=0){
        printf(" -group-----------\n");
        for(int i=0;i<obj->group_length;i++){
                group_column_t *group=&obj->group[i];
                printf("   Qualifier: %s,Source: %s",group->identity->qualifier,group->identity->source);
                printf(", ordinal: %d, \n",group->ordinal);
        }
    }    

    if (obj->order!=0){
        printf(" -order-----------\n");
        for(int i=0;i<obj->order_length;i++){

                order_column_t *order=&obj->order[i];
                printf("   Qualifier: %s,Source: %s",order->identity->qualifier,order->identity->source);
            
                printf(" %s, ordinal: %d\n",token_type(order->direction),order->ordinal);
        }
    }    
}


