//Vikasni Kalahasthi 78601545 ; 
#include <stdlib.h>
#include<string.h>
#include<stdio.h>

#define MAX_LINE 80

enum ALGORITHM {
    FIRSTFIT, 
    BESTFIT
};

void distributeInput(char* input, int* argc, char** argv) { //distributes input into argc & argv
    char* token;        
    const char* delims = " \t\n";
    token = strtok(input, delims);      // first token is the command
    while (token != NULL) {             // getting next arguments in to argv
        argv[(*argc)++] = token;
        token = strtok(NULL, delims);
    }
}

int findFirstFree(){
    /*
    p = start; 
    while ((p < end) &&     \\ not passed end
    ((*p & 1) ||     \\ already allocated
    (*p <= len)))    \\ too small 
    p = p + (*p & -2);    \\ goto next block (word addressed) 
    */  
}

void eval(char **argv, int argc, enum ALGORITHM algo){

    if (strcmp(argv[0],"malloc") == 0){
        //do something 
        
    }
    else if (strcmp(argv[0],"free") == 0){
        //do something
    }
    else if(strcmp(argv[0],"blocklist") == 0){ 
        //do something
    }
    else if(strcmp(argv[0],"writemem") == 0){ 
        //do something
    }
    else if(strcmp(argv[0],"printmem") == 0){ 
        //do something
    }
    else if (strcmp(argv[0],"quit") == 0){
        exit(0);
    }


}

int main(int argc, char* argv[]){
    char input[MAX_LINE]; //user input
    int u_argc = 0;//user entered: number of arguments
    char* u_argv[MAX_LINE]; //user entered: number of arguments
    enum  ALGORITHM algo = FIRSTFIT; //default memory allocation algorithm 

    if(argc == 2){
        if(strcmp(argv[1],"BestFit")){
            algo = BESTFIT;
        }
    }


    while(1) { //take user input 
        fflush(stdin);
        fflush(stdout);
        u_argc = 0;
        printf(">");
        fgets(input,MAX_LINE,stdin);
        distributeInput(input,&u_argc,u_argv);
        eval(u_argv,u_argc,algo);

    }
    return 0;
}