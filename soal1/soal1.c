#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <wait.h>
#include <time.h>

int main() {
    pid_t pid, sid;        // Variabel untuk menyimpan PID
    pid_t child_pid,child_pid2;
    pid_t child_id;
    pid_t child_id1;
    pid_t child_id2;
    pid_t child_id3;
    pid_t child_id4;
    
    
    int status=0;
    pid = fork();     // Menyimpan PID dari Child Process

    /* Keluar saat fork gagal
    * (nilai variabel pid < 0) */
    //kode template untuk proses daemon
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
    * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/home/zelda/sisopku")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    time_t now = time(NULL);
    time_t times = time(NULL);
    int batas = 1;
    while(1){
        // Tulis program kalian di sini
        now = time(NULL);
        //melakukan timecheck apakah sudah kurang dari 6 jam
        if(now >= 1617960120  && batas == 1){
            batas++;
            //membuat directory
            child_id = fork();
            if (child_id == 0){
                //membuat direktori baru
                char *argv[] = {"mkdir","Fylm","Musyik","Pyoto",NULL};
                execv("/bin/mkdir", argv); 
            
            //No. 1b
            //mendownload dari link
            }else{
                child_id1 = fork();
                if (child_id1 == 0) {
                    child_id2 = fork();
                    if (child_id2 == 0){
                        char *download[]=  { "wget", "-q","--no-check-certificate","https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download", "-O","Film_for_Stevany.zip",NULL};
                        execv("/bin/wget", download);
                    }
                    else {
                        while ((wait(&status)) > 0);
                        sleep (10);
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
                //No. 1c dan 1d
                }else{
                    while ((wait(&status)) > 0);
                    char* unzip[][10]= {{"unzip","-j","Film_for_Stevany.zip","-d","./Fylm",NULL},{"unzip","-j","Musik_for_Stevany.zip","-d","./Musyik",NULL},{"unzip","-j","Foto_for_Stevany.zip","*.jpg","-d","./Pyoto",NULL}};
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
        }else if(now >= 1617981718){
            //kode setelah jam d-day
            child_pid2 = fork();
            if(child_pid2 == 0){
                char *zipAkhir[]= {"zip","-r","Lopyu_Stevany.zip","Fylm","Musyik","Pyoto",NULL};
                execv("/bin/zip", zipAkhir); 
            }else{
                while ((wait(&status)) > 0);
                char *hapus[]= {"rm","-r","Fylm","Musyik","Pyoto",NULL};
                execv("/bin/rm", hapus); 
                exit(1);
            }
        }
        sleep(30);
    }
}

