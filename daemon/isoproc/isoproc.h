#ifndef ISOLATED_PROCESS_H
#define ISOLATED_PROCESS_H

#include "parser/parse_proc_spec.h"

int isoproc(void *p);


void prepare_mntns(struct Process* proc);

#endif // ISOLATED_PROCESS_H