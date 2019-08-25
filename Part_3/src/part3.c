#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 


void compute(char* path){
    struct stat path_stat;
    stat(path, &path_stat);
    if(S_ISREG(path_stat.st_mode)){
        // printf("its a regular file\n");
        // qsearch(path, query);
        printf("reg file\n");
        return;
    }
    DIR * dirp = opendir(path);
    struct dirent * dp;
    if (dirp == NULL){
        printf("some error occured\n");
    }else{
        // int len = strlen(name);
        while ((dp = readdir(dirp)) != NULL) {
            if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")){
                continue;
            }
            // struct stat path_stat;
            // strcpy(cpath, path);
            // strcat(cpath, "/");
            // strcat(cpath, dp->d_name);
            stat(path, &path_stat);
            if(S_ISREG(path_stat.st_mode)){
                // qsearch(cpath, query);
                printf("reg file\n");
            }else if(S_ISDIR(path_stat.st_mode)){
                // search(cpath, query);
                printf("directory\n");
            }else{
                printf("something else %s\n", path);  
            }
        }
        (void)closedir(dirp);
    }
    return;
}

int main(int argc, char ** argv){
    if(argc<2){
        printf("please enter the remaining args\n");
        return 0;
    }else{
        compute(argv[1]);
    }
    return 0;
}