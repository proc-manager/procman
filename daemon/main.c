#include<stdlib.h>

#include "parser/parse_proc_spec.h"

int main() {
    struct Process* p = calloc(1, sizeof(struct Process));

    parse_process_yaml("process.yaml", p);
    return 0;
}