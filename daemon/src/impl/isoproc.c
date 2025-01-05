#define _GNU_SOURCE


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mount.h>
#include <sys/wait.h>

// custom 
#include "lib/isoproc.h"
#include "lib/process.h"
#include "lib/parse_proc_spec.h"



void graceful_exit(struct Process* proc, char* msg, int exit_code) {
    free_process(proc);
    perror(msg);
    exit(exit_code);
}

int isoproc(void* p) {
    
    struct Process* process = (struct Process*)p;

    if( chdir(process->ContextDir) != 0 ) {
        graceful_exit(process, "error chdir to context directory" ,1);
    }

    prepare_mntns(process);

}


void prepare_mntns(struct Process* proc) {
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

    if ( syscall(SYS_pivot_root, ".", put_old) ) {  
        graceful_exit(proc, "error pivoting root", 1);
    }

    if ( chdir("/") ) {
        graceful_exit(proc, "error chdir to root", 1);
    }

}


void overwrite_env(struct Process* proc) {
    clearenv();

    struct ProcessEnv* env;
    for(int i=0; i< env->count; i++) {
        setenv(env->env[i]->Key, env->env[i]->Val, 1);
    }
    
}

void execute_job(struct Process* proc) {
    overwrite_env(proc);

    struct ProcessJob* job = proc->Job;
    struct ProcessJobCommand* cmd = job->Command;
    printf("executing job: %s\n", job->Name);

    pid_t pid = fork();

    if (pid < 0) {
        // fork failed  
        graceful_exit(proc, "fork failed", 1);
    } else if ( pid == 0 ) {
        // parent process
        if ( execvp(cmd->command, cmd->args) == -1 ) {
            graceful_exit(proc, "execvp failed", 1);
        }
    } else {
        // parent process
        int status;
        while(1) {
            pid_t result = waitpid(pid, &status, WNOHANG);

            if (result == 0) {
                sleep(1);
            } else if (result == pid) {
                if ( WIFEXITED(status) ) {
                    proc->ExitStatus = WIFEXITED(status);
                } else {
                    proc->ExitStatus = status;
                }
                break;
            } else {
                graceful_exit(proc, "waitpid failed", 1);
            }
        }
    }
}
