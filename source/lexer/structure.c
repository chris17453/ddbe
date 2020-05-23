#include "include/core.h"
#include "include/structure.h"
#include "include/errors.h"

void structure_init(){
    select_t obj;
    // init data structure
    obj->alias            ="";
    obj.distinct         =0;
    obj.column_length    =0;

    obj.from.qualifier   ="";
    obj.from.source      ="";
    obj.limit_start      =0;
    obj.limit_length     =0;
    
    obj.has_alias        =0;
    obj.has_columns      =0;
    obj.has_group        =0;
    obj.has_order        =0;
    obj.has_where        =0;
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
    //if no columns... create
    if(obj.has_columns<=0) {
        obj.columns =data_column_list_init(1);
    } else {
        // columns... create, copy, destroy old, replace
        // create
        data_column_t *new_columns=data_column_list_init(obj.column_length+1);
        
        // copy
        int data_size=sizeof(data_column_t)*obj.column_length;
        memcpy(new_columns,obj.columns,data_size);
        // destroy old
        free(obj.columns);
        // replace
        obj.columns=new_columns;
    }
    

    //init the newest column
    data_column_init(&obj.columns[obj.column_length]);

    ++obj.column_length;
}


