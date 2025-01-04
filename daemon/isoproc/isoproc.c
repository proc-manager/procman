#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/mount.h>

// custom 
#include "parser/parse_proc_spec.h"
#include "isoproc.h"
#include "helper.h"

int isoproc(void* p) {
    
    struct Process* process = (struct Process*)p;

    // 
    if( chdir(process->ContextDir) != 0 ) {
        graceful_exit(process, "error chdir to context directory" ,1);
    }

}


int prepare_mntns(struct Process* proc) {
    char buffer[150];
    char* mntfs;

    if ( sprintf(buffer, "%s/%s", proc->ContextDir, "rootfs") < 0 ) {
        graceful_exit(proc, "error copying rootfs path to buf", 1);
    }

    mntfs = strdup(buffer);
    proc->Rootfs = mntfs;

    if ( mount(proc->Rootfs, mntfs, "ext4", MS_BIND, "")) {
        graceful_exit(proc, "error mounting", 1);
    } 


    if ( chdir(mntfs) ) {
        graceful_exit(proc, "error chdir", 1);
    }

    const char* put_old = ".put_old";
    if( mkdir(put_old, 0777) && errno != EEXIST ) {
        graceful_exit(proc, "error creating the putold directory", 1);
    }


    
}