#include "heapMgr.c"

int main(void) {
    init();
    printf("lo: %p\n", lo);
    printf("hi: %p\n", hi);
    printf("root: %p\n", freeListRoot);
    printf("curr: %p\n", usedCurrent);

    char* a = (char*)malloc(sizeof(char));
    char* b = (char*)malloc(sizeof(char));
    
    printf("%p %p\n", a, b);

    return 0;
}
