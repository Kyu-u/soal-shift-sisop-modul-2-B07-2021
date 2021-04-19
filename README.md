# soal-shift-sisop-modul-2-B07-2021
## Anggota Kelompok
* Christian Bennett Robin 05111940000078
* Zelda Elma Sibuea 05111940000038
* Hanifa Fauziah 05111940000024

## Soal 1

## Soal 2

## Soal 3

### 3A ###

Ranora harus membuat sebuah program C yang dimana setiap 40 detik membuat sebuah direktori dengan nama sesuai timestamp [YYYY-mm-dd_HH:ii:ss].

```c
while (1)
    {
        //untuk mendapatkan waktu saat program dieksekusi
        time_t rawtime;
        struct tm *timeinfo;
        char stringTime[sizeof "YYYY-MM-DD_HH:MM:SS"];
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(stringTime, sizeof(stringTime), "%Y-%m-%d_%X", timeinfo);

        if (fork() == 0)
        {
            //membuat direktori baru dengan nama waktu yang telah ditentukan
            char *argv[] = {"mkdir", stringTime, NULL};
            execv("/bin/mkdir", argv);
        }
        ...
     }
```
Pertama-tama kami mendapatkan waktu dengan menggunakan `time_t` lalu kami membuat string `stringTime` yang berisi waktu yang telah kami cari. String tersebut dapat berisi waktu dari `time_t` dengan menggunakan fungsi `strftime`.

Setelah itu kami melakukan fork proses baru untuk membuat direktori baru menggunakan
```c
char *argv[] = {"mkdir", stringTime, NULL};
            execv("/bin/mkdir", argv);
```

### 3B ###

Setiap direktori yang sudah dibuat diisi dengan 10 gambar yang didownload dari https://picsum.photos/, dimana setiap gambar akan didownload setiap 5 detik. Setiap gambar yang didownload akan diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss] dan gambar tersebut berbentuk persegi dengan ukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix.

```c
sleep(1);

        if (fork() == 0)
        {
            //masuk ke direktori yang telah dibuat
            chdir(stringTime);

            for (int i = 0; i < 10; i++)
            {
                time_t rawtime2;
                struct tm *timeinfo2;
                char stringTime2[sizeof "YYYY-MM-DD_HH:MM:SS"];
                time(&rawtime2);
                timeinfo2 = localtime(&rawtime2);
                // strftime(stringTime2, sizeof(stringTime2), "%Y-%m-%d_%X", timeinfo2);

                time_t now = time(NULL);
                struct tm now_tm = *localtime(&now);
                struct tm then_tm = now_tm;
                then_tm.tm_sec -= 1;
                mktime(&then_tm);
                strftime(stringTime2, sizeof(stringTime2), "%Y-%m-%d_%X", &then_tm);

                char url[40];
                //modifikasi string url agar bisa download file sesuai kriteria
                sprintf(url, "https://picsum.photos/%ld", ((now - 1) % 1000) + 50);

                if (fork() == 0)
                {
                    char *argv[] = {"wget", url, "-O", stringTime2, NULL};
                    execv("/usr/bin/wget", argv);
                }
                sleep(5);
            }
            
            ...
            
        }
```

Hal ini dilakukan dengan cara melakukan fork proses baru, namun sebelum membuat fork, kami menambahkan `wait(1)` dulu agar proses pertama soal 3A tidak terhambat saat pengerjaan soal berikutnya. 

Untuk isi dari proses yang dibuat itu sendiri, pertama-tama kami melakukan `chdir(stringTime)` untuk masuk ke dalam direktori yang telah dibuat di soal 3A. Lalu kami lakukan dalam loop sebanyak 10x sesuai permintaan soal. Untuk mendapatkan waktu yang sama lagi, kami modifikasi waktunya sedikit menggunakan `then_tm.tm_sec -= 1` karena pada awalnya kami menggunakan `sleep(1)`, yang menyebabkan proses menjadi 1 detik terlambat. `then_tm.tm_sec -= 1` ini digunakan untuk mengurangi detiknya selama 1 detik agar seolah-olah sleepnya tidak terjadi.

Setelah itu kami membuat string `url` dan hasil waktu yang telah didapatkan dimasukkan ke dalam situ agar dapat kami lakukan proses fork untuk mendownload setiap gambar menggunakan
```
char *argv[] = {"wget", url, "-O", stringTime2, NULL};
                    execv("/usr/bin/wget", argv);
```

Proses ini berjalan setiap 5 detik, maka kami tambahkan  `sleep(5)` pada akhir loop.

### 3C ###

Setelah direktori telah terisi dengan 10 gambar, program tersebut akan membuat sebuah file “status.txt”, dimana didalamnya berisi pesan “Download Success” yang terenkripsi dengan teknik Caesar Cipher dan dengan shift 5. Caesar Cipher adalah Teknik enkripsi sederhana yang dimana dapat melakukan enkripsi string sesuai dengan shift/key yang kita tentukan. Misal huruf “A” akan dienkripsi dengan shift 4 maka akan menjadi “E”. Karena Ranora orangnya perfeksionis dan rapi, dia ingin setelah file tersebut dibuat, direktori akan di zip dan direktori akan didelete, sehingga menyisakan hanya file zip saja.

