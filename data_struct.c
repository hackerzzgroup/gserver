#include "midori.h"
#include "data_struct.h"
#include <stdio.h>
#include <stdlib.h>
#define LIST_INC 5

list_error _lerror = 0;

list_error list_get_error(void) {
    // Return last error value, then set it to 0
    list_error r = _lerror;
    _lerror = 0;
    return r;
}

char *list_strerror(list_error val) {
    // Returns a string description of val
    switch (val) {
        case NO_ERROR:
            return "no error";
        case INDEX_OUT_OF_BOUNDS:
            return "requested index out of bounds";
        case OUT_OF_MEMORY:
            return "malloc(3) or realloc(3) failed";
        case LIST_IS_NULL:
            return "tried to pass null as handle";
        default:
            return "something else";
    }
}

list_handle list_alloc(unsigned int size, const void *starter) {
    if (size == 0) {
        size = LIST_INC;
    }
    list_handle list = calloc(sizeof(*list), 1);
    if (list == NULL) {
        _lerror = OUT_OF_MEMORY;
        return NULL;
    }
    list_node_t arr = calloc(sizeof(*arr), size);
    if (arr == NULL) {
        _lerror = OUT_OF_MEMORY;
        free(list);
        return NULL;
    }
    list->start = arr;
    list->mz = size;
    arr[0].data = starter;
    _lerror = NO_ERROR;
    return list;
}

void *list_index(const list_handle head, unsigned int ind) {
    if (head == NULL) {
        _lerror = LIST_IS_NULL;
        return NULL;
    }
    if (ind > head->mz) {
        _lerror = INDEX_OUT_OF_BOUNDS;
        return NULL;
    }
    _lerror = NO_ERROR;
    return (void*)(head->start[ind].data);
}

void list_set_index(const list_handle head, unsigned int ind, const void *item) {
    if (head == NULL) {
        _lerror = LIST_IS_NULL;
        return;
    }
    if (ind >= list_size(head)) {
        _lerror = INDEX_OUT_OF_BOUNDS;
    }
    head->start[ind].data = item;
    _lerror = NO_ERROR;
}

void list_append(const list_handle head, const void *item) {
    if (head == NULL) {
        _lerror = LIST_IS_NULL;
        return;
    }
    if (head->start[0].next == NULL && head->start[0].data == NULL) {
        head->start[0].data = item;
        _lerror = NO_ERROR;
        return;
    }
    if (list_size(head) >= head->mz) {
        list_node_t newptr = realloc(head->start, sizeof(*(head->start)) * (head->mz + LIST_INC));
        if (newptr == NULL) {
            _lerror = OUT_OF_MEMORY;
            return;
        } else {
            head->mz += LIST_INC;
            head->start = newptr;
        }
    }
    unsigned int last_index = list_size(head);
    head->start[last_index].data = item;
    if (last_index != 0) {
        head->start[last_index - 1].next = &(head->start[last_index]);
    }
    _lerror = NO_ERROR;
}

void *list_pop(const list_handle head) {
    if (head == NULL) {
        _lerror = LIST_IS_NULL;
        return NULL;
    }
    unsigned int last_index = list_size(head) - 1;
    if (last_index == 0) {
        _lerror = INDEX_OUT_OF_BOUNDS;
        return NULL;
    }
    void *data = (void*)(head->start[last_index].data);
    head->start[last_index].data = NULL;
    head->start[last_index - 1].next = NULL;
    _lerror = NO_ERROR;
    if (last_index <= head->mz - LIST_INC) {
        list_node_t newptr = realloc(head->start, sizeof(*(head->start)) * (head->mz - LIST_INC));
        if (newptr == NULL) {
            _lerror = OUT_OF_MEMORY;
            return data;
        } else {
            head->mz -= LIST_INC;
            head->start = newptr;
        }
    }
    return data;
}

void list_iter(const list_handle head, int (callback)(unsigned int index, void *item)) {
    if (head == NULL) {
        _lerror = LIST_IS_NULL;
        return;
    }
    unsigned int ix = list_size(head);
    for (unsigned int i = 0; i < ix; i++) {
        if (callback(i, (void*)(head->start[i].data)) != 0) {
            break;
        }
    }
}

unsigned int inline list_size(const list_handle head) {
    if (head == NULL) {
        _lerror = LIST_IS_NULL;
        return 0;
    }
    if (head->start[0].next == NULL && head->start[0].data == NULL) {
        return 0;
    }
    unsigned int i;
    for (i = 0; i < head->mz; i++) {
        if (head->start[i].next == NULL) {
            i++;
            break;
        }
    }
    _lerror = NO_ERROR;
    return i;
}

void list_dealloc(list_handle head) {
    if (head == NULL) return;
    free(head->start);
    free(head);
    _lerror = NO_ERROR;
}
