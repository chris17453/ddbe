#include "include/errors.h"
#include <memory.h>

void * safe_malloc(int size,int qty){
    int length=size*qty;
    if(length<=0) ghost(ERR_OUT_OF_BOUNDS);
    printf("malloc %d\n",length);
    void *ptr=(void *)malloc(length*2);
    printf("malloc2\n");
    if(ptr==0) {
        ghost(ERR_MEMORY_ALLOCATION_ERR);
    }
    return ptr;
}