
#include "../include/dataset.h"

void dataset_create(int length){
    dataset_t data;
    data.length=length;
    
    data.alias="";
    
    identifier_t identifier;
    identifier.qualifier="";
    identifier.source="";

    data.identifier=identifier;

    data.columns=(data_column_t*)malloc(sizeof(data_column_t)*length);
}

