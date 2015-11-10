#include "heapMgr.h"

header* header_ptr(void* ptr) {
    return (header*)ptr - 1;
}

// int validate_ptr(void* ptr) {
//     header* temp = (header*)ptr - 1;

//     if( temp->payload_ptr == ptr ) {
//         return 1;
//     }
//     return 0;
// }

header* validate_ptr(void* ptr) {
    usedCurrent = usedListRoot;

    printf("validating...\n"); //remove

    while(usedCurrent != NULL) {
        printf("%li\n", usedCurrent->size);
        if ( ptr == usedCurrent->payload_ptr) {
            return usedCurrent;
        }
        usedPrev = usedCurrent;
        usedCurrent = usedCurrent->next;
    }
    return NULL;
}

void* allocate(size_t size) {
    header* temp;

    if ((char*)unalloc + headerSize + size >(char*)hi) {//check upper boundary
        return NULL;
        //set error
    }

    //set header values, block is now 'used'
    temp = (header*)unalloc;
    temp->size = size;
    temp->isFree = 0;
    temp->payload_ptr = temp +  1;
    temp->next = NULL;
    unalloc = (char*)temp->payload_ptr + size;
    
    //update used list
    if ( usedListRoot == NULL ) { //on first mymalloc call set usedlist root
        usedListRoot = temp;
        usedCurrent = temp;
    }
    usedCurrent->next = temp;
    usedCurrent = temp;

    return temp->payload_ptr; //return pointer to payload area
}

header* freeBlockLookup(size_t size) {
    header* temp;
    
    //check if freelist exists
    if ( freeListRoot == NULL) {
        return NULL;
    }
    
    temp = freeListRoot;
    while (temp && temp->size < size) { //first fit
        temp = temp->next;
    }
    //splitblock();
    temp->size = 
    
    return temp;
}

// void display(header* h) {
//     printf("\nheader: size %li, free %d, next %p\n", h->size, h->isFree, h->next);
// }

void init(void) {
    lo = malloc(MALLOC_SIZE);
    hi = (char*)lo + MALLOC_SIZE;
    unalloc = (header*)lo;
}

void* mymalloc(size_t size) {
    // place temp pointer here?
    header* temp;

    if ( size <= 0) {
        return NULL;
        //set error
    }

    temp = freeBlockLookup(size); // O(N)
    if( temp == NULL ) {
        temp = allocate(size); // O(1)-new chunk starting off "unalloc" address
        if( temp == NULL ) {
            return NULL;
            //set error
        }
    }

    return temp->payload_ptr;
}

// void myfree(void* ptr) {
//     if( ptr == NULL) {
//         return;
//         //set error message
//     }

//     usedCurrent = validate_ptr(ptr);
//     if(usedCurrent == NULL) { //check if the ptr was malloc'ed/realloc'ed
//         return NULL;
//         //set error message
//     }
//     usedCurrent->isFree = 1;
//     usedPrev->next = usedCurrent->next;
//     freeCurrent->next = usedCurrent;
//     usedCurrent = freeCurrent;
// }

// void* myrealloc(void* ptr, size_t newSize) {
//     assert(newSize >= 0);

//     if ( newSize == 0 ) {
//         myfree(ptr);
//     }
//     if (ptr == NULL) {
//         if (newSize > 0) {
//             return mymalloc(newSize);
//         }
//         return NULL; //??
//     }
// }
