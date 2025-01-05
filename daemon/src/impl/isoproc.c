#define _GNU_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/mount.h>
#include <sys/wait.h>

// custom 
#include "lib/isoproc.h"
#include "lib/process.h"
#include "lib/helper.h"
#include "lib/parse_proc_spec.h"


int isoproc(void* p) {
    
    struct Process* process = (struct Process*)p;

    if( chdir(process->ContextDir) != 0 ) {
        graceful_exit(process, "error chdir to context directory" ,1);
    }

    prepare_mntns(process);
    execute_job(process);

    return 0;
}


void prepare_mntns(struct Process* proc) {
    char buffer[150];
    char* mntfs;

    printf("preparing mntns\n");

    if ( sprintf(buffer, "%s/%s", proc->ContextDir, "rootfs") < 0 ) {
        graceful_exit(proc, "error copying rootfs path to buf", 1);
    }

    mntfs = strdup(buffer);
    proc->Rootfs = mntfs;

    if ( mount(proc->Rootfs, mntfs, "ext4", MS_BIND, "")) {
        graceful_exit(proc, "error mounting", 1);
    } 
    printf("mounted rootfs\n");

    if ( chdir(mntfs) ) {
        graceful_exit(proc, "error chdir", 1);
    }
    printf("changed dir\n");

    const char* put_old = ".put_old";
    if( mkdir(put_old, 0777) && errno != EEXIST ) {
        graceful_exit(proc, "error creating the putold directory", 1);
    }
    printf("created .put_old\n");

    if ( syscall(SYS_pivot_root, ".", put_old) ) {  
        graceful_exit(proc, "error pivoting root", 1);
    }
    printf("performed sys_pivot\n");

    if ( chdir("/") ) {
        graceful_exit(proc, "error chdir to root", 1);
    }
    printf("chdir to root successful\n");

    
    printf("proc initial setup done\n");

}


void overwrite_env(struct Process* proc) {

    printf("overwriting env\n");

    if (proc == NULL || proc->Env == NULL) {
        return;
    }

    // if ( clearenv() ) {
    //     graceful_exit(proc, "error clearenv", 1);
    // }

    struct ProcessEnv* env = proc->Env;
    for(int i=0; i< env->count; i++) {
        if( setenv(env->env[i]->Key, env->env[i]->Val, 1) ) {
            graceful_exit(proc, "error setenv", 1);
        }
    }

    printf("env overwrite success");
    
}

void execute_job(struct Process* proc) {


    struct ProcessJob* job = proc->Job;
    struct ProcessJobCommand* cmd = job->Command;
    printf("executing job: %s\n", job->Name);

    pid_t pid = fork();


    if (pid < 0) {
        // fork failed  
        graceful_exit(proc, "fork failed", 1);
    } else if ( pid == 0 ) {
        // child process
        overwrite_env(proc);
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
