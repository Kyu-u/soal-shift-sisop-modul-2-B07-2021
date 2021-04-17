#include <sys/wait.h>
#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

void folder2a(){
   pid_t pid1;
   pid1 = fork();
   if (pid1 < 0){
      exit(EXIT_FAILURE);
   }
   if (pid1 == 0) {
      char *argvA[] = {"mkdir", "-p", "./petshop", NULL};
      execv("/bin/mkdir", argvA);
   }
   wait(NULL);
   pid_t unzip_id;
   unzip_id = fork();
   if (unzip_id<0){
      exit(EXIT_FAILURE);
   }
   if (unzip_id == 0){
      char *argvB[] = {"unzip", "/home/hanifa/modul2/pets.zip", "-d", "/home/hanifa/modul2/petshop", "-x", "*/*", NULL};
      execv("/usr/bin/unzip", argvB);
   }
}

void folder2b(){
   DIR *d;
    struct dirent *dir;
    d = opendir("/home/hanifa/modul2/petshop");
    char *delim1 = ";";
     while ((dir = readdir(d)) != NULL) {
       if ((strcmp(dir->d_name, "..")!=0) && (strcmp(dir->d_name, ".")!=0)){
          char filename[100] = "";
          strcpy(filename, dir->d_name);
          char *name = strtok(filename, delim1);
          char makefolder[100] = "/home/hanifa/modul2/petshop/";
          strcat(makefolder, name);
	  wait(NULL);
	  pid_t folder_id;
	  folder_id = fork();
	    if (folder_id<0){
	       exit(EXIT_FAILURE);
	    }
	    if (folder_id == 0){
	       char *argvC[] = {"mkdir", "-p", makefolder, NULL};
               execv("/bin/mkdir", argvC);
	    }
       }
    }
    closedir(d);
}


void move2c(){
    DIR *d;
    struct dirent *dir;
    d = opendir("/home/hanifa/modul2/petshop");
    char *delim1 = ";";
    char *delim2 = "_";
     while ((dir = readdir(d)) != NULL) {
       if ((dir->d_type != DT_DIR) && (strcmp(dir->d_name, "..")!=0) && (strcmp(dir->d_name, ".")!=0)){
	 
	  char filename[100] = "";
          strcpy(filename, dir->d_name);
	  char pindah[100] = "/home/hanifa/modul2/petshop/";	  
   	  strcat(pindah, filename);
	  if (strstr(filename, delim2) != NULL ){
	     char namanya[100];
	     strcpy(namanya, filename); 
	     char *name2 = strtok(namanya, delim2);
	     name2 = strtok(NULL, delim2);
	     name2 = strtok(name2, delim1);
	     char makefolder[100] = "/home/hanifa/modul2/petshop/";
	     strcat(makefolder, name2);
             
	     wait(NULL);
	     pid_t move_id;
   	     move_id = fork();
	     if (move_id<0){
	        exit(EXIT_FAILURE);
	     }
	     if (move_id == 0){
	        char *argvD[] = {"cp", pindah, makefolder, NULL};
	        execv("/bin/cp", argvD);
	     }
	  }
	     char *name = strtok(filename, delim1);
	     char makefolder[100] = "/home/hanifa/modul2/petshop/";
	     strcat(makefolder, name);
	     wait(NULL);
	     pid_t move_id;
	     move_id = fork();
	     if (move_id<0){
	        exit(EXIT_FAILURE);
	     }
	     if (move_id == 0){
	        char *argvD[] = {"mv", "-f", pindah, makefolder, NULL};
	        execv("/bin/mv", argvD);
	     }	 
       }
    }
    closedir(d);
}

int main() {
    chdir("/home/hanifa/modul2");
    folder2a();
    wait(NULL);
    folder2b();
    wait(NULL);
    move2c();
    return 0;
}
