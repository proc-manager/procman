#include <stdio.h>
#include <stdlib.h>
#include "lib/helper.h"
#include "lib/process.h"
#include "lib/process.h"

void graceful_exit(struct Process* proc, char* msg, int exit_code) {
    printf("graceful exit called\n");
    free_process(proc);
    perror(msg);
    exit(exit_code);
}