#include "midori.h"
#include "net.h"
#include "data_struct.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int run_loop(config_t cfg) {
    unsigned int next_id = 0;
    list_handle players = list_alloc(10, NULL);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        log("socket create failed: %s", strerror(errno));
        return 0;
    }
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(cfg->port);
    bind(sock, (struct sockaddr*)&address, sizeof(address));
    listen(sock, 5);
    fd_set reads;
    FD_ZERO(reads);
    fd_set writes;
    FD_ZERO(writes);
    int highest = 0;
    while (1) {
        int new = accept(sock, (struct sockaddr*)&address, sizeof(address));
        if (new != -1) {
            player_t pl = calloc(sizeof(*pl), 1);
            pl->fd = new;
            pl->cid = next_id++;
            list_append(players, pl);
            FD_SET(new, reads);
            FD_SET(new, writes);
            if (new > highest) {
                highest = new;
            }
        }
        int a = select(highest + 1, &reads, &writes, NULL, NULL);
        if (a != -1) {
            // do shit here
        } else {
            log("%s", strerror(errno));
            break;
        }
    }
    close(sock);
    return 0;
}