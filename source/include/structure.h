
#include "tokens.h"

#if !defined(_STRUCTURE_H_)
    #define _STRUCTURE_H_ 1

typedef struct select_t{
        char          *alias;
        int            distinct;
        data_column_t *columns;
        identifier_t   from;
        //token_t *where;
        //token_t *order_by;
        //token_t *group_by;
        int    column_length;

        int    limit_start;
        int    limit_length;
        int    has_limit_length;
        int    has_limit_start;
        void * alias_target;
    } select_t;
    

    void            structure_init();
    data_column_t  *data_column_list_init(int length);
    void            select_add_column(select_t obj);
    void            data_column_init(data_column_t *column);

#endif