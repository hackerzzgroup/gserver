#ifndef _NET_H_
#define _NET_H_
#include <sys/types.h>
#include "midori.h"

typedef struct player_s {
    int fd;
    unsigned int cid;
    char *name;
    unsigned long pos_x;
    unsigned long pos_y;
    unsigned int sprite_x;
    unsigned int sprite_y;
    unsigned int sprite_pixels[];
} *player_t;

int run_loop(config_t cfg);

#endif