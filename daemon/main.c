#include<stdlib.h>
#include<stdio.h>

#include "parser/parse_proc_spec.h"

int main() {
    struct Process* p = calloc(1, sizeof(struct Process));
    printf("parsing yaml\n");
    parse_process_yaml("process.yaml", p);
    printf("done parsing yaml\n");
    return 0;
}