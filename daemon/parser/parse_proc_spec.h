#ifndef PARSE_PROC_SPEC_H
#define PARSE_PROC_SPEC_H

#define MAX_JOB_CMD_ARGS 20
#define MAX_PROC_ENV 50

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

struct PortMapping {
    char* HostPort;
    char* ContainerPort;
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
    char** Veths; 
};

struct ProcessNetwork {
    struct PortMapping** ports;
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
};

void parse_process_yaml(char* filepath, struct Process* process);
void parse_image(yaml_parser_t* parser, struct Image* image);
void parse_process_job(yaml_parser_t* parser, struct ProcessJob* job);
void parse_job_command(yaml_parser_t* parser, struct ProcessJobCommand* job);
void parse_process_env(yaml_parser_t* parser, struct ProcessEnv* penv);
// void parse_job_command(yaml_parser_t* parser, char** command);
// void parse_job(yaml_parser_t* parser, struct ProcessJob* job);
// void parse_env(yaml_parser_t* parser, struct Env** env);
// void parse_network(yaml_parser_t* parser, struct ProcessNetwork* net);
// void parse_network_
#endif // PARSE_PROC_SPEC_H