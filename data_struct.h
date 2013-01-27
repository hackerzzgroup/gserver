#ifndef _DATASTRUCT_H_
#define _DATASTRUCT_H_

typedef enum list_error {
    NO_ERROR,
    INDEX_OUT_OF_BOUNDS,
    OUT_OF_MEMORY,
    LIST_IS_NULL,
} list_error;

typedef struct __listhandle {
    unsigned int mz;
    struct __listnode *start;
} *list_handle;

typedef struct __blisthandle {
    unsigned int mz;
    struct __listnode *start;
    char null;
} *blocking_list_handle;

typedef struct __listnode {
    struct __listnode *next;
    const void *data;
} *list_node_t;

list_error list_get_error(void);
char *list_strerror(list_error val);

list_handle list_alloc(unsigned int size, const void *starter);
void *list_index(const list_handle head, unsigned int ind);
void list_set_index(const list_handle head, unsigned int ind, const void *item);
void list_append(const list_handle head, const void *item);
void *list_pop(const list_handle head);
void list_iter(const list_handle head, int (callback)(unsigned int index, void *item));
unsigned int list_size(const list_handle head);
void list_dealloc(list_handle head);

#endif