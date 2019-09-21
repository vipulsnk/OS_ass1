// 170803, Vipul Shankhpal
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
//part1.c code
void qsearch(char * cpath, char * query){
    int fd, ind=0; 
    char ch,line[100000]; 
    if((fd=open(cpath,O_RDONLY)) != -1) { 
        while((read(fd,&ch,sizeof(char)))!= 0) { 
            if(ch!='\n') { 
                line[ind]=ch; 
                ind++; 
            } 
            else { 
                line[ind]='\0';
                if(strstr(line,query)!=NULL){
                    word_cnt++;     // counting the lines 
                } 
                ind=0; 
            } 
        }
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
//this function uses 2 pipes, pipe1 to extract output from grep and write it to pipe2 and given file, pipe2 to execute given cmd
void dollar(char ** argv){
    int pipefd[2];
    pid_t pid1;
    pid_t pid2;
    int pipefd2[2];
    if(pipe(pipefd) == -1){
        perror("pipe");
        exit(1);
    }
    if(pipe(pipefd2) == -1){
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
        char ch;
        int filefd=open(argv[4], O_RDWR | O_CREAT, S_IRWXU);
        int b=0;
        close(pipefd[1]);
        while((b=read(pipefd[0], &ch, 1)) > 0) {    // reading char by char from pipe1
            write(filefd, &ch, 1);  // writing to file
            write(pipefd2[1], &ch, 1);  // writing to pipew
        }
        close(pipefd2[1]);
        dup2(pipefd2[0], 0);
        execvp(argv[5], argv+5);    // using pipe2 to execute cmd
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
        dollar(argv);
    }else{
        printf("input error\n");
        return 0;
    }
    
    return 0;
}