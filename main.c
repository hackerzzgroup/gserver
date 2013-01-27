#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "midori.h"
#include "ini/ini.h"
#include "net.h"
#define SETTINGS_FILE "server.ini"
#define MAXLEN_CHAT 255
#define MAXLEN_NAME 32

config_t cfg;

int ini_callback(void* user, const char* section, const char* name, const char* value) {
    struct config_s a = *(struct config_s*)user;
    if (strcmp(name, "public") == 0) {
        int pub = atoi(value);
        a.public = (char)pub;
    } else if (strcmp(name, "port") == 0) {
        int port = atoi(value);
        if (port < 1 || port > 65536) {
            return 0;
        } else {
            a.port = (unsigned short)port;
        }
    } else if (strcmp(name, "name") == 0) {
        a.name = strdup(value);
    }
    return 1;
}

int main (int argc, char const *argv[]) {
    log("gserver-ng init -- c 2013 kirarara and friends");
    FILE *fp = NULL;
    if (!(fp = fopen(SETTINGS_FILE, "r"))) {
        log("failed to read config file: %s", strerror(errno));
        exit(1);
    }
    fclose(fp);
    cfg = calloc(sizeof(*cfg), 1);
    if (ini_parse(SETTINGS_FILE, ini_callback, cfg) != 0) {
        log("error parsing config file");
    } else {
        run_loop(cfg);
    }
    free(cfg);
    return 0;
}