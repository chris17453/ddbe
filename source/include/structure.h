
#include "tokens.h"

#if !defined(_STRUCTURE_H_)
    #define _STRUCTURE_H_ 1



typedef struct identifier_t {
    char *qualifier;
    char *source;
} identifier_t;

typedef struct  data_column_t{
    int   type;
    int   ordinal;
    void *object;
    char *alias;
} data_column_t;

typedef struct order_column_t {
    identifier_t *identity;
    int direction;
} order_column_t;

typedef struct group_column {
    identifier_t *identity;
} group_by;

typedef struct select_t{
        // sub elements
        data_column_t *columns;
        identifier_t  *from;
        void          *where;
        void          *order;
        void          *group;
        
        // elements
        char          *alias;
        int            distinct;
        int            limit_start;
        int            limit_length;

        // internal
        int            column_length;     
        int            order_length;     
        int            group_length;     
        int            where_length;     
        int            has_limit_length;
        int            has_limit_start;
    } select_t;
    

    select_t       *select_new();
    data_column_t  *data_column_list_init(int length);
    void            select_add_column(select_t *obj);
    void            data_column_init(data_column_t *column);
    void            set_distinct(select_t *obj);
    void            select_debug(select_t *obj);

#endif