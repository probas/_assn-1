/*
    Heap Manager that user malloc to get a chunk on initial memory
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h> //malloc

#define MALLOC_SIZE 128

enum mode {FIRST_FIT=1, NEXT_FIT, BEST_FIT, WORST_FIT, FIXED_SIZE_ALLOCATION};

typedef struct header header;//no service area, header - 16 bytes
struct header {
    size_t size;
    int isFree; //may not be needed - remove
    void* payload_ptr;
    header* next;
}; //todo: try to reduce header size using first bit of size for storing free/busy

/*variables*/
void* lo = NULL; //lower heap address
void* hi = NULL; //higher heap address
void* unalloc = NULL; //pointer to unallocated heap space;
const size_t headerSize = sizeof(header);

/*Below pointers constitute service portion of the heap, but in fact they are located in Initialized Data Segment*/
header* flr = NULL; //freelist root
header* ulr = NULL; //usedlist root. Used by myfree function
header* fc = NULL; //pointer current free block
header* uc = NULL; //pointer to current used block
header* temp = NULL; //temporary

/*Aux functions*/
header* header_ptr(void*);
void* allocate(size_t);//
int validate_ptr(void*);//check if the ptr was previously allocated by mymalloc/myrealloc 
void* freeBlockLookup(size_t);//traverse free list to find one of the fits
void* splitBlock(void);
void display(header* h);
void init(void); //prepares heap for use;

/*Functions to implement*/
void* mymalloc(size_t); //todo: errorchecking if there is block big enough
void* myrealloc(void*, size_t);
// void *calloc(size_t nmemb, size_t size); //?
void myfree(void*);//todo: errorchecking if the ptr was previously allocated by mymalloc/myrealloc - done
void* mymemcpy(void*, void*, size_t); //todo: error checking 
void* mymemmove(void*, void*, size_t); //todo:errorchecking if the areas overlap
void mymap(void); //todo: visualize info, gather totals of free and used and calculate stats
