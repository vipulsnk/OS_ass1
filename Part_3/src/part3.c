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
    if(S_ISREG(path_stat.st_mode)){
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
            if(S_ISREG(path_stat.st_mode)){
                ret+=path_stat.st_size;
            }else if(S_ISDIR(path_stat.st_mode)){
                pid=fork();
                if(pid==0){
                    close(pipefd[0]);
                    int temp=0;
                    temp=compute(cpath, root);
                    write(pipefd[1], &temp, 4);
                    if(!strcmp(root, path)){
                        printf("%s ", cpath);
                    }
                    exit(0);
                    break;
                }else{
                    wait(NULL);
                    int buf;
                    read(pipefd[0], &buf, 4);
                    if(!strcmp(root, path)){
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