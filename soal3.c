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

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

void caesarShift(char word[], int key)
{
    char ch;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        ch = word[i];

        if (ch >= 'a' && ch <= 'z')
        {
            ch = ch + key;

            if (ch > 'z')
            {
                ch = ch - 'z' + 'a' - 1;
            }

            word[i] = ch;
        }
        else if (ch >= 'A' && ch <= 'Z')
        {
            ch = ch + key;

            if (ch > 'Z')
            {
                ch = ch - 'Z' + 'A' - 1;
            }

            word[i] = ch;
        }
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
        //untuk mendapatkan waktu saat program dieksekusi
        time_t rawtime;
        struct tm *timeinfo;
        char stringTime[sizeof "YYYY-MM-DD_HH:MM:SS"];
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(stringTime, sizeof(stringTime), "%Y-%m-%d_%X", timeinfo);

        //membuat string dengan nama file untuk melakukan zip
        char zipName[40];
        strcpy(zipName, stringTime);
        strcat(zipName, ".zip");

        pid_t pidA;
        pidA = fork();
        if (pidA < 0)
        {
            exit(EXIT_FAILURE);
        }
        if (pidA == 0)
        {
            //membuat direktori baru dengan nama waktu yang telah ditentukan
            char *argv[] = {"mkdir", stringTime, NULL};
            execv("/bin/mkdir", argv);
        }

        sleep(1);

        pid_t pidB;
        pidB = fork();

        if (pidB < 0)
        {
            exit(EXIT_FAILURE);
        }
        if (pidB == 0)
        {
            //masuk ke direktori yang telah dibuat
            chdir(stringTime);

            int statusC;

            for (int i = 0; i < 10; i++)
            {
                //mendapatkan waktu saat mendownload gambar
                time_t rawtime2;
                struct tm *timeinfo2;
                timeinfo2->tm_sec -= 1;
                mktime(&timeinfo2);
                char stringTime2[sizeof "YYYY-MM-DD_HH:MM:SS"];
                time(&rawtime2);
                timeinfo2 = localtime(&rawtime2);
                strftime(stringTime2, sizeof(stringTime2), "%Y-%m-%d_%X", timeinfo2);

                char url[40];
                //modifikasi string url agar bisa download file sesuai kriteria
                sprintf(url, "https://picsum.photos/%ld", (rawtime2 % 1000) + 50);

                //printf("\n\nepoch = %ld\n\n", (rawtime2 % 1000) + 50);

                pid_t pidC;
                pidC = fork();
                if (pidC < 0)
                {
                    exit(EXIT_FAILURE);
                }
                if (pidC == 0)
                {
                    //mendownload gambar dari url yang telah dibuat
                    char *argv[] = {"wget", url, "-O", stringTime2, NULL};
                    execv("/usr/bin/wget", argv);
                }
                sleep(5);
            }

            while (wait(&statusC) > 0)
                ;

            char status[] = {"Download Success"};
            //caesar cypher 5
            caesarShift(status, 5);
            //printf("\n\n%s\n\n", status);

            //masukkan kedalam file
            FILE *fp = NULL;
            fp = fopen("status.txt", "w");
            fputs(status, fp);
            fclose(fp);

            //kembali ke direktori sebelumnya
            chdir("..");

            int statusD;

            pid_t pidD;
            pidD = fork();

            if (pidD < 0)
            {
                exit(EXIT_FAILURE);
            }
            if (pidD == 0)
            {
                //melakukan zip direktori stringTime dengan format nama zipName
                char *argv[] = {"zip", zipName, "-r", stringTime, NULL};
                execv("/usr/bin/zip", argv);
            }

            while (wait(&statusD) > 0)
                ;

            pid_t pidE;
            pidE = fork();

            if (pidE < 0)
            {
                exit(EXIT_FAILURE);
            }
            if (pidE == 0)
            {
                //melakukan remove direktori yang tidak dizip
                char *argv[] = {"rm", "-r", stringTime, NULL};
                execv("/usr/bin/rm", argv);
            }
        }
        sleep(39);
    }
}