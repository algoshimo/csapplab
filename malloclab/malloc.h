#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>

typedef char ALIGN[16];
union header  
{
    struct
    {
        size_t size;
        unsigned is_free;
        struct header_t * next;
    } s;
    ALIGN stub;  //保证16字节对齐(union大小为最大成员的字节大小)
}
typedef union header header_t;


header_t *head, *tail;
pthread_mutex_t global_malloc_lock;


void *malloc(size_t size)
{
    size_t total_size;
}

void *free(void *p)
{
    
}