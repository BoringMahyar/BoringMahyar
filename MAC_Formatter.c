#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <regex.h>

#define MAX_LINE_LENGTH 1024

// Function to check if a string is a valid MAC address
bool is_mac_address(const char *str) {
    regex_t regex;
    int ret;
    const char *pattern = "^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$";

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret) {
        fprintf(stderr, "Could not compile regex\n");
        return false;
    }

    ret = regexec(&regex, str, 0, NULL, 0);
    regfree(&regex);

    return ret == 0;
}

// Function to apply Markdown formatting (bold)
void apply_markdown(const char *str) {
    printf("**%s**", str);
}

// Function to apply ANSI color formatting (magenta)
void apply_color(const char *str) {
    printf("\033[35m%s\033[0m", str); // 35 is the ANSI code for magenta
}

// Function to process input and apply transformations
void process_input(FILE *input, FILE *output, bool use_color, int start_obj, int end_obj) {
    char line[MAX_LINE_LENGTH];
    int obj_count = 0;

    while (fgets(line, MAX_LINE_LENGTH, input)) {
        char *token = strtok(line, " \t\n");
        while (token != NULL) {
            printf("Processing token: %s\n", token); // Debugging
            if (is_mac_address(token)) {
                printf("MAC Address found: %s\n", token); // Debugging
                obj_count++;
                if (obj_count >= start_obj && (end_obj == -1 || obj_count <= end_obj)) {
                    if (use_color) {
                        apply_color(token);
                    } else {
                        apply_markdown(token);
                    }
                } else {
                    fprintf(output, "%s", token);
                }
            } else {
                fprintf(output, "%s", token);
            }
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                fprintf(output, " ");
            }
        }
        fprintf(output, "\n");
    }
}

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;
    bool use_color = false;
    int start_obj = 1;
    int end_obj = -1;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            use_color = true;
        } else if (strncmp(argv[i], "-f=", 3) == 0) {
            start_obj = atoi(argv[i] + 3);
            if (start_obj < 1) {
                fprintf(stderr, "Error: Start object must be at least 1.\n");
                return 1;
            }
        } else if (strncmp(argv[i], "-t=", 3) == 0) {
            end_obj = atoi(argv[i] + 3);
            if (end_obj < start_obj) {
                fprintf(stderr, "Error: End object must be greater than or equal to start object.\n");
                return 1;
            }
        } else if (i == argc - 2) {
            input = fopen(argv[i], "r");
            if (input == NULL) {
                fprintf(stderr, "Error: Could not open input file %s.\n", argv[i]);
                return 1;
            }
        } else if (i == argc - 1) {
            output = fopen(argv[i], "w");
            if (output == NULL) {
                fprintf(stderr, "Error: Could not open output file %s.\n", argv[i]);
                fclose(input);
                return 1;
            }
        } else {
            fprintf(stderr, "Error: Unsupported option or argument: %s.\n", argv[i]);
            return 1;
        }
    }

    // Process input and apply transformations
    process_input(input, output, use_color, start_obj, end_obj);

    // Clean up
    if (input != stdin) {
        fclose(input);
    }
    if (output != stdout) {
        fclose(output);
    }

    return 0;
}
