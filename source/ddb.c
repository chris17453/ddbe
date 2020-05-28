#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "include/teleprompter.h"
#include "include/warp_gate.h"
#include "include/lexer.h"
#include "include/debug.h"
#include <unistd.h>  


void janitor(){
  gabble("EXIT","CLEANUP");
}


int main(int argc, char* argv[]) {


    int opt; 
  
    int opt_print_tokens=0;
    int opt_debug=0;
    while((opt = getopt(argc, argv, ":if:lrx")) != -1) {  
        switch(opt)  {  
            case 't':  opt_print_tokens=1;  break;
            case 'd':  opt_debug=1;  break;
        }  
    }  


    char *query_str;
    // if not a terminal grab from pipe
    if (!isatty(0)) {   
      query_str=get_stdin();
    } else {  //args at this point
      if (argc==1){
        show_usage();
        return 0;
      }
      query_str=read_file(argv[1]);
    }



    token_array_t *tokens=lex(query_str);
    free(query_str);
    if(opt_print_tokens) token_print(tokens);
    if(opt_debug) debug(tokens);

    process_queries(tokens);
    
    
    tokens_destroy(tokens);
    atexit(janitor);
    return 0;
}


/****
 * F5                for debug
 * CTRL + SHIFT + B  to build 
 * vscode requires launch.json andtask.json to be setup for this to work.
 * sofar the project is manually created as a command to gcc to build...
 ****/