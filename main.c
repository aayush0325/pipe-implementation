#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void pipeline(const char* process1, const char* process2) {
    int fd[2];
    
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    // Fork the first process
    if (fork() != 0) {
        // Parent process (cat)
        close(fd[0]); // Close reading end of pipe
        dup2(fd[1], STDOUT_FILENO); // Redirect stdout to the writing end of the pipe
        close(fd[1]); // Close writing end after redirection

        // Execute cat command (process1)
        execlp("cat", "cat", process1, (char*)NULL);
        perror("Failed to execute cat");
        exit(1);
    } else {
        // Child process (grep)
        close(fd[1]); // Close writing end of pipe
        dup2(fd[0], STDIN_FILENO); // Redirect stdin to the reading end of the pipe
        close(fd[0]); // Close reading end after redirection

        // Execute grep command (process2)
        execlp("grep", "grep", "Welcome", (char*)NULL);
        perror("Failed to execute grep");
        exit(1);
    }
}

int main() {
    // Welcome to Systems Programming
    pipeline("main.c", "grep Welcome");

    return 0;
}
