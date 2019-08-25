#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 

int main(int argc, char ** argv){
    int pipefd[2];
    int pid;
    char buf[20];
    if(pipe(pipefd) == -1){
        perror("pipe");
        exit(1);
    }
    pid = fork();
    if(pid==0){
        // printf("child process\n");
        // write(pipefd[1], "hello world!", 12);
        dup2(pipefd[1], 1);
        close(pipefd[0]);
        char *args[]={"grep", "-rF", argv[1], argv[2], NULL};
        // execv("ls", argv);
        execvp(args[0],args);
        return 0;
    }else{
        wait(NULL);
        // printf("parent process\n");
        // read(pipefd[0], buf, 15);
        dup2(pipefd[0], 0);
        close(pipefd[1]);
        char *argv[]={"wc", "-l", "-c", NULL};
        // execv("wc", NULL);
        execvp("wc",argv);
        // printf("buf: %s\n", buf);
    }
    return 0;
}