//test
//Vikasni Kalahasthi 78601545 ; 
// Thi Thuy Trang Tran 74889299
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h> 

#define MAX_LINE 80
#define MAX_HEAP_SIZE 127  

// Global variable Heap
// Defined as an unsigned char array since we need to do bitwise operations 
// to get unsigned integers (info of the block's size and a-flag)
unsigned char heap[MAX_HEAP_SIZE];   // a heap with 127 bytes long

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


void bfmalloc(char* size) { //chooses block with fewest bytes left over
    //keep track of current minimum of size - free_block_size
    //if the free block is bigger than the size, then subtract and compare it to the smallest
    //once you find the smallest, add block there and update the header/footers of the blocks

    //do something
}

void freeMemory(char* index){
    // Assuming the index is at the correct address of the allocated block 
    int header = atoi(index) - 1;
    int size = heap[header] >> 1;   // size of needed-to-be-freed block
    int footer = header + size - 1;
    int prevFooter = header - 1;
    // size and status
    int a_prev = heap[prevFooter] & 1;
    int size_prev = heap[prevFooter] >> 1;
    int nextHeader = footer + 1;
    int a_next = heap[nextHeader] & 1;
    int size_next = heap[nextHeader] >> 1;

    // Coalescing with next block if free and not exceed end of Heap
    if (nextHeader < MAX_HEAP_SIZE && a_next == 0) { 
        footer = nextHeader + size_next - 1;
    }
    // Coalescing with the previous block if free and not exceed the beginning of Heap
    if( size_prev > 2 && a_prev  == 0) { //if prev block is free 
        header = prevFooter - size_prev + 1; //prev footer - the size of previous block + 1 
    }

    int size = footer - header + 1;
    // free the whole coalesced block by setting the block back to all 0
    //from header to footer should all be 0
    for(int i = header; i <= footer; i ++){
        heap[i] = 0;
    }
    // 

    
    // Set freed size to header and footer with a-flag = 0
    heap[header] = size << 1;
    heap[footer] = size << 1;
    

}

void blocklist(){ //note: sort in order of payload size so this will need to be restructured- if payload sizes are equal, sort by lower address number
    

        int header = 0;
        char status[MAX_LINE];
        
        while(header < MAX_HEAP_SIZE){
           
           int a_flag = heap[header] & 1; //allocation status 0 = free 1 = allocated
           if(a_flag == 0){
            strcpy(status, "free");
           }
           else{
            strcpy(status, "allocated");
           }
           int block_size = (heap[header] >> 1); 
           int payload = block_size - 2;
           int start = header + 1;
           int footer = header + block_size - 1;
           header = footer + 1;
           printf("%d-%d-%s\n",payload,start,status);
        }
           //wdym the terminal looks fine to me? huh that's weird i can open a new one
           //yes, but it's not in order since it's supposed to be sorted 
           //  why? the list is supposed to be in order of payload size b
           //ut the last one won't always be the biggest payload size no? 
           //ye need to either insert into an array in order or sort an array afterwards aka a pain in the ass lol 
           //decided to move on to something else cuz i'll deal with that tmr  i thought maybe a new struct or new array 
           //Yeah that would be best probs 
        // a problem for tmr lol xD
           
           
    
}

void writemem(char* c_index, char* str){
   int index = atoi(c_index);
   for( int i = index; i < index + strlen(str); i ++){ //loop thorugh heap
        int str_indx = i - index;
        heap[i] = str[str_indx]; //assign heap index to each char of string
   }
   
}

void printmem(char* c_index, char* c_number){ //we don't have to worry about how this method can destroy the entire heap thankfully lol
        int index = atoi(c_index); //change to ints
        int number = atoi(c_number);
        for(int i = index; i < index + number; i++){ //loop through heap
            if(i == index + number - 1){
                printf("%d\n",heap[i]); //formatting
            }
            else{
                printf("%d-",heap[i]);
            }
        }
    
    
}

/*int isAllocatedBlock(int block) {
    int a_flag = block & 1;     // get the a-flag of the block.
                                // last bit 1 -> allocated block. else 0 -> free block
    return a_flag;
}*/

/*
int getBlockSize( int block){ //get the size of the block
    int size = 
   
*/

void ffmalloc(char* size) {    // FIRST-FIT algorithm
    // Iterate through the whole heap to find first block that fits from the start
    // since we need to find the first block in ascending order
    // Do splitting when two blocks of sizes not smaller than 3
    // otherwise just take the whole block
    int newSize = atoi(size) + 2;   // number of needed allocations + header and footer
    int header = 0;     // starting at first block
    while (header < MAX_HEAP_SIZE) {    // iterate until reaches end of heap
        int oldSize = heap[header] >> 1;    // size of block = first 7-bit
        int remaining = oldSize - newSize;  // size of the remaining blocks when added blocks occupied the old blocks
        int a = heap[header] & 1;   // get the a-flag. last bit = 1 -> allocated block. else 0 -> free block
        if (a == 0 && oldSize >= newSize) {    // if block is free and its size is greater than the new size that
                                                                // needed to be allocated
            // Content of block: header + payload + footer
            int footer = header + newSize - 1;  // update footer's block
            // set a-flag = 1 and size for the newly added block
            heap[header] = newSize << 1;    
            heap[footer] = newSize << 1;
            heap[header] = heap[header] | 1;
            heap[footer] = heap[footer] | 1;
            // set next header and footer
            int nextHeader = footer + 1;
            int nextFooter = nextHeader + remaining - 1;
            // if we reach end of Heap. Return since no need to do splitting
            if (nextHeader >= MAX_HEAP_SIZE) {
                printf("%d\n", header + 1); // still need to print out address of the allocation right at end of heap
                printf("Reached end of heap\n");
                return;
            }
            // if the remaining free space is less than 3-bytes 
            // take the whole block
            if (remaining < 3) {
                heap[header] = oldSize << 1;
                heap[nextFooter] = oldSize << 1;
                heap[header] = heap[header] | 1;
                heap[nextFooter] = heap[header] | 1;
            }
            else {  // splitting the old block into two blocks
                    // the remaining block is free. set a-flag = 0 by shifting its size to left
                    // they already had their positions only need to set their new sizes
                heap[nextHeader] = remaining << 1;
                heap[nextFooter] = remaining << 1;
            }
            // print the first address of newly allocated block
            printf("%d\n", header + 1);
            return;
        }
        else {
            // iterate to next header since current one is either allocated or not enough space
            header += heap[header] >> 1;
        }
    }
    return;
}
                      

void eval(char **argv, int argc, enum ALGORITHM algo){

    if (strcmp(argv[0],"malloc") == 0){
        if (algo == FIRSTFIT) {
            ffmalloc(argv[1]);  // add new space in heap using first fit algo
        }
        else {                  
            bfmalloc(argv[1]);  // add new space in heap using best fit algo
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
    // unsigned char heap[MAX_HEAP_SIZE] = {0};

    // Initialize header and footer at first stage when heap is empty
    // both have size 127 at first with free a-flag. Shift-left << 1 -> LSB = 0
    // 11111110 
    heap[0] = (MAX_HEAP_SIZE << 1);                 // header
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
        eval(u_argv,u_argc,algo);
    }
    return 0;
}