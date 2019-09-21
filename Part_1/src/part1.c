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
// this function searches for the given query in file denoted by cpath
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
                    printf("%s:%s\n",cpath,line); 
                } 
                ind=0; 
            } 
        }
    }
    return;
}

// this function recursively searches for files and directories and calls qsearch if a file is found otherwise itself to look in the directory found
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

int main(int argc, char ** argv){
    if(argc<3){
        printf("please enter the missing arguments\n");
        return 0;
    }else if(argc>3){
        printf("Too many args!\n");
        return 0;
    }
    search(argv[2], argv[1]);
    return 0;
}