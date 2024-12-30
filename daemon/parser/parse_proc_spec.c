#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yaml.h>
#include "parse_proc_spec.h"


void print_parsed_process(struct Process process){
    printf("Id = %s\n", process.Id);
    printf("Name = %s\n", process.Name);
    printf("Pid = %d\n", process.Pid);
}

void free_process(struct Process* process) {
    free(process->Id);
    free(process->Name);
    free(process);
}

void parse_process_yaml(char* filepath, struct Process* process) {

    if (filepath == NULL){
        perror("empty filepath");
        exit(1);
    }

    if (process == NULL){
        perror("process is null");
        exit(1);
    }

    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("unable to open yaml file");
        exit(1);
    }

    yaml_parser_t parser;
    yaml_event_t event;

    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "failed to initialize yaml parser\n");
        fclose(file);
        exit(1);
    }
    yaml_parser_set_input_file(&parser, file);

    char* key = NULL;

    while(1) {
        yaml_parser_parse(&parser, &event);
        if(event.type == YAML_STREAM_END_EVENT) {
            break;
        }

        if (event.type == YAML_SCALAR_EVENT) {
            if (key == NULL) {
                key = strdup((char*)event.data.scalar.value);
                continue;
            } else {
                if(strcmp(key, "id") == 0) {
                    process->Id = strdup((char*)event.data.scalar.value);
                } else if (strcmp(key, "name") == 0) {
                    process->Name = strdup((char*)event.data.scalar.value);
                } else if (strcmp(key, "pid") == 0) {
                    process->Pid = atoi((char*)event.data.scalar.value);
                } 
                free(key);
                key = NULL; 
            }
        }

        yaml_event_delete(&event);
    }
    free_process(process);
}


// void parse_image(yaml_parser_t* parser, struct Image* image) {
//     yaml_event_t event;
//     char* key = NULL;

//     while(1) {
//         if (!yaml_parser_parse(parser, &event)) {
//             fprintf(stderr, "parser error: %d\n", parser->error);
//             break;
//         }

//         if (event.type == YAML_MAPPING_END_EVENT) {
//             break;
//         }

//         if (event.type == YAML_SCALAR_EVENT) {
//             if (key == NULL) {
//                 key = strdup((char*)event.data.scalar.value);
//                 continue;
//             } else {
//                 if (strcmp(key, "Id") == 0) {
//                     image->Id = strdup((char*)event.data.scalar.value);
//                 } else if (strcmp(key, "name") == 0) {
//                     image->Name = strdup((char*)event.data.scalar.value);
//                 } else if (strcmp(key, "context_temp_dir") == 0) {
//                     image->ContextTempDir = strdup((char*)event.data.scalar.value);
//                 } else if (strcmp(key, "img_path") == 0) {
//                     image->ImgPath = strdup((char*)event.data.scalar.value);
//                 } else if (strcmp(key, "tag") == 0) {
//                     image->Tag = strdup((char*)event.data.scalar.value);
//                 } else if (strcmp(key, "created") == 0) {
//                     image->Created = strdup((char*)event.data.scalar.value);
//                 }
//             }
//             free(key);
//             key = NULL;
//         }
//         yaml_event_delete(&event);
//     }
// }

// void parse_job_command(yaml_parser_t* parser, char** command) {
//     yaml_event_t event;

//     size_t size = 1;   
//     if ( command == NULL ){
//         command = (char**)calloc(size, sizeof(char *));
//     }

//     while(1) {
//         if (!yaml_parser_parse(parser, &event)) {
//             fprintf(stderr, "parser error: %d\n", parser->error);
//             break;
//         }

//         if (event.type == YAML_SEQUENCE_END_EVENT) {
//             break;
//         }

//         if (event.type == YAML_SCALAR_EVENT) {
//             command[size-1] = strdup((char*)event.data.scalar.value);
//             size = size + 1;
//             command = (char*)realloc(command, size); 
//             if (command == NULL) {
//                 perror("realloc failed");
//                 exit(1);
//             }
//         }
//     }
//     yaml_event_delete(&event);
// }


// void parse_job(yaml_parser_t* parser, struct ProcessJob* job) {
//     yaml_event_t event;
//     char *key = NULL;

//     while(1) {
//         if (!yaml_parser_parse(parser, &event)) {
//             fprintf(stderr, "parser error: %d\n", parser->error);
//             break;
//         }

//         if (event.type == YAML_MAPPING_END_EVENT) {
//             break;
//         }

//         if (event.type == YAML_SCALAR_EVENT) {
//             if (key == NULL){
//                 key = strdup((char*)event.data.scalar.value);
//                 continue;
//             } else {
//                 if(strcmp(key, "name") == 0) {
//                     job->Name = strdup((char*)event.data.scalar.value);
//                 } else if (strcmp(key, "command") == 0) {
//                     char** Command = NULL;
//                     parse_job_command(&event, Command); 
//                 }
//             }
//         }
//     }
// }