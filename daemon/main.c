#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sched.h>
#include <sys/syscall.h>

#include "parser/parse_proc_spec.h"


void start_process(char* process_yaml_loc, struct Process* p) {
    parse_process_yaml(process_yaml_loc, p);
    
    if ( chdir(p->ContextDir) != 0 ) {
        perror("error changing dir");
        exit(1);
    }

    int clone_flags = SIGCHLD | CLONE_NEWNS;

    pid_t pid = clone()



}


int main() {

    struct Process* p = (struct Process*)calloc(1, sizeof(struct Process));
    printf("parsing yaml\n");
    parse_process_yaml("process.yaml", p);
    printf("done parsing yaml\n");
    free_process(p);

    return 0;
}