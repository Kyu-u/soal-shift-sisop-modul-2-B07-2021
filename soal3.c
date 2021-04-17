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
#include <sys/prctl.h>

#define KILL "\
#!/bin/bash\n\
pkill soal3\n\
rm Killer.sh\n\
"

// #define TERM "\
// #!/bin/bash\n\
// pkill soal3\n\
// rm Killer.sh\n\
// "

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
    int flag = 1;

    //jika argumen tidak benar
    if (argc != 2)
    {
        printf("Argumen salah!\nMasukkan \"-z\" sebagai argumen 1 atau \"-x\" sebagai argumen 2!\n");

        exit(0);
    }

    // int status0;

    if (strcmp(argv[1], "-z") == 0)
    {
        //Killer bash program
        FILE *fp = NULL;
        fp = fopen("Killer.sh", "w");
        fputs(KILL, fp);
        fclose(fp);
    }
    if (strcmp(argv[1], "-x") == 0)
    {
        //Killer bash program
        FILE *fp = NULL;
        fp = fopen("Killer.sh", "w");
        fprintf(fp, "#!/bin/bash\nkill %d\n", getpid() + 1);
        fclose(fp);

        flag = 0;
    }

    else
    {
        printf("Argumen salah!\nMasukkan \"-z\" sebagai argumen 1 atau \"-x\" sebagai argumen 2!\n");

        exit(0);
    }

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

        // int statusA;
        int statusA, statusB, statusC, statusTemp;
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

        // sleep(1);

        pid_t temp;
        while (wait(&statusTemp) > 0)
            ;
        temp = fork();

        if (temp < 0)
        {
            exit(EXIT_FAILURE);
        }
        if (temp == 0)
        {
            pid_t pidB;
            while ((wait(&statusA)) > 0)
                ;

            //masuk ke direktori yang telah dibuat
            chdir(stringTime);

            // int statusC;

            for (int i = 0; i < 10; i++, sleep(5))
            {
                pidB = fork();
                if (pidB < 0)
                {
                    exit(EXIT_FAILURE);
                }
                if (pidB == 0)
                {
                    //mendapatkan waktu saat mendownload gambar
                    char stringTime2[sizeof "YYYY-MM-DD_HH:MM:SS"];
                    time_t now = time(NULL);
                    struct tm now_tm = *localtime(&now);
                    struct tm then_tm = now_tm;
                    // then_tm.tm_sec -= 1;
                    mktime(&then_tm);
                    strftime(stringTime2, sizeof(stringTime2), "%Y-%m-%d_%X", &then_tm);

                    char url[40];
                    //modifikasi string url agar bisa download file sesuai kriteria
                    sprintf(url, "https://picsum.photos/%ld", (now % 1000) + 50);

                    //printf("\n\nepoch = %ld\n\n", (rawtime2 % 1000) + 50);
                    char *argv[] = {"wget", url, "-O", stringTime2, NULL};
                    execv("/usr/bin/wget", argv);

                    // sleep(5);
                }
            }

            pid_t pidC;
            while (wait(&statusB) > 0)
                ;

            char statusMessage[] = {"Download Success"};
            //caesar cypher 5
            caesarShift(statusMessage, 5);
            //printf("\n\n%s\n\n", status);

            pidC = fork();
            //kembali ke direktori sebelumnya
            chdir("..");

            if (pidC < 0)
            {
                exit(EXIT_FAILURE);
            }
            if (pidC == 0)
            {
                //masukkan kedalam file
                FILE *fp = NULL;
                fp = fopen("status.txt", "w");
                fprintf(fp, "%s", statusMessage);
                fclose(fp);

                //melakukan zip direktori stringTime dengan format nama zipName
                char *argv[] = {"zip", zipName, "-r", stringTime, NULL};
                execv("/usr/bin/zip", argv);
            }
            pid_t pidD;
            while (wait(&statusC) > 0)
                ;
            // pidD = fork();
            // if (pidD < 0)
            // {
            //     exit(EXIT_FAILURE);
            // }
            // if (pidD == 0)
            // {
            //melakukan remove direktori sebelumnya
            char *argv[] = {"rm", "-r", stringTime, NULL};
            execv("/usr/bin/rm", argv);
            // }
        }
        sleep(40);
    }
}