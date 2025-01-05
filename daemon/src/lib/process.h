#ifndef PROCESS_H
#define PROCESS_H

#define MAX_JOB_CMD_ARGS 20
#define MAX_PROC_ENV 50
#define MAX_PORT_MAPS 30
#define STACKSIZE (1024*1024)

struct Env {
    char* Key;
    char* Val;
};

struct ProcessEnv {
    int count;
    struct Env** env;
};

struct Image {
    char* Id;
    char* Name;
    char* ContextTempDir;
    char* ImgPath;
    char* Tag;
    char* Created;
};

struct PortMap {
    char* HostPort;
    char* ProcPort;
};

struct PortMapping {
    int nports;
    struct PortMap** pmap;
};

struct ProcessJobCommand{
    char* command; 
    char** args; 
    int argc;
};

struct ProcessJob {
    char* Name;
    struct ProcessJobCommand* Command;
};

struct ProcessNetworkNamespace {
    char* NamespaceId; 
};

struct ProcessNetwork {
    struct PortMapping* pm;
};

struct Process {
    char* Id;
    char* Name;
    int Pid;
    char* ContextDir;
    struct Image* Image;
    struct ProcessJob* Job;
    struct ProcessEnv* Env;
    struct ProcessNetwork* Network;

    // params from outside the yaml 
    int fd[2];
    char* Stack; // the allocated stack - must be freed
    char* Rootfs; // pointer to rootfs 
    int ExitStatus; // exit status
};


void free_process(struct Process* process);

#endif