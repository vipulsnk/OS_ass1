#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 


// void match_pattern(char *argv[],char *arr) 
// { 
//     int fd,r,j=0; 
//     char temp,line[1000]; 
//     if((fd=open(cpath,O_RDONLY)) != -1) 
//     { 
//         while((r=read(fd,&temp,sizeof(char)))!= 0) 
//         { 
//             if(temp!='\n') 
//             { 
//                 line[j]=temp; 
//                 j++; 
//             } 
//             else 
//             { 
//                 line[j]='\0';
//                 if(strstr(line,query)!=NULL) 
//                     printf("%s:%s\n",cpath,line); 
//                 j=0; 
//             } 
//         }
//     }
//     return;
// }



void qsearch(char * cpath, char * query){
    // FILE *fptr;
    // char * line = NULL;
    // size_t len = 0;
    // ssize_t read;
    // // int fd;
    // // char temp, line[10000];
    // // if((fd=open(cpath, O_RDONLY)))
    // if ((fptr = fopen(cpath, "r")) == NULL){
    //     printf("Error! opening file");
    //     // Program exits if file pointer returns NULL.
    //     exit(1);         
    // }else{
    //     while ((read = getline(&line, &len, fptr)) != -1) {
    //         // printf("Retrieved line of length %zu:\n", read);
    //         if(strstr(line, query) != NULL) {
    //             printf("%s:%s", cpath, line);
    //         }
    //     }
    // if (line)
    //     free(line);
    // fclose(fptr);
    // }

    int fd,r,j=0; 
    char temp,line[1000]; 
    if((fd=open(cpath,O_RDONLY)) != -1) 
    { 
        while((r=read(fd,&temp,sizeof(char)))!= 0) 
        { 
            if(temp!='\n') 
            { 
                line[j]=temp; 
                j++; 
            } 
            else 
            { 
                line[j]='\0';
                if(strstr(line,query)!=NULL) 
                    printf("%s:%s\n",cpath,line); 
                j=0; 
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