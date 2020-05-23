#include "include/core.h"
#include "include/structure.h"
#include "include/errors.h"

void structure_init(){
    select_t obj;
    // init data structure
    // sub elements
    obj.columns          =0;
    obj.where            =0;
    obj.order            =0;
    obj.group            =0;

    obj.from.qualifier   =0;
    obj.from.source      =0;

    // elements
    obj.alias            =0;
    obj.distinct         =0;
    obj.limit_start      =0;
    obj.limit_length     =0;
    
    // internal
    obj.column_length    =0;
    obj.has_limit_length =0;
    obj.has_limit_start  =0;
    obj.alias_target     =0;
}



// init a list of columns with 
data_column_t *data_column_list_init(int length){
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


void select_add_column(select_t obj){
    // columns... create, copy, destroy old, replace
    // create
    data_column_t *new_columns=data_column_list_init(obj.column_length+1);
    
    // if existing items exist
    if(obj.columns!=0) {
        // copy
        int data_size=sizeof(data_column_t)*obj.column_length;
        memcpy(new_columns,obj.columns,data_size);
        // destroy old
        free(obj.columns);
    }

    // replace
    obj.columns=new_columns;
    
    //init the newest column
    data_column_init(&obj.columns[obj.column_length]);

    obj.alias_target=&obj.columns[obj.column_length];
    ++obj.column_length;
}


void select_debug(select_t *select){
    printf("SELECT_DEBUG\n");
    
    char *has_alias;
    char *has_columns;
    char *has_group;
    char *has_order;
    char *has_where;

    if(select->alias  ==0) has_alias  ="NO"; else has_alias  ="YES";
    if(select->columns==0) has_columns="NO"; else has_columns="YES";
    if(select->group  ==0) has_group  ="NO"; else has_group  ="YES";
    if(select->order  ==0) has_order  ="NO"; else has_order  ="YES";
    if(select->where  ==0) has_where  ="NO"; else has_where  ="YES";

    printf("  has_alias      =%s\n"   ,has_alias);
    printf("  has_columns    =%s\n"   ,has_columns);
    printf("  has_group      =%s\n"   ,has_group);
    printf("  has_order      =%s\n"   ,has_order);
    printf("  has_where      =%s\n"   ,has_where);
    printf("  alias          =%s\n"   ,select->alias);
    printf("  distinct       =%d\n"   ,select->distinct);
    printf("  from.qualifier =%s\n"   ,select->from.qualifier);
    printf("  from.source    =%s\n"   ,select->from.source);
    printf("  limit_length   =%d\n"   ,select->limit_length);
    printf("  limit_start    =%d\n"   ,select->limit_start);
}


