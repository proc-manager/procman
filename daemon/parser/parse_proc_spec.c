#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yaml.h>
#include "parse_proc_spec.h"


void print_parsed_image(struct Image* image){
    printf("-------Image-----\n");
    printf("%s\n", image->Id);
    printf("%s\n", image->Name);
    printf("%s\n", image->ContextTempDir);
    printf("%s\n", image->ImgPath);
    printf("%s\n", image->Tag);
    printf("%s\n", image->Created);
    printf("-------Image-----\n");
}

void print_parsed_job(struct ProcessJob* job){
    printf("-------Job-----\n");
    printf("%s\n", job->Name);

    struct ProcessJobCommand* cmd = job->Command;
    printf("command: %s\n", cmd->command);
    for(int c=0; c < cmd->argc; c++){
        if( cmd->args[c] != NULL ){
            printf("%s ", cmd->args[c]);
        }
    }
    printf("\n-------Job-----\n");   
}

void print_parsed_process(struct Process *process){
    printf("Id = %s\n", process->Id);
    printf("Name = %s\n", process->Name);
    printf("Pid = %d\n", process->Pid);
    print_parsed_image(process->Image);
    print_parsed_job(process->Job);
}

void free_process_job(struct ProcessJob* job) {
    free(job->Name);
    struct ProcessJobCommand* cmd = job->Command;
    for(int c=0; c < cmd->argc; c++){
        if( cmd->args[c] != NULL ){
            free(cmd->args[c]);
        }
    }
    free(cmd->command);
    free(cmd->args);
    free(job);
}

void free_image(struct Image* image) {
    free(image->Id);
    free(image->Name);
    free(image->ContextTempDir);
    free(image->ImgPath);
    free(image->Tag);
    free(image->Created);
    free(image);
    image = NULL;
}

void free_process(struct Process* process) {
    free(process->Id);
    free(process->Name);
    free_image(process->Image);
    free_process_job(process->Job);
    // free process mem
    free(process);
    process = NULL;
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
            yaml_event_delete(&event);
            break;
        }

        switch(event.type) {
            case YAML_SCALAR_EVENT:
                if (key == NULL) {
                    key = strdup((char*)event.data.scalar.value);
                    break;
                } else {
                    if ( strcmp(key, "id") == 0 ) {
                        process->Id = strdup((char*)event.data.scalar.value);
                        // printf("key: %s, val: %s\n", key, process->Id);
                    } else if ( strcmp(key, "name") == 0 ) {
                        process->Name = strdup((char*)event.data.scalar.value);
                        // printf("key: %s, val: %s\n", key, process->Name);
                    } else if ( strcmp(key, "pid") == 0 ) {
                        process->Pid = atoi((char*)event.data.scalar.value);
                        // printf("key: %s, val: %d\n", key, process->Pid);
                    } else if ( strcmp(key, "image") == 0 ) {
                        break;
                    } else if ( strcmp(key, "job") == 0 ) {
                       break; 
                    }
                }
                free(key);
                key = NULL;
                break;
            
            case YAML_MAPPING_START_EVENT:
                if (key == NULL) {
                    break;
                } else if (strcmp(key, "image") == 0) {
                    free(key);
                    key = NULL;
                    struct Image* image = (struct Image*)calloc(1, sizeof(struct Image));
                    parse_image(&parser, image);
                    process->Image = image;
                } else if (strcmp(key, "job") == 0) {
                    free(key);
                    key = NULL;
                    struct ProcessJob* job = (struct ProcessJob*)calloc(1, sizeof(struct ProcessJob));
                    parse_process_job(&parser, job);
                    process->Job = job;
                }
                break;
        }
        yaml_event_delete(&event);
    }

    if (key != NULL){
        free(key);
        key = NULL;
    }

    printf("parsed the yaml\n");
    yaml_parser_delete(&parser);
    fclose(file);
    print_parsed_process(process);
    free_process(process);
}


