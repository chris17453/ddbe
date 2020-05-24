
#include "tokens.h"

#if !defined(_STRUCTURE_H_)
    #define _STRUCTURE_H_ 1

typedef struct select_t{
        // sub elements
        data_column_t *columns;
        identifier_t   from;
        void          *where;
        void          *order;
        void          *group;
        
        // elements
        char          *alias;
        int            distinct;
        int            column_length;     
        int            limit_start;
        int            limit_length;

        // internal
        int            has_limit_length;
        int            has_limit_start;
        void         * alias_target;
    } select_t;
    

    select_t       *select_new();
    data_column_t  *data_column_list_init(int length);
    void            select_add_column(select_t obj);
    void            data_column_init(data_column_t *column);
    void            set_distinct(select_t *obj);
    void            select_debug(select_t *obj);

#endif