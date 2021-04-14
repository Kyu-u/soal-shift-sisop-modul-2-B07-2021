#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int main()
{
    pid_t pid, sid; // Variabel untuk menyimpan PID

    pid = fork(); // Menyimpan PID dari Child Process

    /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
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

    if ((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        pid_t pid[5];

        for (int i = 0; i < 4; i++)
        {
            pid[i] = fork();

            if (pid[i] == 0)
            {
                if (i == 1)
                {
                    //untuk mendapatkan waktu saat program dieksekusi
                    time_t rawtime;
                    struct tm *timeinfo;
                    char stringTime[sizeof "YYYY-MM-DD_HH:MM:SS"];
                    time(&rawtime);
                    timeinfo = localtime(&rawtime);
                    strftime(stringTime, sizeof(stringTime), "%Y-%m-%d_%X", timeinfo);

                    //buat direktori 3a
                    char *argva[3] = {"mkdir", stringTime, NULL};
                    execv("/bin/mkdir", argva);
                    sleep(40);
                    exit(0);
                }
                if (i == 2)
                {
                    exit(0);
                }
                if (i == 3)
                {
                    exit(0);
                }
            }
            if (pid[i] < 0)
            {
                perror("Something went wrong with the fork");
                exit(1);
            }
        }
    }
}