/*
******************************************************
What constitutes a good malloc impl.?

1. Time - Minimize time taken for allocation and free
2. Space - Minimize fragmentation
3. Locality - Allocations around time t are stored near each other
4. Similar sized blocks are stored near by
5. Safe (can only free malloc'ed or realloc'ed pointers)

To do the best job of space utilization, operations must take more time. Trick is to balance these two conflicting goals.

Drag the slider:)
Performance <---------*--------------> Fragmentation

* give implementation ideas for either model (perf-model or mem-model)

dlmalloc – General purpose allocator
ptmalloc2 – glibc
jemalloc – FreeBSD and Firefox
tcmalloc – Google
libumem – Sola

Problematic:
malloc is a library call provided by the standard c library shipped with Unix-like operating systems. Most modern implementations use mmap MAP_ANONYMOUS to allocate(actually reserve) virtual address space for the calling process. This memory is managed by malloc implementation. Most malloc implementations use some kind of tree data structure (Cartesian tree, RB tree etc) to store the free blocks. Metadata is either stored at a fixed offset from the virtual address returned to the calling application(HP-UX implementation) or stored separately elsewhere.  Malloc grows the address space of a process(heap) when the current request can not be satisfied by its maintained free pool (either due to fragmentation or the request is too big) by calling mmap again. Advantages of using mmap over sbrk/brk are obvious. With mmap one can shrink the heap size by mumapping the contiguous unused pages in free list.

*****************************************************
Implementation Issues
 How do we know how much memory to free just given a
pointer? - keep size in header
 How do we keep track of the free blocks? - linked list
 What do we do with the extra space when allocating a memory
block that is smaller than the free block it is placed in?
 How do we pick which free block to use for allocation?

My Strategy:
1. Performance accent:

2. Service part: Private block headers representing heap metadata and four pointers of type "header" located in the heap.

3. One call of original malloc is used to allocate one big chunk

4. Functions mymalloc, myrealloc, myfree designed to work for all possible inputs.

5. Coalescing policy:
Immediate coalescing: Coalesce each time free is called
Deferred coalescing: Improve free performance by deferring coalescing until needed.
Examples:
● Coalesce while scanning the free list for malloc()
● Coalesce when the amount of external fragmentation reaches some threshold.

My Features:
1. Coalescing when freeing memory or searching for appropriate sized free block. Odd or even call to traverse the free/used list must trigger the coalescing function that merges free blocks.

The memory management code must obey the following constraints:
1. Can’t control number or size of requested blocks
2. Must respond immediately to all allocation requests, i.e. can’t reorder or buffer requests (concurrency?)
3. Must allocate blocks from free memory, i.e. can only place allocated blocks in free memory
4. Must align blocks so they satisfy all alignment requirements 8 byte alignment for GNU malloc (libc malloc) on Linux boxes
5. Can only manipulate and modify free memory
6. Can’t move the allocated blocks once they are allocated, i.e. compaction is not allowed???
Do not create extra linked lists outside of the provided private heap;
7. Errors 
perror("malloc");
exit(0);

Header:
size_t size - size of allocated or free space
int free: ==0 free, != 0 used
void* ptr - pointer to newly allocated space
header* next

void* mymalloc(size_t size) - The memory is not initialized.
    If successful: Returns a pointer to a memory block of at least size bytes.
    If size == 0, returns NULL
    If unsuccessful: returns NULL.

void free(void *p) - 
    Returns the block pointed at by p to pool of available memory
    The pointer p must come from a previous call to malloc or realloc, ie free requests must correspond to an previously allocated block


void* realloc(void *p, size_t newSize)
    Changes size of block p and returns pointer to new block.
    Contents of new block unchanged up to min of old and new size.

    The realloc() function changes the size of the memory block pointed
       to by ptr to newSize bytes.  The contents will be unchanged in the range
       from the start of the region up to the minimum of the old and new
       sizes.  If the new size is larger than the old size, the added memory
       will not be initialized.  If ptr is NULL, then the call is equivalent
       to malloc(size), for all values of size; if size is equal to zero,
       and ptr is not NULL, then the call is equivalent to free(ptr).
       Unless ptr is NULL, it must have been returned by an earlier call to
       malloc(), calloc() or realloc().  If the area pointed to was moved, a
       free(ptr) is done.

    If newSize is bigger than the size of chunk in reallocThis:
        It should first try to allocate a chunk of size newSize in place so that new chunk's base pointer also is reallocThis;
        If there is no free space available to do in place allocation, it should allocate a chunk of requested size in a different region; and then it should copy the contents from the previous chunk.
    If the function failed to allocate the requested block of memory, a NULL pointer is returned, and the memory block pointed to by argument reallocThis is left unchanged.
    If newSize is smaller, realloc should shrink the size of the chunk and should always succeed.
    If newSize is 0, it should work like free.
    If reallocThis is NULL, it should work like malloc.
    If reallocThis is pointer that was already freed, then it should fail gracefully by returning NULL
myFree should not crash when it is passed a pointer that has already been freed.
*/