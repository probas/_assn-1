#include "heapMgrMalloc.h"

header* header_ptr(void* ptr) {
    return (header*)ptr - 1;
}

int validate_ptr(void* ptr) {
    header* current = ulr;

    printf("validating...\n"); //remove
    while(current) {
        printf("%li\n", current->size);
        if ( ptr == current->payload_ptr) {
            return 1;
        }
    }
    return 0;
}

void* allocate(size_t size) {
    if ((char*)unalloc + headerSize + size > hi) {
        return NULL;
    }

    temp = (header*)unalloc;
    temp->size = size;
    temp->isFree = 0;
    temp->payload_ptr = uc +  1;
    temp->next = NULL;
    unalloc = temp->payload_ptr + size;
    
    return temp->payload_ptr;
}

void* freeLookup(size_t size) {
    if( flr != NULL ) {
        header* temp = flr;

        while (temp && temp->size < size)) {
            temp = temp->next;
        }
        uc = temp;
        return temp;
    }
}

void* splitBlock(void*) {}

void display(header* h) {

    printf("\nheader: size %li, free %d, next %p\n", h->size, h->isFree, h->next);
}

void init(void) {
    lo = malloc(MALLOC_SIZE);
    hi = (char*)lo + MALLOC_SIZE;
    unalloc = (header*)lo;
}

void* mymalloc(size_t size) {
    // place temp pointer here?
    void* result;

    if ( size <= 0) {
        return NULL;
        //set error
    }
    
    if ( ulr == NULL ) { // on first call to mymalloc
        ulr = temp;
    } else {
        uc->next = temp;
    }
    uc = temp;


    result = freeLookup(size); // O(N)
    if( result == NULL ) {
        result = allocate(size); // O(1)-new chunk starting off "unalloc" address
        if( result == NULL ) {
            return NULL;
            //set error
        }
    }
    return result;


}

void myfree(void* ptr) {
    if( ptr == NULL) { // the same as: if(!ptr)
        return; //set error message
    }

    if( !validate_ptr(ptr) ) { //check if the ptr was malloc'ed/realloc'ed
        return; //set error message
    }

    header* current = header_ptr(ptr);
    flr = current;
    current->isFree =  1;
    current->next = unalloc;//?
}

void* myrealloc(void* ptr, size_t newSize) {
    assert(newSize >= 0);

    if ( newSize == 0 ) {
        myfree(ptr);
    }
    if (ptr == NULL) {
        if (newSize > 0) {
            return mymalloc(newSize);
        }
        return NULL; //??
    }
}