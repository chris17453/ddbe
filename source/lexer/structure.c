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

    obj->from.qualifier   =0;
    obj->from.source      =0;

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

void data_column_init(data_column_t column){
    printf("old columns\n");

    column.alias=0;
    column.object=0;
    column.ordinal=-1;
    column.type=-1;
    printf("old columns\n");

}
// init a list of columns with 


void select_add_column(select_t *obj){
    // columns... create, copy, destroy old, replace
    // create
    printf("new columns\n");
    data_column_t *new_columns=data_column_list_init(obj->column_length+1);
    memset(new_columns,0,sizeof(data_column_t)*obj->column_length+1);
    // if existing items exist
    if(obj->columns!=0) {
        printf("old columns\n");
        // copy
        int data_size=sizeof(data_column_t)*obj->column_length;
        //memcpy(new_columns,obj->columns,data_size);
        // destroy old
        free(&obj->columns);
    }

    // replace
    obj->columns=&new_columns;
    
    printf("column init\n");
    //init the newest column
    data_column_init(obj->columns[obj->column_length]);

    ++obj->column_length;
    select_debug(obj);
    printf("NO \n");

}

void set_distinct(select_t *obj){
    obj->distinct=1;
}

void select_debug(select_t *obj){
    printf("SELECT_DEBUG\n");
    
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

    printf("  has_alias      =%s\n"   ,has_alias);
    printf("  has_columns    =%s\n"   ,has_columns);
    printf("  has_group      =%s\n"   ,has_group);
    printf("  has_order      =%s\n"   ,has_order);
    printf("  has_where      =%s\n"   ,has_where);
    printf("  column length  =%d\n"   ,obj->column_length);
    
    printf("  alias          =%s\n"   ,obj->alias);
    printf("  distinct       =%d\n"   ,obj->distinct);
    printf("  from.qualifier =%s\n"   ,obj->from.qualifier);
    printf("  from.source    =%s\n"   ,obj->from.source);
    printf("  limit_length   =%d\n"   ,obj->limit_length);
    printf("  limit_start    =%d\n"   ,obj->limit_start);
    if (obj->columns!=0){
        printf (" Columns: \n");
        for(int i=0;i<obj->column_length;i++){
            printf(" alias:%s ",obj->columns[i].alias);
            printf(" type:%d ",obj->columns[i].type);
            printf(" ordinal:%d ",obj->columns[i].ordinal);
            printf("\n");
        }
    }
}


