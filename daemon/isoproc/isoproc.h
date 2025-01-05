#ifndef ISOLATED_PROCESS_H
#define ISOLATED_PROCESS_H

#include "parse_proc_spec.h"

int isoproc(void *p);


void prepare_mntns(struct Process* proc);
void graceful_exit(struct Process* proc, char* msg, int exit_code);

#endif // ISOLATED_PROCESS_H