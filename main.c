//test
//Vikasni Kalahasthi 78601545 ; 
// Thi Thuy Trang Tran 74889299
#include <stdlib.h>
#include<string.h>
#include<stdio.h>
#include <stdint.h> 

#define MAX_LINE 80
#define MAX_HEAP_SIZE 127

//char *heap; //pointer to first block of heap 
char heap[MAX_HEAP_SIZE];   // a heap with 127 bytes long

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


void bfmalloc(char* size) {
    //do something
}

void freeMemory(char* index){
    //do something
}

void blocklist(){
    //do something
}

void writemem(char* index, char* str){
    //do something
}

void printmem(char* index, char* num_chars){
    //do something
}

/*int isAllocatedBlock(char* block) {
    int a_flag = block & 1;     // get the a-flag of the block.
                                // last bit 1 -> allocated block. else 0 -> free block
    return a_flag;
}*/

void ffmalloc(char* size) {    // FIRST-FIT algorithm
    // Iterate through the whole heap to find first block that fits from the start
    // since we need to find the first block in ascending order
    // Do splitting when two blocks of sizes not smaller than 3
    // otherwise just take the whole block
    int newSize = atoi(size) + 2;   // number of needed allocations + header and footer
    int header = 0;     // starting at first block
    while (header < MAX_HEAP_SIZE) {    // iterate until reaches end of heap
        int oldSize = heap[header] >> 1;    // size of block = first 7-bit
        int remaining = oldSize - newSize; 
        if ((heap[header] != 1) && oldSize >= newSize) {    // if block is free and its size is greater than the new size that
                                                                // needed to be allocated
            // Content of block: header + payload + footer
            int footer = header + newSize - 1;  // update footer's block
            printf("footer after added %d is now at: %d", newSize, footer);
            // set a-flag = 1 and size for the newly added block
            heap[header] = newSize << 1;    
            heap[footer] = newSize << 1;
            heap[header] = heap[header] | 1;
            heap[footer] = heap[footer] | 1;
            // set next header and footer
            int nextHeader = footer + 1;
            int nextFooter = nextHeader + remaining - 1;
            // if the remaining free space is less than 3-bytes 
            // take the whole block
            if (remaining < 3) {
                heap[header] = oldSize << 1;
                heap[nextFooter] = oldSize << 1;
                heap[header] = heap[header] | 1;
                heap[nextFooter] = heap[header] | 1;
            }
            else {  // splitting the old block into two blocks
                    // the remaing block is free. a-flag = 0 when shift its size to left
                    // they already had their positions only need to set their new sizes
                heap[nextHeader] = remaining << 1;
                heap[nextFooter] = remaining << 1;
            }
            // print the first address of newly allocated block
            printf("%d\n", header + 1);
        }
        else {
            // iterate to next header since current one is either allocated or not enough space
            header += heap[header] >> 1;
        }

    }


}
                      

void eval(char **argv, int argc, enum ALGORITHM algo){
    
    printf("in eval\n");
    if (strcmp(argv[0],"malloc") == 0){
        if (algo == FIRSTFIT) {
            ffmalloc(argv[1]);  // add new space using first fit algo
            printf("ffmalloc starts\n");
        }
        else {                  
            bfmalloc(argv[1]);  // add new space using best fit algo
        }  
    }
    else if (strcmp(argv[0],"free") == 0){
        freeMemory(argv[1]);
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

    //heap = (int*) malloc(128 * sizeof(int)); //original malloc of heap 
    // we don't need to use dynamic allocation for this
    // we only make a heap simulation 

    // Since we declare our heap as global variable and global variable is automatically initialized with 0
    // We might not need to an intialization for local but i still put it here just in case
    // char heap[MAX_HEAP_SIZE] = {0};

    // Initialize header and footer at first stage when heap is empty
    // both have size 127 at first with free a-flag. Shift-left << 1 -> LSB = 0
    // 11111110 
    heap[0] = MAX_HEAP_SIZE << 1;                   // header
    heap[MAX_HEAP_SIZE - 1] = MAX_HEAP_SIZE << 1;   // footer

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
        printf("about to go in eval\n");
        printf("%s\n", u_argv[0]);
        eval(u_argv,u_argc,algo);
    }
    return 0;
}