void parse_image(yaml_parser_t* parser, struct Image* image) {
    yaml_event_t event;
    char* key = NULL;

    while(1) {
        if (!yaml_parser_parse(parser, &event)) {
            fprintf(stderr, "parser error: %d\n", parser->error);
            break;
        } 

        switch(event.type) {
            case YAML_MAPPING_START_EVENT:
                if (key != NULL){
                    free(key);
                }
                key = NULL;
                break;

            case YAML_SCALAR_EVENT:
                if ( key == NULL ) {
                    key = strdup((char*)event.data.scalar.value);
                } else {
                    if ( strcmp(key, "id") == 0 ) {
                        image->Id = strdup((char*)event.data.scalar.value);
                        // printf("key: %s, val: %s\n", key, image->Id);
                    } else if ( strcmp(key, "name") == 0 ) {
                        image->Name = strdup((char*)event.data.scalar.value);
                        // printf("key: %s, val: %s\n", key, image->Name);
                    } else if ( strcmp(key, "context_temp_dir") == 0 ) {
                        image->ContextTempDir = strdup((char*)event.data.scalar.value);
                        // printf("key: %s, val: %s\n", key, image->ContextTempDir);
                    } else if ( strcmp(key, "imgpath") == 0 ) {
                        image->ImgPath = strdup((char*)event.data.scalar.value);
                        // printf("key: %s, val: %s\n", key, image->ImgPath);
                    } else if ( strcmp(key, "tag") == 0 ) {
                        image->Tag = strdup((char*)event.data.scalar.value);
                        // printf("key: %s, val: %s\n", key, image->Tag);
                    } else if ( strcmp(key, "created") == 0 ) {
                        image->Created = strdup((char*)event.data.scalar.value);
                        // printf("key: %s, val: %s\n", key, image->Created);
                    }
                    free(key);
                    key = NULL;
                }
                yaml_event_delete(&event);
                break;

            case YAML_MAPPING_END_EVENT:
                if ( key != NULL ){
                    free(key);
                    key = NULL;
                }
                yaml_event_delete(&event);
                printf("mapping end event\n");
                return; 
        }
    }
    if (key != NULL){
        free(key);
        key = NULL;
    }

}

void parse_process_job(yaml_parser_t* parser, struct ProcessJob* job) {
    yaml_event_t event;
    char* key = NULL;

    while(1) {
        if (!yaml_parser_parse(parser, &event)) {
            fprintf(stderr, "parser error: %d\n", parser->error);
            break;
        } 

        switch(event.type) {
            case YAML_MAPPING_START_EVENT:
                if (key != NULL){
                    free(key);
                }
                key = NULL;
                break;

            case YAML_SCALAR_EVENT:
                if ( key == NULL ) {
                    key = strdup((char*)event.data.scalar.value);
                } else {
                    if ( strcmp(key, "name") == 0 ) {
                        job->Name = strdup((char*)event.data.scalar.value);
                        // printf("key: %s, val: %s\n", key, image->Name);
                    } else if ( strcmp(key, "command") == 0 ) {
                        job->Command = (struct ProcessJobCommand*)calloc(1, sizeof(struct ProcessJobCommand));
                        parse_job_command(&parser, job->Command);
                    }
                    free(key);
                    key = NULL;
                }
                yaml_event_delete(&event);
                break;

            case YAML_MAPPING_END_EVENT:
                if ( key != NULL ){
                    free(key);
                    key = NULL;
                }
                yaml_event_delete(&event);
                printf("mapping end event\n");
                return; 
        }
    }
    if (key != NULL){
        free(key);
        key = NULL;
    }
}

void parse_job_command(yaml_parser_t* parser, struct ProcessJobCommand* job) { 
    yaml_event_t event;

    job->argc = 0;
    int argc = 0;
    char** args = (char**)calloc(1, sizeof(char*));

    while(1) {
        if (!yaml_parser_parse(parser, &event)) {
            fprintf(stderr, "parser error: %d\n", parser->error);
            break;
        } 

        switch(event.type) {
            case YAML_SEQUENCE_START_EVENT:
                yaml_event_delete(&event);
                break;

            case YAML_SCALAR_EVENT:
                argc = argc + 1;
                args[argc-1] = strdup((char*)event.data.scalar.value); 
                args = (char**)realloc(args, argc+1);
                if ( args == NULL ) {
                    perror("error realloc");
                    exit(1);
                }
                args[argc] = NULL; 
                yaml_event_delete(&event);
                break;

            case YAML_SEQUENCE_END_EVENT:
                job->argc = argc;
                if(argc > 0){
                    job->args = args;
                    job->command = strdup(args[0]);
                }
                yaml_event_delete(&event);
                printf("mapping end event\n");
                return; 
        }
    }
}
