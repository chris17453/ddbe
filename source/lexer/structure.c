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
    obj->where            =0;
    obj->order            =0;
    obj->group            =0;

    obj->from             =0;

    // elements
    obj->alias            =0;
    obj->distinct         =0;
    obj->limit_start      =0;
    obj->limit_length     =0;
    
    // internal
    obj->column_length    =0;
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


void select_add_column(select_t *obj){
    // columns... create, copy, destroy old, replace
    // create
    data_column_t *new_columns=data_column_list_init(obj->column_length+1);
    memset(new_columns,0,sizeof(data_column_t)*obj->column_length+1);
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

void set_distinct(select_t *obj){
    obj->distinct=1;
}

void select_debug(select_t *obj){
    printf("SELECT_DEBUG\n");
    
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

    printf(" ----------------\n");
    printf("  has_from       =%s\n"   ,has_from);
    printf("  has_alias      =%s\n"   ,has_alias);
    printf("  has_columns    =%s\n"   ,has_columns);
    printf("  has_group      =%s\n"   ,has_group);
    printf("  has_order      =%s\n"   ,has_order);
    printf("  has_where      =%s\n"   ,has_where);
    printf("  has_limit_start =%s\n"   ,has_limit_start);
    printf("  has_limit_length=%s\n"   ,has_limit_length);
    printf("  column length  =%d\n"   ,obj->column_length);
    printf(" ----------------\n");
    
    printf("  alias          =%s\n"   ,obj->alias);
    printf("  distinct       =%d\n"   ,obj->distinct);
    if(obj->from){
        printf("  from qualifier =%s\n"   ,obj->from->qualifier);
        printf("  from source    =%s\n"   ,obj->from->source);
    }
    printf("  limit_length   =%d\n"   ,obj->limit_length);
    printf("  limit_start    =%d\n"   ,obj->limit_start);
    if (obj->columns!=0){
        printf (" columns:{ \n");
        for(int i=0;i<obj->column_length;i++){
        printf ("    { ");
            printf(" alias: %s ,",obj->columns[i].alias);
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
            printf(" ordinal: %d ",obj->columns[i].ordinal);
            printf("}, \n");
        }
        printf("} \n");
    }
}


