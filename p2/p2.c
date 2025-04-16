#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid, wpid;
    int status;

    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        
        printf("CHILD started. ");
        
        if (argc == 1) {
           
            printf("No arguments provided. Terminating child.\n");
            exit(0);
        }
        else if (argc == 2) {
       
            printf("One argument provided. Calling execlp(), never to return (sniff)\n");
            execlp(argv[1], argv[1], (char *)NULL);
            perror("execlp failed");
            exit(1);
        }
        else {
            
            printf("More than one argument provided. Calling execvp(), never to return (sniff)\n");
            execvp(argv[1], &argv[1]);
            perror("execvp failed");
            exit(1);
        }
    }
    else {
       
        printf("PARENT started, now waiting for process ID#%d\n", pid);

        wpid = waitpid(pid, &status, 0);
        if (wpid == -1) {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }
        
        printf("PARENT resumed. Child exit code of %d. Now terminating parent\n",
               WEXITSTATUS(status));
    }

    return 0;
}

