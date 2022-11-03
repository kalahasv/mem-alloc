//test
//Vikasni Kalahasthi 78601545 ; 
#include <stdlib.h>
#include<string.h>
#include<stdio.h>
#include <stdint.h> 

#define MAX_LINE 80

char *heap; //pointer to first block of heap 

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

void mmalloc(int size){
    //do something 
}

void free(int index){
    //do something
}

void blocklist(){
    //do something
}

void writemem(int index, char* str){
    //do something
}

void printmem(int index, int num_chars){
    //do something
}
                      

void eval(char **argv, int argc, enum ALGORITHM algo){

    if (strcmp(argv[0],"malloc") == 0){
        mmalloc(argv[1]);
        
    }
    else if (strcmp(argv[0],"free") == 0){
        free(argv[1]);
    }
    else if(strcmp(argv[0],"blocklist") == 0){ 
        blocklist();
    }
    else if(strcmp(argv[0],"writemem") == 0){ 
        writemem(argv[1],argv[2]);
    }
    else if(strcmp(argv[0],"printmem") == 0){ 
        printmem(argv[1],argv[2]);
    }
    else if (strcmp(argv[0],"quit") == 0){
        exit(0);
    }


}



int main(int argc, char* argv[]){

    heap = (int*) malloc(128 * sizeof(int)); //original malloc of heap 
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