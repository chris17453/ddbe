#include "../../../include/errors.h"
#include "../../../include/tokens.h"
#include "../../../include/structure.h"

// alpha or alpha.alpha
int expr_group_by(token_array_t *tokens,int depth,select_t* sel){
    ++depth;
    #ifdef PARSE_ENTRANCE
    goop(depth,"group_by","IN");
    #endif
    int pos=tokens->position;
    
    if(compare_token(tokens,0,TOKEN_GROUP_BY)){
            int looper=1;
            int expr_list=0;
            int index=0;
            tokens->object=0;
            while(looper){
                if(!expr_identifier(tokens,depth)){
                    //no select epression
                    break;
                }

                add_group_column(sel);
                ++index;
                sel->group[sel->group_length-1].identity=tokens->object;
                sel->group[sel->group_length-1].ordinal=index;

                ++expr_list;

                // if we have a delimiter.. loop else exit
                if(!compare_token(tokens,0,TOKEN_LIST_DELIMITER)) {
                    looper=0;
                } else {
                }
            } //end looper
            
            if(expr_list>0) {
                return 1;
            }
    } //end order by        
    tokens->position=pos;
    return 0;
}
