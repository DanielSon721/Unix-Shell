#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stddef.h>
#include <sysexits.h>
#include <err.h>

#define MAX_LINE_LEN 1024
#define MAX_STACK 16

int main() {
    char buffer[MAX_LINE_LEN];
    char command[MAX_LINE_LEN];
    char argument[MAX_LINE_LEN];
    char *directory_stack[MAX_STACK];
    int stack_count = 0;
    int stop = 1;
    int inputs;

    while (stop) {
        printf("shell_jr: ");
        fflush(stdout);

        if (!fgets(buffer, MAX_LINE_LEN + 1, stdin)) {
            break;  // EOF
        }

        inputs = sscanf(buffer, "%s%s", command, argument);

        if (strcmp(command, "exit") == 0 || strcmp(command, "goodbye") == 0) {
            printf("See you\n");
            stop = 0;
        } 
        else if (strcmp(command, "cd") == 0) {
            if (inputs < 2 || chdir(argument) != 0) {
                printf("Cannot change to directory %s\n", argument);
            }
        } 
        else if (strcmp(command, "pushd") == 0) {
            if (stack_count == MAX_STACK) {
                printf("Directory stack is full\n");
            } else {
                char *current_directory = getcwd(NULL, 0);
                if (!current_directory) {
                    perror("getcwd");
                    continue;
                }
                directory_stack[stack_count++] = current_directory;

                if (inputs < 2 || chdir(argument) != 0) {
                    printf("Cannot change to directory %s\n", argument);
                }
            }
        } 
        else if (strcmp(command, "dirs") == 0) {
            for (int i = stack_count - 1; i >= 0; i--) {
                printf("%s\n", directory_stack[i]);
            }
        } 
        else if (strcmp(command, "popd") == 0) {
            if (stack_count == 0) {
                printf("Directory stack is empty\n");
            } else {
                char *last_dir = directory_stack[--stack_count];
                if (chdir(last_dir) != 0) {
                    printf("Failed to change to %s\n", last_dir);
                }
                free(last_dir);
            }
        } 
        else {
            int pid = fork();

            if (pid == 0) { // Child
                char *argv[3];  // Only supports command and one argument
                argv[0] = command;

                if (inputs == 1) {
                    argv[1] = NULL;
                } else {
                    argv[1] = argument;
                    argv[2] = NULL;
                }

                execvp(command, argv);
                printf("Failed to execute %s\n", command);
                exit(EX_OSERR);
            } 
            else if (pid > 0) {
                wait(NULL);  // Parent waits
            } 
            else {
                perror("fork");
            }
        }
    }

    // Clean up any allocated memory
    for (int i = 0; i < stack_count; i++) {
        free(directory_stack[i]);
    }

    return 0;
}
