#define _GNU_SOURCE

#include "isoproc/isoproc.h"
#include "parser/parse_proc_spec.h"

#include<signal.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<linux/unistd.h>
#include<linux/sched.h>
#include<sched.h>
#include<sys/syscall.h>

#define STACKSIZE (1024*1024)



void start_process(char* process_yaml_loc, struct Process* p) {
    parse_process_yaml(process_yaml_loc, p);
    
    if ( chdir(p->ContextDir) != 0 ) {
        perror("error changing dir");
        exit(1);
    }

    int clone_flags = SIGCHLD | CLONE_NEWNS | CLONE_NEWUTS | CLONE_NEWUSER;
    char* cmd_stack = malloc(STACKSIZE);

    pid_t pid = clone(isoproc, cmd_stack + STACKSIZE, clone_flags, (void*)p);
    if (pid == -1){
        perror("clone");
        free(cmd_stack);
        exit(EXIT_FAILURE);
    }

    p->Pid = pid;
    p->Stack = cmd_stack;
}


int main() {

    struct Process* p = (struct Process*)calloc(1, sizeof(struct Process));
    printf("parsing yaml\n");
    // parse_process_yaml("process.yaml", p);
    start_process("process.yaml", p);
    printf("done parsing yaml\n");
    free_process(p);

    return 0;
}