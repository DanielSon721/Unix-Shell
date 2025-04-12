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

int main() {

    char buffer[MAX_LINE_LEN];
    char command[MAX_LINE_LEN];
    char argument[MAX_LINE_LEN];
    char *directory_stack[16];
    int stack_count = 0;
    int stop = 1;
    int inputs;
    
    while (stop && fgets(buffer, MAX_LINE_LEN + 1, stdin)) {

        inputs = sscanf(buffer, "%s%s", command, argument);
        printf("shell_jr: ");
        fflush(stdout);

        if (strcmp(command, "exit") == 0 || strcmp(command, "goodbye") == 0) {
            printf("See you\n");
            stop = 0;
        }
        else if (strcmp(command, "cd") == 0) {

            if (chdir(argument) != 0) {
                printf("Cannot change to directory %s", argument);
            }

        }
        else if (strcmp(command, "pushd") == 0) {

            if (stack_count == 16) {
                printf("Directory stack is full\n");
            }
            else {
                char *current_directory;
                current_directory = getcwd(NULL, 0);
                directory_stack[stack_count++] = current_directory; /*pushes current directory onto stack*/
                chdir(argument); /*changes current directory to directory given*/
            }
        }
        else if (strcmp(command, "dirs") == 0) {

            int i;

            for (i = 0; i < stack_count; i++) {
                printf("%s\n", directory_stack[i]);
            }
        }
        else if (strcmp(command, "popd") == 0) {
            if (stack_count == 0) {
                printf("Directory stack is empty\n");
            }
            else {
                directory_stack[stack_count--] = NULL;
            }
        }
        else {
            
            int pid = fork();

            if (pid == 0) { /*child*/
                char *argv[MAX_LINE_LEN];
                argv[0] = command;

                fflush(stdout);
                if (inputs == 1) {
                    if (execvp(command, argv) == -1) {
                        printf("Failed to execute %s\n", command);
                        fflush(stdout);
                        exit(EX_OSERR);
                    }
                }
                else {
                    argv[1] = argument;
                    if (execvp(command, argv) == -1) {
                        printf("Failed to execute %s\n", command);
                        fflush(stdout);
                        exit(EX_OSERR);
                    }
                }
            }
            else {
                wait(NULL);
            }
        }
    }

    if (stop) {
        printf("shell_jr: ");
    }

    return 0;
}

