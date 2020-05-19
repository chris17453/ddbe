#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "../include/errors.h"
#include "../include/tokens.h"
#include "../include/debug.h"


token_array_t *lex(char * query){
    unsigned int query_length=strlen(query);
    if (query_length<1){
        ghost(ERR_EMPTY_QUERY_STRING);

    }

    char *sanitized_sql=query;
    query_length=strlen(sanitized_sql);
    if (query_length<1){
        ghost(ERR_EMPTY_QUERY_STRING);
    }
    

    int new_word=0;
    char c,d,e;
    int skip=0;
    char t=0;
    int buffer_len=0;
    char *new_token;
    token_array_t *tokens=token_array(1000);

    for(int i=0;i<query_length;i++){
        buffer_len=query_length-i;
        c=sanitized_sql[i];

        //The start of a new token....
        
        if(new_word==0){
            if (buffer_len>2) d=sanitized_sql[i+1]; else  d=0;
            if (buffer_len>3) e=sanitized_sql[i+2]; else  e=0;
            skip=0;
            if      (c>='a' && c<='z')           { t=TOKEN_ALPHA;            }
            else if (c>='A' && c<='Z')           { t=TOKEN_ALPHA;            }
            else if (c=='\'')                    { t=TOKEN_STRING;           }
            else if (c=='"')                     { t=TOKEN_STRING;           }
            else if (c<=32)                      { t=TOKEN_WHITESPACE;       }
            else if (c=='0' && d=='x')           { t=TOKEN_HEX;              }
            else if (c=='0' && d=='X')           { t=TOKEN_HEX;              }
            else if (c=='0' && d=='b')           { t=TOKEN_BINARY;           }
            else if (c=='0' && d=='B')           { t=TOKEN_BINARY;           }
            else if (c>='0' && c<='9')           { t=TOKEN_NUMERIC;          }
            else if (c=='<' && d=='=' && e=='>') { t=TOKEN_NULL_EQ;         skip=2; }
            else if (c=='-' && d=='-')           { t=TOKEN_LINE_COMMENT;    skip=1; }
            else if (c=='/' && d=='*')           { t=TOKEN_BLOCK_COMMENT;   skip=1; }
            else if (c=='<' && d=='<')           { t=TOKEN_SHIFT_LEFT;      skip=1; }
            else if (c=='>' && d=='>')           { t=TOKEN_SHIFT_RIGHT;     skip=1; }
            else if (c=='|' && d=='|')           { t=TOKEN_SHORT_OR;        skip=1; }
            else if (c=='&' && d=='&')           { t=TOKEN_SHORT_AND;       skip=1; }
            else if (c=='<' && d=='=')           { t=TOKEN_LESS_EQ;         skip=1; }
            else if (c=='>' && d=='=')           { t=TOKEN_GREATER_EQ;      skip=1; }
            else if (c=='+' && d=='=')           { t=TOKEN_PLS_EQ;          skip=1; }
            else if (c=='-' && d=='=')           { t=TOKEN_MIN_EQ;          skip=1; }
            else if (c=='/' && d=='=')           { t=TOKEN_DIV_EQ;          skip=1; }
            else if (c=='*' && d=='=')           { t=TOKEN_MUL_EQ;          skip=1; }
            else if (c=='%' && d=='=')           { t=TOKEN_MOD_EQ;          skip=1; }
            else if (c=='!' && d=='=')           { t=TOKEN_NOT_EQ;          skip=1; }
            else if (c=='<' && d=='>')           { t=TOKEN_NOT_EQ;          skip=1; }
            else if (c=='<')                     { t=TOKEN_LESS;            skip=1; }
            else if (c=='>')                     { t=TOKEN_GREATER;         skip=0; }
            else if (c=='+')                     { t=TOKEN_PLUS;            skip=0; }
            else if (c=='-')                     { t=TOKEN_MINUS;           skip=0; }
            else if (c=='/')                     { t=TOKEN_DIVIDE;          skip=0; }
            else if (c=='*')                     { t=TOKEN_MULTIPLY;        skip=0; }
            else if (c=='!')                     { t=TOKEN_NOT;             skip=0; }
            else if (c=='%')                     { t=TOKEN_MODULUS;         skip=0; }
            else if (c=='=')                     { t=TOKEN_ASSIGNMENT;      skip=0; }
            else if (c=='(')                     { t=TOKEN_PAREN_LEFT;      skip=0; }
            else if (c==')')                     { t=TOKEN_PAREN_RIGHT;     skip=0; }
            else if (c==',')                     { t=TOKEN_LIST_DELIMITER;  skip=0; }
            else if (c=='.')                     { t=TOKEN_DOT;             skip=0; }
            else if (c==';')                     { t=TOKEN_DELIMITER;       skip=0; }
            else if (c=='|')                     { t=TOKEN_BIT_OR;          skip=0; }
            else if (c=='&')                     { t=TOKEN_BIT_AND;         skip=0; }
            else if (c=='\n')                    { t=TOKEN_NEW_LINE;        skip=0; }
            else if (c=='\r')                    { t=TOKEN_LINE_FEED;       skip=0; }
            else if (c=='\t')                    { t=TOKEN_TAB;             skip=0; }
    
            else t=0;
            switch(t){
                case TOKEN_ALPHA:
                    for(int lazer=i+1;lazer<query_length;lazer++){
                        c=sanitized_sql[lazer];
                        if ((c>='a' && c<='z') ||   
                            (c>='A' && c<='Z') ||
                            (c=='_' ) ||
                            (c>='0' && c<='9')) continue;
                        else {
                            new_token=sub_str_cpy(sanitized_sql,i,lazer-i);
                            skip=lazer-i-1;
                            break;
                        }
                    }
                    if     (0==strncasecmp(new_token,"DISTINCT" ,10) ) t=TOKEN_DISTINCT;
                    else if(0==strncasecmp(new_token,"SELECT"   ,10) ) t=TOKEN_SELECT;
                    else if(0==strncasecmp(new_token,"WHERE"    ,10) ) t=TOKEN_WHERE;
                    else if(0==strncasecmp(new_token,"ORDER"    ,10) ) t=TOKEN_ORDER;
                    else if(0==strncasecmp(new_token,"GROUP"    ,10) ) t=TOKEN_GROUP;
                    else if(0==strncasecmp(new_token,"LIMIT"    ,10) ) t=TOKEN_LIMIT;
                    else if(0==strncasecmp(new_token,"DESC"     ,10) ) t=TOKEN_DESC;
                    else if(0==strncasecmp(new_token,"JOIN"     ,10) ) t=TOKEN_JOIN;
                    else if(0==strncasecmp(new_token,"LIKE"     ,10) ) t=TOKEN_LIKE;
                    else if(0==strncasecmp(new_token,"FROM"     ,10) ) t=TOKEN_FROM;
                    else if(0==strncasecmp(new_token,"NULL"     ,10) ) t=TOKEN_NULL;
                    else if(0==strncasecmp(new_token,"TRUE"     ,10) ) t=TOKEN_FALSE;
                    else if(0==strncasecmp(new_token,"FALSE"    ,10) ) t=TOKEN_TRUE;
                    else if(0==strncasecmp(new_token,"UNKNOWN"  ,10) ) t=TOKEN_UNKNOWN;
                    else if(0==strncasecmp(new_token,"AND"      ,10) ) t=TOKEN_AND;
                    else if(0==strncasecmp(new_token,"ASC"      ,10) ) t=TOKEN_ASC;
                    else if(0==strncasecmp(new_token,"BY"       ,10) ) t=TOKEN_BY;
                    else if(0==strncasecmp(new_token,"AS"       ,10) ) t=TOKEN_AS;
                    else if(0==strncasecmp(new_token,"OR"       ,10) ) t=TOKEN_OR;
                    else if(0==strncasecmp(new_token,"ON"       ,10) ) t=TOKEN_ON;
                    else if(0==strncasecmp(new_token,"IS"       ,10) ) t=TOKEN_IS;
                    else if(0==strncasecmp(new_token,"IN"       ,10) ) t=TOKEN_IN;
                    else if(0==strncasecmp(new_token,"NOT"      ,10) ) t=TOKEN_NOT;
                    break;

                case TOKEN_NUMERIC: 

                    for(int lazer=i+1;lazer<query_length;lazer++){
                        c=sanitized_sql[lazer];
                        if (c>='0' && c<='9') continue;
                        else {
                            new_token=sub_str_cpy(sanitized_sql,i,lazer-i);
                            skip=lazer-i-1;
                            break;
                        }
                    }

                    break;

                case TOKEN_HEX: 
                    for(int lazer=i+2;lazer<query_length;lazer++){
                        c=sanitized_sql[lazer];
                        if ((c>='0' && c<='9') || ( (c>='a' && c<='f') || (c>='A' && c<='F') ) ) continue;
                        else {
                            new_token=sub_str_cpy(sanitized_sql,i,lazer-i);
                            skip=lazer-i-1;
                            break;
                        }
                    }
                    if(skip==0) ghost(ERR_MALFORMED_HEX_TOKEN);
                    if ( (c>'F' && c<='Z') || 
                         (c>'F' && c<='z') ) ghost(ERR_MALFORMED_HEX_TOKEN);
                    break;

                case TOKEN_BINARY: 
                    for(int lazer=i+2;lazer<query_length;lazer++){
                        c=sanitized_sql[lazer];
                        if (c=='0' || c=='1') continue;
                        else {
                            new_token=sub_str_cpy(sanitized_sql,i,lazer-i);
                            skip=lazer-i-1;
                            break;
                        }
                    }
                    if(skip==0) ghost(ERR_MALFORMED_BINARY_TOKEN);
                    if ( (c>'1' && c<='9') || 
                         (c>='A' && c<='Z') || 
                         (c>='a' && c<='z') ) ghost(ERR_MALFORMED_BINARY_TOKEN);

                    break;


                case TOKEN_WHITESPACE: 
                    for(int lazer=i+1;lazer<query_length;lazer++){
                        c=sanitized_sql[lazer];
                        if (c<32) continue;
                        else {
                            new_token=sub_str_cpy(sanitized_sql,i,lazer-i);
                            skip=lazer-i-1;
                            break;
                        }
                    }
                    break;
                case TOKEN_LINE_COMMENT:
                    //is it long enough
                    skip=-1;
                    for(int lazer=i+2;lazer<query_length;lazer++){
                        c=sanitized_sql[lazer];
                        if (c=='\n') {
                            new_token=sub_str_cpy(sanitized_sql,i+2,lazer-i-2);
                            skip=lazer-i-1;
                            break;
                        }
                    }
                    if(skip==-1){
                        printf("Error at: %d of %d\n",i,query_length);
                        ghost(ERR_UNTERMINATED_LINE_COMENT);
                    }
                    break;
                case TOKEN_BLOCK_COMMENT:
                    //is it long enough
                    if(query_length-i-3>0){
                        for(int lazer=i+2;lazer<query_length-1;lazer++){
                            c=sanitized_sql[lazer];
                            d=sanitized_sql[lazer+1];
                            
                            if (c=='*' && d=='/') {;
                                new_token=sub_str_cpy(sanitized_sql,i+2,lazer-i-2);
                                skip=lazer-i+1;
                                break;
                            }
                        }
                    } else {
                        ghost(ERR_UNTERMINATED_BLOCK_COMENT);
                    }

                    break;



                case TOKEN_STRING:
                    skip=-1;
                    for(int lazer=i+1;lazer<query_length;lazer++){
                        if(sanitized_sql[lazer]==c) {
                            // it's an empty string.....
                            if (lazer-i==1) 
                                new_token="";
                            else
                                new_token=sub_str_cpy(sanitized_sql,i+1,lazer-i-1);
                            skip=lazer-i;
                            break;
                        }
                    }
                    if (skip==-1) ghost(ERR_UNTERMINATED_STRING);
                break;
                case 0:     
                    printf("%c",c);
                    exit(ERR_UNKNOWN_SQL);
                    break;
                default:
                    new_token=sub_str_cpy(sanitized_sql,i,skip+1);
                    break;
            }//end switch

            //we are not addng whitespace stuffs
            if (t!= TOKEN_WHITESPACE && 
                t!=TOKEN_LINE_COMMENT && 
                t!=TOKEN_BLOCK_COMMENT && 
                t!=TOKEN_NEW_LINE &&
                t!=TOKEN_TAB) {
                token_t *token=malloc(sizeof(token_t));
                token->type=t;
                token->value=new_token;
                token_push(tokens,token);
            }
                
            i+=skip;
        }//end new word
         //char *token=get_token();
         //gabble("shred",token);
    }
    return tokens;

}


