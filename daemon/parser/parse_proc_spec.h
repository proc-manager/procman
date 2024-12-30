/*
    Process specification yaml
*/

#include<sys/types.h>

struct Env {
    char* Key;
    char* Val;
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
    unsigned short HostPort;
    unsigned short ProcPort;
};

struct Process {
    char* Id;
    char* Name;
    pit_t Pid = 0;
    char* ContextDir;
    struct Image* Image;
    struct Env* Env;
    struct PortMapping** Network; 
};

