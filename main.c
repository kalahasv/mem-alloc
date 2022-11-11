// Vikasni Kalahasthi 78601545 & Thi Thuy Trang Tran 74889299
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
    if( size_prev > 2 && a_prev == 0) { //if prev block is free 
        header = prevFooter - size_prev + 1; //prev footer - the size of previous block + 1 
    }
    int newSize = footer - header + 1;  // in case size changes due to coalesces
    // free the whole coalesced block by setting the block back to all 0
    // from header to footer should all be 0
    for(int i = header; i <= footer; i ++){ 
        heap[i] = 0;
    }
    // Set freed size to header and footer with a-flag = 0
    heap[header] = newSize << 1;
    heap[footer] = newSize << 1;

}


void blocklist(){ // note: sort in order of payload size so this will need to be restructured- if payload sizes are equal, sort by lower address number
        
        struct Block { 
            int payload, start;
            char status[MAX_LINE];
        };
        struct Block blockList[MAX_HEAP_SIZE];
        int header = 0;
        char status[MAX_LINE];
        int current = 0;
        struct Block temporary;
        
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

            
           struct Block newBlock;
           newBlock.payload = payload;
           newBlock.start = start;
           strcpy(newBlock.status,status);
           
           blockList[current] = newBlock;
           current++;
        }
        for (int i = 0; i < current; ++i){ // standard sorting alg 
            for (int j = i + 1; j < current; ++j){
                if (blockList[i].payload < blockList[j].payload){
                    temporary = blockList[i];
                    blockList[i] = blockList[j];
                    blockList[j] = temporary;
                }
                else if (blockList[i].payload == blockList[j].payload){
                    if (blockList[i].start > blockList[j].start){
                        temporary = blockList[i];
                        blockList[i] = blockList[j];
                        blockList[j] = temporary;
                    }
                }
            }
        }
       for(int i = 0; i < current; i++){
             printf("%d-%d-%s\n",blockList[i].payload,blockList[i].start,blockList[i].status);
       }
}

void writemem(char* c_index, char* str){    // no bounds-checking. can cause corruptions to heap structure
   int index = atoi(c_index);
   for( int i = index; i < index + strlen(str); i ++){ //loop through heap
        int str_indx = i - index;
        heap[i] = str[str_indx];    // assign heap index to each char of string
   }
   
}

void printmem(char* c_index, char* c_number){ // we don't have to worry about how this method can destroy the entire heap thankfully lol
        int index = atoi(c_index);              // change to int. position of address
        int number = atoi(c_number);            // number of addresses needed to print
        for(int i = index; i < index + number; i++){ //loop through heap
            if(i == index + number - 1){
                printf("%d\n", heap[i]);         // formatting
            }
            else{
                printf("%d-", heap[i]);
            }
        }
    
    
}

void addMemory(int header, int newSize, int oldSize) {
    // size of the remaining blocks when added blocks occupied the old blocks
    int remaining = oldSize - newSize;
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
    // if the remaining free space is less than 3-bytes take the whole block
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
}

void ffmalloc(char* size) {    // FIRST-FIT algorithm
    // Iterate through the whole heap to find first block that fits from the start
    // since we need to find the first block in ascending order
    // Do splitting when two blocks of sizes not smaller than 3
    // otherwise just take the whole block
    int newSize = atoi(size) + 2;   // number of needed allocation + header and footer
    int header = 0;                 // starting at first block
    while (header < MAX_HEAP_SIZE) {        // iterate until reaches end of heap
        int oldSize = heap[header] >> 1;    // size of block = first 7-bit
        int a_flag = heap[header] & 1;      // get the a-flag. last bit = 1 -> allocated block. else 0 -> free block
        if (a_flag == 0 && oldSize >= newSize) {    // if block is free and its size is greater than the new size that need-to-be allocated
            // Do the add and return
            addMemory(header, newSize, oldSize);
            return;
        }
        else {
            // iterate to next header since current one is either allocated or not enough space
            // jump to next header by size step
            header += heap[header] >> 1;
        }
    }
    return;
}

void bfmalloc(char* size) {     // BEST-FIT algorithm
    // Iterating through the whole heap to find minimum-bytes block that fits the new size
    // if the min block is found, send its header to addMemory() to do the add and maybe split there
    // if not, inform that there is not enough space in heap
    int min_position = 0;           // start at the beginning of heap
    int min_size = 99999999;        // arbitrary large size to start
    int newSize = atoi(size) + 2;   // number of needed allocations + header and footer
    int isFound = 0;

    // loop through heap to find the minimum that fits new size
    int header = 0;
    while(header < MAX_HEAP_SIZE){
        int a_flag = heap[header] & 1;          // allocation status 0 = free, 1 = allocated
        int block_size = (heap[header] >> 1);   // size of block in order to do comparison 
        int footer = header + block_size - 1;
        
        if(block_size < min_size && block_size >= newSize && a_flag == 0){ // block is smaller than current min and can fit new block
            min_position = header;  // update min position and min size
            min_size = block_size;
            isFound = 1;            // set isFound to 1 since we have found the block
        }
        header = footer + 1;        // jump to next header in heap
    }
    // if min block is found -> Add memory else return without doing anything
    if (isFound == 1) {
        header = min_position;  // set min block back to header 
        int oldSize = min_size;
        addMemory(header, newSize, oldSize);
    }
    else {
        printf("There is not enough space in heap\n");
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

    // Since we declare our heap as global variable and global variable is automatically initialized with 0
    // We might not need to an intialization for local but i still put it here just in case
    // unsigned char heap[MAX_HEAP_SIZE] = {0};

    // Initialize header and footer at first stage when heap is empty
    // both have size 127 at first with free a-flag. Shift-left << 1 -> LSB = 0 
    // 11111110 = size 127 with a_flag = 0
    heap[0] = (MAX_HEAP_SIZE << 1);                 // header
    heap[MAX_HEAP_SIZE - 1] = MAX_HEAP_SIZE << 1;   // footer

    char input[MAX_LINE];               //user input
    int u_argc = 0;                     //user entered: number of arguments
    char* u_argv[MAX_LINE];             //user entered: number of arguments
    enum  ALGORITHM algo = FIRSTFIT;    //default memory allocation algorithm 

    if(argc == 2){
        if(strcmp(argv[1],"BestFit") == 0){
            algo = BESTFIT;
        }
    }

    while(1) { // take user input 
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