```c
char statusMessage[] = {"Download Success"};
            //caesar cypher 5
            caesarShift(statusMessage, 5);
            //printf("\n\n%s\n\n", status);

            //masukkan kedalam file
            FILE *fp = NULL;
            fp = fopen("status.txt", "w");
            fprintf(fp, "%s", statusMessage);
            fclose(fp);

            //kembali ke direktori sebelumnya
            chdir("..");

            //membuat string dengan nama file untuk melakukan zip
            char zipName[40];
            strcpy(zipName, stringTime);
            strcat(zipName, ".zip");

            //melakukan zip direktori stringTime dengan format nama zipName
            char *argv[] = {"zip", "-rm", zipName, stringTime, NULL};
            execv("/usr/bin/zip", argv);
```

Pertama-tama kami inisialisasi string `statusMessage` dengan isi `Download Success` sesuai permintaan soal, lalu kami melakukan *Caesar Shift* dengan *key* 5 menggunakan fungsi `caesarShift(statusMessage, 5)` yang kami inisialisasi di awal program, yang berisi: 

```c
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
```

Setelah itu hasilnya kami masukkan kedalam file `status.txt` pada direktori yang sama.

Setelah selesai membuat file `status.txt`, kami melakukan `chdir(..)` untuk pindah ke direktori sebelumnya dan membuat string `zipName` sebagai format penamaan zipnya. Lalu kami melakukan proses zip dengan menggunakan
```c
char *argv[] = {"zip", "-rm", zipName, stringTime, NULL};
            execv("/usr/bin/zip", argv);
```

### 3D ###

Untuk mempermudah pengendalian program, pembimbing magang Ranora ingin program tersebut akan men-generate sebuah program “Killer” yang executable, dimana program tersebut akan menterminasi semua proses program yang sedang berjalan dan akan menghapus dirinya sendiri setelah program dijalankan. Karena Ranora menyukai sesuatu hal yang baru, maka Ranora memiliki ide untuk program “Killer” yang dibuat nantinya harus merupakan program bash.

```c
FILE *fp = NULL;
        fp = fopen("Killer.sh", "w");
        fputs(KILL, fp);
        fclose(fp);
```

Kami membuat file baru yang berisi script bash untuk mengeksekusi program.

### 3E ###

Pembimbing magang Ranora juga ingin nantinya program utama yang dibuat Ranora dapat dijalankan di dalam dua mode. Untuk mengaktifkan mode pertama, program harus dijalankan dengan argumen -z, dan Ketika dijalankan dalam mode pertama, program utama akan langsung menghentikan semua operasinya Ketika program Killer dijalankan. Sedangkan untuk mengaktifkan mode kedua, program harus dijalankan dengan argumen -x, dan Ketika dijalankan dalam mode kedua, program utama akan berhenti namun membiarkan proses di setiap direktori yang masih berjalan hingga selesai (Direktori yang sudah dibuat akan mendownload gambar sampai selesai dan membuat file txt, lalu zip dan delete direktori).

```c
if (argc == 2 && strcmp(argv[1], "-z") == 0)
    {
        //Killer bash program
        FILE *fp = NULL;
        fp = fopen("Killer.sh", "w");
        fputs(KILL, fp);
        fclose(fp);
    }
    else if (argc == 2 && strcmp(argv[1], "-x") == 0)
    {
        //Killer bash program
        FILE *fp = NULL;
        fp = fopen("Killer.sh", "w");
        fprintf(fp, "#!/bin/bash\nkill %d\nrm Killer.sh\n", getpid() + 1);
        fclose(fp);
    }

    else
    {
        printf("Argumen salah!\nMasukkan \"-z\" sebagai argumen 1 atau \"-x\" sebagai argumen 2!\n");

        exit(0);
    }
```

Kami menambahkan `if else` untuk setiap argumen, jika argumen berisi `-z`, maka file bash akan berisi KILL yang terdefinisi di awal program, yaitu

```c
#define KILL "\
#!/bin/bash\n\
pkill soal3\n\
rm Killer.sh\n\
"
```

Untuk argumen `-z` ini, program akan melakukan `pkill soal3` yaitu process kill soal3, dimana soal3 merupakan nama programnya. Program akan langsung selesai berjalan seadanya.

Jika argumen berisi `-x`, maka file bash akan berisi 
```c
fprintf(fp, "#!/bin/bash\nkill %d\nrm Killer.sh\n", getpid() + 1);
```

dimana program bash tersebut jika di run akan membunuh proses `getpid() + 1`, yaitu proses pembuatan direktori. Proses setelah-setelahnya masih akan dijalankan sampai selesai.

Untuk argumen yang selain dari kedua argumen diatas, kami menambahkan output bahwa argumen salah dan lansgung exit program.



