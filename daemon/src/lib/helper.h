#ifndef PROCMAN_HELPER_H
#define PROCMAN_HELPER_H

#include "process.h"

void graceful_exit(struct Process* proc, char* msg, int exit_code);

#endif