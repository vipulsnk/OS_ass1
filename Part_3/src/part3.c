#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 


int compute(char* path, char * root){
    struct stat path_stat;
    char cpath[1000];
    int pid;
    int ret=0;
    stat(path, &path_stat);
    int pipefd[2];
    if(pipe(pipefd) == -1){
        perror("pipe");
        exit(1);
    }
    if(S_ISREG(path_stat.st_mode)){ //returning size of given file
        return path_stat.st_size;
    }
    DIR * dirp = opendir(path);
    struct dirent * dp;
    if (dirp == NULL){
        printf("some error occured\n");
    }else{
        while ((dp = readdir(dirp)) != NULL) {
            if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")){
                continue;
            }
            struct stat path_stat;
            strcpy(cpath, path);
            strcat(cpath, "/");
            strcat(cpath, dp->d_name);
            stat(cpath, &path_stat);
            if(S_ISREG(path_stat.st_mode)){ // regular file found, just add up the size
                ret+=path_stat.st_size;
            }else if(S_ISDIR(path_stat.st_mode)){   // directory found
                pid=fork();     // splitting process
                if(pid==0){     // child process
                    close(pipefd[0]);
                    int temp=0;
                    temp=compute(cpath, root);  // recursively computing for founded directory
                    write(pipefd[1], &temp, sizeof(int)); // writing to pipe, communicate to parent process
                    if(!strcmp(root, path)){    // for printing purposes
                        printf("%s ", cpath);
                    }
                    exit(0);    // child should exit after computing 
                    break;
                }else{
                    wait(NULL);
                    int buf;
                    read(pipefd[0], &buf, sizeof(int));   // parent reading from child's data using pipe
                    if(!strcmp(root, path)){    // printing purposes
                        printf("%d\n", buf);
                    }
                    ret+=buf;
                    continue;
                }
            }else{
                printf("something else %s\n", path);  
            }
        }
        (void)closedir(dirp);
    }
    return ret;
}

int main(int argc, char ** argv){
    if(argc<2){
        printf("please enter the remaining args\n");
        return 0;
    }else{
        printf("%s %d\n", argv[1], compute(argv[1], argv[1]));
    }
    return 0;
}