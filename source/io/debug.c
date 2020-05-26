// message output
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/errors.h"
#include "../include/debug.h"

#define DEBUG 1
//#define PARSE_ENTRANCE 1


void gabble(char *source,char *msg){
#if defined(DEBUG)

    char *type="ERR";
    printf(" %s %s: %s\n",type,source,msg);
#endif
}

void gobble(char *source,char *msg){
#if defined(DEBUG)
    char *type="INFO";
    printf(" %s %s: %s\n",type,source,msg);
#endif
}

void goop(int depth,char *source,char *msg){
 #if defined(DEBUG)
    for (int i=0;i<depth;i++) printf(" ");
    printf("%s: %s\n",source,msg);
 #endif
}   

void vomit(int err_no){
    char *msg;
    switch(err_no){
        case ERR_UNTERMINATED_COMMENT_BLOCK  : msg="unterminated comment block"; break;
        case ERR_MEMORY_ALLOCATION_ERR       : msg="memory allocation error."; break;
        case ERR_FILE_OPEN_ERROR             : msg="file open error"; break;
        case ERR_EMPTY_QUERY_STRING          : msg="empty query string"; break;
        case ERR_EMPTY_STRING                : msg="empty string"; break;
        case ERR_STRING_DUPLICATION_ERROR    : msg="string duplication error"; break;
        case ERR_OUT_OF_BOUNDS               : msg="index out of bounds"; break;
        case ERR_UNTERMINATED_STRING         : msg="unterminated string"; break;
        case ERR_MALFORMED_HEX_TOKEN         : msg="malformed hex token"; break;
        case ERR_MALFORMED_BINARY_TOKEN      : msg="malformed binary token"; break;
        case ERR_UNKNOWN_SQL                 : msg="unknown sql"; break;
        case ERR_TOKENS_FULL                 : msg="tokens full"; break;   
        case ERR_TOKENS_OUT_OF_BOUNDS        : msg="tokens out of bounds"; break;           
        case ERR_TOKENS_EMPTY                : msg="tokens empty"; break;   
        case ERR_INVALID_SELECT_EXPR_ALIAS   : msg="select expression has an missing or invalid alias"; break;   
        case ERR_UNTERMINATED_LINE_COMENT    : msg="unterminated line comment"; break;
        case ERR_UNTERMINATED_BLOCK_COMENT   : msg="unterminated block comment"; break;
        case ERR_TOKEN_TARGET_NULL           : msg="token target is null"; break;

        default: msg="NO CLUE";
    }
    printf("%d ",err_no);
    gabble("SYS",msg);
}

void ghost(int err_no){
    vomit(err_no);
    exit(err_no);
}

char *string_duplicate(const char *str){
    char *new_str = strdup(str);
    if(new_str == NULL)
        ghost(ERR_STRING_DUPLICATION_ERROR);
    return new_str;
}


char *sub_str_cpy(char *data,int start,int length){
    int string_len=strlen(data);
    if(start<0 || start >string_len) {
        ghost(ERR_OUT_OF_BOUNDS);
    }
    if(string_len<length) {
        ghost(ERR_OUT_OF_BOUNDS);
    }
    if( length<1) {
        //exit(EMPTY_STRING);
        return "?";
    }
    char *buffer=malloc(length+1);
    if (buffer==NULL){
        ghost(ERR_MEMORY_ALLOCATION_ERR);
    }

    memset(buffer,0,length+1);
    memcpy(buffer,&data[start],length);

    return buffer;
}

