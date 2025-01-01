#ifndef PARSE_PROC_SPEC_H
#define PARSE_PROC_SPEC_H

#define MAX_JOB_CMD_ARGS 20
#define MAX_PROC_ENV 50
#define MAX_PORT_MAPS 30

#include <sys/types.h>
#include <yaml.h>

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
};

void parse_process_yaml(char* filepath, struct Process* process);
void parse_image(yaml_parser_t* parser, struct Image* image);
void parse_process_job(yaml_parser_t* parser, struct ProcessJob* job);
void parse_job_command(yaml_parser_t* parser, struct ProcessJobCommand* job);
void parse_process_env(yaml_parser_t* parser, struct ProcessEnv* penv);
void parse_process_net(yaml_parser_t* parser, struct ProcessNetwork* net);
void parse_pnet_ports(yaml_parser_t* parser, struct ProcessNetwork* net);
void parse_pnet_port_map(yaml_parser_t* parser, struct PortMap* pm);
void free_process(struct Process* process);
#endif // PARSE_PROC_SPEC_H