#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 
int word_cnt=0;
void qsearch(char * cpath, char * query){
    FILE *fptr;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int fd=open("output.txt", O_RDWR | O_CREAT);
    dup2(fd, 1);
    if ((fptr = fopen(cpath, "r")) == NULL){
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }else{
        while ((read = getline(&line, &len, fptr)) != -1) {
            // printf("Retrieved line of length %zu:\n", read);
            if(strstr(line, query) != NULL) {
                printf("%s:%s", cpath, line);
                word_cnt++;
            }
        }
        if (line)
            free(line);
        fclose(fptr);
    }
    return;
}

void search(char * path, char * query){
    DIR * dirp = opendir(path);
    char name[]="Postgraduate";
    char cpath[1000];
    struct stat path_stat;
    stat(path, &path_stat);
    if(S_ISREG(path_stat.st_mode)){
        // printf("its a regular file\n");
        qsearch(path, query);
        return;
    }
    struct dirent * dp;
    if (dirp == NULL){
        printf("some error occured\n");
    }else{
        int len = strlen(name);
        while ((dp = readdir(dirp)) != NULL) {
            if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")){
                continue;
            }
            // struct stat path_stat;
            strcpy(cpath, path);
            strcat(cpath, "/");
            strcat(cpath, dp->d_name);
            stat(cpath, &path_stat);
            if(S_ISREG(path_stat.st_mode)){
                qsearch(cpath, query);
            }else if(S_ISDIR(path_stat.st_mode)){
                search(cpath, query);
            }else{
                printf("something else %s\n", cpath);  
            }
        }
        (void)closedir(dirp);
    }
    return;
}

void count(char * path, char * query){
    search(path, query);
    if(word_cnt){
        printf("      %d\n", word_cnt);
    }
    return;
}

void dollar(char ** argv){
    int pipefd[2];
    pid_t pid1;
    pid_t pid2;
    if(pipe(pipefd) == -1){
        perror("pipe");
        exit(1);
    }
    pid1=fork();
    if(pid1==-1){
        perror("fork");
        exit(1);
    }else if(pid1==0){
        /*child process*/
        dup2(pipefd[1], 1);
        close(pipefd[0]);
        char *args[]={"grep", "-rF", argv[2], argv[3], NULL};
        execvp(args[0],args);
        return;
    }else{
        /*parent process*/
        wait(NULL);
        dup2(pipefd[0], 0);
        close(pipefd[1]);
        // pid2=fork();
        execvp(argv[5], argv+6);
    }
}

int main(int argc, char ** argv){
    int c=argc;
    if(argc<4){
        printf("please enter the missing arguments\n");
        return 0;
    }
    if(!strcmp("@", argv[1])){
        count(argv[3], argv[2]);
    }else if(!strcmp("$", argv[1])){
        // char cmd[1000]="";
        // for(int i=5; i<c; i++){
        //     strcat(cmd, argv[i]);
        //     strcat(cmd, " ");
        // }
        // printf("cmd: %s\n", cmd);
        dollar(argv);
        // dollar(argv[2], argv[3]);
        printf("$ option\n");
    }else{
        printf("input error\n");
        return 0;
    }
    
    return 0;
}