void consolidate_tokens(token_array_t *tokens){
    token_t d;
    token_t e;
    int buffer_len;
    for(int i=0;i<tokens->length;i++){
        buffer_len=tokens->length-i;

        // starts with a dot... REAL        
        if (buffer_len>2 && tokens->array[i].type==TOKEN_DOT && tokens->array[i+1].type==TOKEN_NUMERIC){
            tokens->array[i].type=TOKEN_REAL;
            tokens->array[i].value=strcat(tokens->array[i].value,tokens->array[i+1].value);
            token_delete(tokens,i+1);
            i+=1;
            continue;
        }
        // starts with a numeric dot numeric ... REAL        
        if (buffer_len>3 &&  tokens->array[i].type==TOKEN_NUMERIC && tokens->array[i+1].type==TOKEN_DOT && tokens->array[i+2].type==TOKEN_NUMERIC){
            tokens->array[i].type=TOKEN_REAL;
            tokens->array[i].value=strcat(tokens->array[i].value,tokens->array[i+1].value);
            tokens->array[i].value=strcat(tokens->array[i].value,tokens->array[i+2].value);
            token_delete(tokens,i+1);
            token_delete(tokens,i+1);
            i+=2;
        }
    }
                  
    token_print(tokens);

    process_queries(tokens);
}

void process_queries(token_array_t *tokens){
    int loop=1;
    while(loop){

        expr_select(tokens,1);
        select_debug(tokens->select);
        if(!compare_token(tokens,0,TOKEN_DELIMITER)){
            loop=0;
        } 
    }
    if(tokens->position<tokens->top){
        printf("Limit:%d\n",tokens->top);

        printf ("error: unknown text at position :%d %s >>> %s  <<< \n",tokens->position,token_type(tokens->array[tokens->position].type),tokens->array[tokens->position].value);
        token_print(tokens);
    }
}


void print_token_range(token_array_t *tokens,char * prefix,int start,int end){
    for(int i=start;i<end;i++) {
        gabble(prefix,tokens->array[i].value);
    }
}



