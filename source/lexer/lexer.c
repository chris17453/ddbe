#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "../include/errors.h"
#include "../include/tokens.h"
#include "../include/select.h"
#include "../include/debug.h"


token_array_t *lex(char * query){
    unsigned int query_length=strlen(query);
    if (query_length<1){
        ghost(ERR_EMPTY_QUERY_STRING);

    }

    int new_word=0;
    char c,d,e;
    int skip=0;
    char t=0;
    int buffer_len=0;
    token_array_t *tokens=token_array(1000);
    char *new_token=0;

    if(1==1)

    for(int i=0;i<query_length;i++){

        buffer_len=query_length-i;
        c=query[i];

        //The start of a new token....
        
        if(new_word==0){
            if (buffer_len>2) d=query[i+1]; else  d=0;
            if (buffer_len>3) e=query[i+2]; else  e=0;
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
                        c=query[lazer];
                        if (lazer!=query_length-1 && (
                            (c>='a' && c<='z') ||   
                            (c>='A' && c<='Z') ||
                            (c=='_' ) ||
                            (c>='0' && c<='9')) ) continue;
                        else {
                            new_token=sub_str_cpy(query,i,lazer-i);
                            skip=lazer-i-1;
                            break;
                        }
                    }
                    //one off.. yea clean this up TODO: dont be a bone head
                    if(new_token) { 
                        //new_token=sub_str_cpy(query,i,query_length-i);
                        //skip=query_length-i-1;
                    
                         
                        if (0==strncasecmp(new_token,"DISTINCT"  ,8 ) ) { t=TOKEN_DISTINCT;         } else
                        if (0==strncasecmp(new_token,"UNKNOWN"   ,7 ) ) { t=TOKEN_UNKNOWN;          } else
                        if (0==strncasecmp(new_token,"SELECT"    ,6 ) ) { t=TOKEN_SELECT;           } else
                        if (0==strncasecmp(new_token,"WHERE"     ,5 ) ) { t=TOKEN_WHERE;            } else
                        if (0==strncasecmp(new_token,"ORDER"     ,5 ) ) { t=TOKEN_ORDER;            } else
                        if (0==strncasecmp(new_token,"GROUP"     ,5 ) ) { t=TOKEN_GROUP;            } else
                        if (0==strncasecmp(new_token,"LIMIT"     ,5 ) ) { t=TOKEN_LIMIT;            } else
                        if (0==strncasecmp(new_token,"OUTER"     ,5 ) ) { t=TOKEN_OUTER;            } else
                        if (0==strncasecmp(new_token,"INNER"     ,5 ) ) { t=TOKEN_INNER;            } else
                        if (0==strncasecmp(new_token,"RIGHT"     ,5 ) ) { t=TOKEN_RIGHT;            } else
                        if (0==strncasecmp(new_token,"FALSE"     ,5 ) ) { t=TOKEN_FALSE;            } else
                        if (0==strncasecmp(new_token,"LEFT"      ,4 ) ) { t=TOKEN_LEFT;             } else
                        if (0==strncasecmp(new_token,"FULL"      ,4 ) ) { t=TOKEN_FULL;             } else
                        if (0==strncasecmp(new_token,"DESC"      ,4 ) ) { t=TOKEN_DESC;             } else
                        if (0==strncasecmp(new_token,"JOIN"      ,4 ) ) { t=TOKEN_JOIN;             } else
                        if (0==strncasecmp(new_token,"LIKE"      ,4 ) ) { t=TOKEN_LIKE;             } else
                        if (0==strncasecmp(new_token,"FROM"      ,4 ) ) { t=TOKEN_FROM;             } else
                        if (0==strncasecmp(new_token,"NULL"      ,4 ) ) { t=TOKEN_NULL;             } else
                        if (0==strncasecmp(new_token,"TRUE"      ,4 ) ) { t=TOKEN_TRUE;             } else
                        if (0==strncasecmp(new_token,"AND"       ,3 ) ) { t=TOKEN_AND;              } else
                        if (0==strncasecmp(new_token,"ASC"       ,3 ) ) { t=TOKEN_ASC;              } else
                        if (0==strncasecmp(new_token,"NOT"       ,3 ) ) { t=TOKEN_NOT;              } else
                        if (0==strncasecmp(new_token,"BY"        ,2 ) ) { t=TOKEN_BY;               } else
                        if (0==strncasecmp(new_token,"AS"        ,2 ) ) { t=TOKEN_AS;               } else
                        if (0==strncasecmp(new_token,"OR"        ,2 ) ) { t=TOKEN_OR;               } else
                        if (0==strncasecmp(new_token,"ON"        ,2 ) ) { t=TOKEN_ON;               } else
                        if (0==strncasecmp(new_token,"IS"        ,2 ) ) { t=TOKEN_IS;               } else
                        if (0==strncasecmp(new_token,"IN"        ,2 ) ) { t=TOKEN_IN;               } else 
                        if (0==strncasecmp(new_token,"DELIMITER" ,9 ) ) { t=TOKEN_COLUMN_DELIMITER; } else
                        if (0==strncasecmp(new_token,"PASSWORD"  ,8 ) ) { t=TOKEN_PASSWORD;         } else
                        if (0==strncasecmp(new_token,"ACCOUNT"   ,7 ) ) { t=TOKEN_ACCOUNT;          } else
                        if (0==strncasecmp(new_token,"COMMIT"    ,6 ) ) { t=TOKEN_COMMIT;           } else
                        if (0==strncasecmp(new_token,"COLUMN"    ,6 ) ) { t=TOKEN_COLUMN;           } else
                        if (0==strncasecmp(new_token,"QUOTED"    ,6 ) ) { t=TOKEN_QUOTED;           } else
                        if (0==strncasecmp(new_token,"STRICT"    ,6 ) ) { t=TOKEN_STRICT;           } else
                        if (0==strncasecmp(new_token,"CREATE"    ,6 ) ) { t=TOKEN_CREATE;           } else
                        if (0==strncasecmp(new_token,"ARRAY"     ,5 ) ) { t=TOKEN_ARRAY_DELIMITER;  } else
                        if (0==strncasecmp(new_token,"TABLE"     ,5 ) ) { t=TOKEN_TABLE;            } else
                        if (0==strncasecmp(new_token,"FILE"      ,4 ) ) { t=TOKEN_FILE;             } else
                        if (0==strncasecmp(new_token,"FIFO"      ,4 ) ) { t=TOKEN_FIFO;             } else
                        if (0==strncasecmp(new_token,"REPO"      ,4 ) ) { t=TOKEN_REPO;             } else
                        if (0==strncasecmp(new_token,"FILE"      ,4 ) ) { t=TOKEN_FILE;             } else
                        if (0==strncasecmp(new_token,"BASE"      ,4 ) ) { t=TOKEN_BASE;             } else
                        if (0==strncasecmp(new_token,"PATH"      ,4 ) ) { t=TOKEN_PATH;             } else
                        if (0==strncasecmp(new_token,"PUSH"      ,4 ) ) { t=TOKEN_PUSH;             } else
                        if (0==strncasecmp(new_token,"PULL"      ,4 ) ) { t=TOKEN_PULL;             } else
                        if (0==strncasecmp(new_token,"READ"      ,4 ) ) { t=TOKEN_READ;             } else
                        if (0==strncasecmp(new_token,"URL"       ,3 ) ) { t=TOKEN_URL;              } 











                    }
                    break;

                case TOKEN_NUMERIC: 

                    for(int lazer=i+1;lazer<query_length;lazer++){
                        c=query[lazer];
                        if (c>='0' && c<='9') continue;
                        else {
                            new_token=sub_str_cpy(query,i,lazer-i);
                            skip=lazer-i-1;
                            break;
                        }
                    }

                    break;

                case TOKEN_HEX: 
                    for(int lazer=i+2;lazer<query_length;lazer++){
                        c=query[lazer];
                        if ((c>='0' && c<='9') || ( (c>='a' && c<='f') || (c>='A' && c<='F') ) ) continue;
                        else {
                            new_token=sub_str_cpy(query,i,lazer-i);
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
                        c=query[lazer];
                        if (c=='0' || c=='1') continue;
                        else {
                            new_token=sub_str_cpy(query,i,lazer-i);
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
                        c=query[lazer];
                        if (c<32) continue;
                        else {

                            new_token=sub_str_cpy(query,i,lazer-i);
                            skip=lazer-i-1;
                            break;
                        }
                    }
                    break;
                case TOKEN_LINE_COMMENT:
                    //is it long enough
                    skip=-1;
                    for(int lazer=i+2;lazer<query_length;lazer++){
                        c=query[lazer];
                        if (c=='\n') {
                            new_token=sub_str_cpy(query,i+2,lazer-i-2);
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
                            c=query[lazer];
                            d=query[lazer+1];
                            
                            if (c=='*' && d=='/') {;
                                new_token=sub_str_cpy(query,i+2,lazer-i-2);
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
                        if(query[lazer]==c) {
                            // it's an empty string.....
                            if (lazer-i==1) 
                                new_token="";
                            else
                                new_token=sub_str_cpy(query,i+1,lazer-i-1);
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
                    new_token=sub_str_cpy(query,i,skip+1);
                    break;
            }//end switch

            //we are not addng whitespace stuffs

            if (t!= TOKEN_WHITESPACE && 
                t!=TOKEN_LINE_COMMENT && 
                t!=TOKEN_BLOCK_COMMENT && 
                t!=TOKEN_NEW_LINE &&
                t!=TOKEN_TAB) {
                token_push(tokens,t,new_token);
                new_token=0;
            } else {
            
                if(new_token) {
                    free(new_token);
                }
                new_token=0;
            }
            
                
            i+=skip;
        }//end new word
    }
    consolidate_tokens(tokens);
    return tokens;

}


void token_combine(token_array_t *tokens,int *list){
    if (tokens==0)  return;
    if (list==0) return;

    int list_index=0;    
    int combo_token=list[1];
    int length=list[0];
    list=&list[2];
    for(int i=0;i<tokens->top;i++){
        if(tokens->array[i].type==list[list_index]) {
            ++list_index;
            if(list_index==length){
                int start_index=i-length+1;

                tokens->array[start_index].type=combo_token;
                int token_len=0;
                for(int w=0;w<length;w++) {
                    token_len+=strlen(tokens->array[start_index+w].value);
                }
                char *new_token=calloc(1,token_len+1);
                

                for(int w=0;w<length;w++){
                    strcat(new_token,tokens->array[start_index+w].value);
                }
                //free up replaced token value
                free(tokens->array[start_index].value);
                
                for(int w=1;w<length;w++){
                    token_delete(tokens,start_index+1);
                }
                tokens->array[start_index].value=new_token;
                i+=length-1;
                list_index=0;

            }
        } else {
            list_index=0;
        }
    }    
}


void consolidate_tokens(token_array_t *tokens){
    token_t d;
    token_t e;
    int buffer_len;

    //             length-2 combo token     match pattern
    int token1 [] ={3,TOKEN_FULL_OUTER_JOIN ,TOKEN_FULL          ,TOKEN_OUTER    ,TOKEN_JOIN     };
    int token2 [] ={3,TOKEN_IS_NOT_NULL     ,TOKEN_IS            ,TOKEN_NOT      ,TOKEN_NULL     };
    int token3 [] ={3,TOKEN_REAL            ,TOKEN_NUMERIC       ,TOKEN_DOT      ,TOKEN_NUMERIC  };
    int token4 [] ={2,TOKEN_NOT_IN          ,TOKEN_NOT           ,TOKEN_IN                       };
    int token5 [] ={2,TOKEN_REAL            ,TOKEN_DOT           ,TOKEN_NUMERIC                  };
    int token6 [] ={2,TOKEN_LEFT_JOIN       ,TOKEN_LEFT          ,TOKEN_JOIN                     };
    int token7 [] ={2,TOKEN_RIGHT_JOIN      ,TOKEN_RIGHT         ,TOKEN_JOIN                     };
    int token8 [] ={2,TOKEN_INNER_JOIN      ,TOKEN_INNER         ,TOKEN_JOIN                     };
    int token9 [] ={2,TOKEN_GROUP_BY        ,TOKEN_GROUP         ,TOKEN_BY                       };
    int token10[] ={2,TOKEN_ORDER_BY        ,TOKEN_ORDER         ,TOKEN_BY                       };
    int token11[] ={2,TOKEN_IS_NULL         ,TOKEN_IS            ,TOKEN_NULL                     };
    
   // starts with a dot... REAL        
    token_combine(tokens,token1);
    token_combine(tokens,token2);
    token_combine(tokens,token3);
    token_combine(tokens,token4);
    token_combine(tokens,token5);
    token_combine(tokens,token6);
    token_combine(tokens,token7);
    token_combine(tokens,token8);
    token_combine(tokens,token9);
    token_combine(tokens,token10);
    token_combine(tokens,token11);

    //alias update
    for(int i=0;i<tokens->top;i++) {
        if(tokens->array[i].type== TOKEN_AS) {
            token_delete(tokens,i);
            if(tokens->top-i>1 && tokens->array[i].type==TOKEN_ALPHA){
                tokens->array[i].type=TOKEN_ALIAS;
            }
        } 
    }


    //identity update
    int length=tokens->top;
    for(int i=0;i<tokens->top;i++) {
        if(tokens->array[i].type== TOKEN_ALPHA) {
            if(length>1 && tokens->array[i+1].type== TOKEN_DOT){
                if(length>2 && tokens->array[i+2].type== TOKEN_ALPHA){
                    tokens->array[i].type=TOKEN_QUALIFIER;
                    tokens->array[i+2].type=TOKEN_SOURCE;
                    token_delete(tokens,i+1);
                    ++i;
                    continue;
                }
            }
            tokens->array[i].type=TOKEN_SOURCE;
        } 
        length--;
    }

    // limit fixup
    for(int i=0;i<tokens->top;i++) {
        if(tokens->array[i].type== TOKEN_LIMIT) {
            token_delete(tokens,i);

            if(i+2<tokens->top && 
                tokens->array[i  ].type==TOKEN_NUMERIC &&
                tokens->array[i+1].type==TOKEN_LIST_DELIMITER && 
                tokens->array[i+2].type==TOKEN_NUMERIC
                ) {
                    tokens->array[i  ].type=TOKEN_LIMIT_START;
                    tokens->array[i+2].type=TOKEN_LIMIT_LENGTH;
                    token_delete(tokens,i+1);
                    ++i;
                    continue;
                }
            if( tokens->array[i].type==TOKEN_NUMERIC){
                tokens->array[i].type=TOKEN_LIMIT_LENGTH;
            }
        }
    }//end limit fixup
      

}//end funciton

void process_queries(token_array_t *tokens){
    int loop=1;
    while(loop){
        select_t *select=process_select(tokens,&tokens->position);

        free_select(select);

        if(!compare_token(tokens,0,TOKEN_DELIMITER)){
            loop=0;
        } 
        printf("\nToken Count:%d of %d\n",tokens->position,tokens->top);
    }
    
    if(tokens->position<tokens->top){

        printf ("error: unknown text at position :%d %s >>> %s  <<< \n",tokens->position,token_type(tokens->array[tokens->position].type),tokens->array[tokens->position].value);
        token_print(tokens);
    }
}


void print_token_range(token_array_t *tokens,char * prefix,int start,int end){
    for(int i=start;i<end;i++) {
        gabble(prefix,tokens->array[i].value);
    }
}



