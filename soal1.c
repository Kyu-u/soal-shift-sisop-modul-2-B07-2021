#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <time.h>
 
int main() {
   pid_t child_id;
    pid_t child_id1;
    pid_t child_id2;
    pid_t child_id3;
    pid_t child_id4;
    int status;
    
    //No. 1a
    child_id = fork();
    if (child_id == 0){
        char *argv[] = {"mkdir","Fylm","Musyik","Pyoto",NULL};
        execv("/bin/mkdir", argv); 
    
    //No. 1b
    }else{
        child_id1 = fork();
        if (child_id1 == 0) {
            child_id2 = fork();
            if (child_id2 == 0){
                char *download[]=  { "wget", "--no-check-certificate","https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download", "-O","Film_for_Stevany.zip",NULL};
                execv("/bin/wget", download);
            }
            else {
                while ((wait(&status)) > 0);
                child_id3 = fork();          
                if (child_id3 == 0){
                    char *download1[] = { "wget", "-q","--no-check-certificate","https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download", "-O","Musik_for_Stevany.zip",NULL};
                    execv("/bin/wget", download1);  
                }
                else {
                    while ((wait(&status)) > 0);
                    char *download2[]=  { "wget", "-q","--no-check-certificate","https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download", "-O","Foto_for_Stevany.zip",NULL};
                    execv("/bin/wget", download2);
                }


            } 
        //No. 1c 
        }else{
            while ((wait(&status)) > 0);
            char* unzip[][10]= {{"unzip","-j","Film_for_Stevany.zip","-d","./Fylm",NULL},{"unzip","-j","Musik_for_Stevany.zip","-d","./Musyik",NULL},{"unzip","-j","Foto_for_Stevany.zip","-d","./Pyoto",NULL}} ;

        //No.1d
            int i = -1;
            while (i<3)
            {
                i++;
                child_id4 = fork();
                if (child_id4 == 0) continue; 
                execv("/bin/unzip",unzip[i]);
                    
           
            }
        }
    }
}  
