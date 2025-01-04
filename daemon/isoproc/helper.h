#ifndef ISOPROC_HELPER_H
#define ISOPROC_HELPER_H

#include <parser/parse_proc_spec.h>

void graceful_exit(struct Process* proc, char* msg, int exit_code);

#endif