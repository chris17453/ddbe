// tokens 

#if !defined(_TOKENS_H_)
    #define _TOKENS_H_ 1


    #define TOKEN_BLANK          0
    #define TOKEN_REAL           1
    #define TOKEN_ALPHA          2
    #define TOKEN_NUMERIC        3
    #define TOKEN_BOOL           63
    #define TOKEN_NULL           60
    #define TOKEN_UNKNOWN        64
    #define TOKEN_FALSE          61
    #define TOKEN_TRUE           62
    #define TOKEN_STRING         13
    #define TOKEN_WHITESPACE     14
    #define TOKEN_SHIFT_LEFT     15
    #define TOKEN_SHIFT_RIGHT    16
    #define TOKEN_SHORT_OR       17
    #define TOKEN_SHORT_AND      18
    #define TOKEN_NULL_EQ        19
    #define TOKEN_LESS_EQ        20
    #define TOKEN_GREATER_EQ     21
    #define TOKEN_NOT_EQ         67
    #define TOKEN_PLS_EQ         22
    #define TOKEN_MIN_EQ         23
    #define TOKEN_DIV_EQ         24
    #define TOKEN_MUL_EQ         25
    #define TOKEN_MOD_EQ         26
    #define TOKEN_LESS           27
    #define TOKEN_GREATER        28
    #define TOKEN_PLUS           29
    #define TOKEN_MINUS          30
    #define TOKEN_DIVIDE         31
    #define TOKEN_MULTIPLY       32
    #define TOKEN_NOT            33
    #define TOKEN_MODULUS        34
    #define TOKEN_ASSIGNMENT     35
    #define TOKEN_PAREN_LEFT     36
    #define TOKEN_PAREN_RIGHT    37
    #define TOKEN_LIST_DELIMITER 38
    #define TOKEN_DOT            39
    #define TOKEN_HEX            40
    #define TOKEN_BINARY         41             
    #define TOKEN_SELECT         42               
    #define TOKEN_DISTINCT       43       
    #define TOKEN_FROM           44       
    #define TOKEN_AS             45   
    #define TOKEN_WHERE          46   
    #define TOKEN_AND            47   
    #define TOKEN_OR             48   
    #define TOKEN_ORDER          49   
    #define TOKEN_GROUP          50   
    #define TOKEN_BY             51   
    #define TOKEN_LIMIT          52   
    #define TOKEN_ASC            53   
    #define TOKEN_DESC           55   
    #define TOKEN_JOIN           56
    #define TOKEN_ON             57
    #define TOKEN_LIKE           58
    #define TOKEN_IN             65
    #define TOKEN_IS             66 
    #define TOKEN_DELIMITER      70
    #define TOKEN_BIT_OR         71
    #define TOKEN_BIT_AND        72
    #define TOKEN_LINE_COMMENT   73
    #define TOKEN_BLOCK_COMMENT  74
    #define TOKEN_NEW_LINE       75
    #define TOKEN_LINE_FEED      76
    #define TOKEN_TAB            77
    //#define TOKEN_ALIAS          78
    //#define TOKEN_EXPRESSION     78

    

    typedef struct identifier_t {
        char *qualifier;
        char *source;
    } identifier_t;

    typedef struct list{
        char *item;
    } list;

    typedef struct  data_column_t{
        identifier_t identifier;
        char *function;
        char *alias;
        int is_function;
        int is_litteral;
        int is_expr;
        int order;
    } data_column_t;

    typedef struct where_t{

        

    } where_t;

    typedef struct token_t{
        int type;
        char *value;
    } token_t;

    typedef struct dataset_t{
        int length;
        data_column_t *columns;
        identifier_t identifier;
        char *alias;
    } dataset_t;

    typedef struct select_t{
        char *alias;
        int distinct;
        identifier_t from;
        token_t *columns;
        token_t *where;
        token_t *order_by;
        token_t *group_by;
        int limit_start;
        int limit_length;
        int has_alias;
        int has_columns;
        int has_group;
        int has_order;
        int has_where;
    } select_t;

    // A structure to represent a stack 
    typedef struct token_array_t{ 
        int top; 
        unsigned length; 
        token_t * array; 
        int position;
        void * target;
        void *token_object;
        
    } token_array_t; 




    token_array_t *  token_array       (unsigned length);
    void             token_close       (token_array_t * arr);
    int              valid_token_index(token_array_t *tokens,int index);
    int              token_is_full     (token_array_t * arr);
    int              token_is_empty    (token_array_t * arr);
    void             token_push        (token_array_t * arr, token_t *item);
    token_t          token_pop         (token_array_t * arr);
    token_t          token_peek        (token_array_t * arr);
    void             token_delete      (token_array_t* tokens,int index);
    void             token_print       (token_array_t * arr);
    char            *token_type        (int t);

#endif