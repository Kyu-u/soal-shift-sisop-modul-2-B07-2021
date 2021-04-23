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
	int status;
	pid_t pid1;
	pid1 = fork();
	if (pid1 < 0){
		exit(EXIT_FAILURE);
   	}
   	if (pid1 == 0) {
    	char *argvA[] = {"mkdir", "-p", "./petshop", NULL};
    	execv("/bin/mkdir", argvA);
   	}
   	while((wait(&status))>0);
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
	int status3;
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
	  		while((wait(&status3))>0);
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

void detail(char *asalnya, char *hewan){
	int status;
	char *delim1 = ";";
	char *jenis = strtok(hewan, delim1);
	char *nama = strtok(NULL, delim1);
	char *umur = strtok(NULL, delim1);
		
	char ket_path[300];
	sprintf(ket_path, "/home/hanifa/modul2/petshop/%s/keterangan.txt", jenis);
	
	char newname[300];
	sprintf(newname, "/home/hanifa/modul2/petshop/%s/%s.jpg", jenis, nama);
	
	char isinya[300];
	sprintf(isinya, "nama : %s \numur : %s\n\n", nama, umur);
	
	FILE *keterangan;
	keterangan = fopen(ket_path, "a"); //agar bisa membuka dan menulis txt
	if(keterangan){
		fprintf(keterangan, "%s", isinya);
		fclose(keterangan);
	}
	
	while((wait(&status))>0);
	pid_t move_id;
	move_id = fork();
	if (move_id<0){
		exit(EXIT_FAILURE);
	}
	if (move_id == 0){
		char *argvD[] = {"cp", asalnya, newname, NULL};
		execv("/bin/cp", argvD);
	}
}


void move2c(){
	int status2;
	DIR *d;
    struct dirent *dir;
    d = opendir("/home/hanifa/modul2/petshop");
    char *delim1 = ";";
	char *delim2 = "_";
	while ((dir = readdir(d)) != NULL) {
		if ((dir->d_type != DT_DIR) && (strcmp(dir->d_name, "..")!=0) && (strcmp(dir->d_name, ".")!=0)){
			char filename[300] = "";
    		strcpy(filename, dir->d_name);
	  		char asal[300] = "/home/hanifa/modul2/petshop/";	  
   	  		strcat(asal, filename);
   	  			
   	  		filename[strlen(filename)-4]='\0'; //hapus .jpg
   	  			
   	  		char *animal1 = strtok(filename, delim2);
   	  		char *animal2 = strtok(NULL, delim2);
   	  		
			if (animal2 != NULL ){ //kalau ada hewan ke-2
				detail(asal, animal2);
			}
			
			detail(asal, animal1);
			
			while((wait(&status2))>0);
			pid_t delete_id;
			delete_id = fork();
			if (delete_id<0){
				exit(EXIT_FAILURE);
			}
			if (delete_id == 0){
				char *argvE[] = {"rm", asal, NULL};
				execv("/bin/rm", argvE);
			}
       	}
    }
    closedir(d);
}

int main() {
    int statuss;
    chdir("/home/hanifa/modul2");
    folder2a();
    while((wait(&statuss))>0);
    folder2b();
    move2c();
    return 0;
}
