#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

void daemonSkeleton()
{
    pid_t pid, sid;

    pid = fork();

    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    // if ((chdir("/")) < 0)
    // {
    //     exit(EXIT_FAILURE);
    // }

    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    // close(STDERR_FILENO);
}

void caesar5(char word[])
{
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        word[i] += 5;
    }
}

int main(int argc, char *argv[])
{
    int status;

    while (wait(&status) > 0)
        ;

    daemonSkeleton();

    while (1)
    {
        pid_t pidA, pidB, pidC, pidD, pidE;

        int statusA;

        //untuk mendapatkan waktu saat program dieksekusi
        time_t rawtime;
        struct tm *timeinfo;
        char stringTime[sizeof "YYYY-MM-DD_HH:MM:SS"];
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(stringTime, sizeof(stringTime), "%Y-%m-%d_%X", timeinfo);

        pidA = fork();
        if (pidA < 0)
        {
            exit(EXIT_FAILURE);
        }
        if (pidA == 0)
        {
            //buat direktori 3a
            char *argv[] = {"mkdir", stringTime, NULL};
            execv("/bin/mkdir", argv);
        }

        //soalB
        while (wait(&statusA) > 0)
            ;

        pidB = fork();

        if (pidB < 0)
        {
            exit(EXIT_FAILURE);
        }
        if (pidB == 0)
        {
            int statusB;

            chdir(stringTime);
            for (int i = 0; i < 10; i++)
            {
                char url[40];

                //mendapatkan waktu saat mendownload gambar
                time_t rawtime2;
                struct tm *timeinfo2;
                char stringTime2[sizeof "YYYY-MM-DD_HH:MM:SS"];
                time(&rawtime2);
                timeinfo2 = localtime(&rawtime2);
                strftime(stringTime2, sizeof(stringTime2), "%Y-%m-%d_%X", timeinfo2);
                //modifikasi string url agar bisa download file sesuai kriteria
                sprintf(url, "https://picsum.photos/%ld", (rawtime2 % 1000) + 50);

                pidC = fork();
                if (pidC < 0)
                {
                    exit(EXIT_FAILURE);
                }
                if (pidC == 0)
                {
                    char *argv[] = {"wget", url, "-O", stringTime2, NULL};
                    execv("/usr/bin/wget", argv);
                }
                sleep(5);
            }

            while (wait(&statusB) > 0)
                ;

            char status[] = {"Download Success"};
            //caesar cypher 5
            caesar5(status);

            //masukkan kedalam file
            FILE *ptr = NULL;
            ptr = fopen("status.txt", "w");
            fputs(status, ptr);
            fclose(ptr);

            //kembali ke direktori sebelumnya
            chdir("..");
        }
        sleep(40);
    }
}