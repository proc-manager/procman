#include <yaml.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_yaml(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    yaml_parser_t parser;
    yaml_event_t event;

    // Initialize the parser
    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize parser\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    yaml_parser_set_input_file(&parser, file);

    // Parse the YAML file
    int done = 0;
    while (!done) {
        if (!yaml_parser_parse(&parser, &event)) {
            fprintf(stderr, "Parser error %d\n", parser.error);
            exit(EXIT_FAILURE);
        }

        switch (event.type) {
            case YAML_NO_EVENT:
                break;

            case YAML_STREAM_START_EVENT:
                printf("Stream start\n");
                break;

            case YAML_STREAM_END_EVENT:
                printf("Stream end\n");
                done = 1;
                break;

            case YAML_DOCUMENT_START_EVENT:
                printf("Document start\n");
                break;

            case YAML_DOCUMENT_END_EVENT:
                printf("Document end\n");
                break;

            case YAML_MAPPING_START_EVENT:
                printf("Mapping start\n");
                break;

            case YAML_MAPPING_END_EVENT:
                printf("Mapping end\n");
                break;

            case YAML_SEQUENCE_START_EVENT:
                printf("Sequence start\n");
                break;

            case YAML_SEQUENCE_END_EVENT:
                printf("Sequence end\n");
                break;

            case YAML_SCALAR_EVENT:
                printf("Scalar: %s\n", event.data.scalar.value);
                break;

            default:
                break;
        }

        yaml_event_delete(&event);
    }

    // Cleanup
    yaml_parser_delete(&parser);
    fclose(file);
}

int main() {
    const char *filename = "process.yaml";
    parse_yaml(filename);
    return 0;
}
