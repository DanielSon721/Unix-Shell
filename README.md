This is Shell Jr., my simple shell written in C!

Shell Jr. is a simple command-line shell that supports basic shell operations such as changing directories, managing a directory stack, and executing external programs.

This project demonstrates process creation, directory manipulation, and basic command parsing using standard system calls in C.

If the user enters a command that isn’t built into the shell (e.g., ls, echo, etc.), Shell Jr. will attempt to run it using fork() and execvp().

Comes with a 16-level directory stack that uses heap allocation, push/pop, and bounds checks to avert stack overflows.
