#include "segdef.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>

int shmid;
int semid;
segment *seg;

struct sembuf sop;

void initialisation(void)
{
    shmid =shmget(cle, segsize, 0);
    if (shmid == -1) {
        perror("Erreur shmget");
        exit(1);
    }
    semid = semget(cle, 3, 0);
    if (semid == -1) {
        perror("Erreur semget");
        exit(1);
    }
    seg = (segment *) shmat(shmid, NULL, 0);
    if (seg == (void *) -1) {
        perror("Erreur shmat");
        exit(1);
    }

    init_rand();
}

void send_request(int num)
{
    long sum = 0;
    long client_avg;
    long serv_avg;
    int pid_r, req_r;

    acq_sem(semid, seg_dispo);

    seg->pid = getpid();
    seg->req = num;
    for (int i = 0; i < maxval; i++) {
        seg->tab[i] = getrand();
        sum += seg->tab[i];
    }
    client_avg = sum / maxval;

    acq_sem(semid, seg_init);

    wait_sem(semid, res_ok);

    serv_avg = seg->result;
    pid_r = seg->pid;
    req_r = seg->req;

    lib_sem(semid, seg_init);
    wait_sem(semid, res_ok);
    lib_sem(semid, seg_dispo);
    //Affichage des resultats
    printf("\nRequete %d\n", num);
    printf("PID client : %ld\n", (long)getpid());
    printf("client Average  : %ld\n", client_avg);
    printf("server Average : %ld\n", serv_avg);

    if (pid_r == getpid() &&req_r == num &&client_avg == serv_avg) {
        printf("Resultat : OK\n");
    } else {
        printf("Resultat : ERREUR\n");
    }
}

int main(int argc, char *argv[])
{

    initialisation();
    send_request(1);
    if (shmdt(seg) == -1) {
        perror("shmdt");
        return 1;
    }

    return 0;
}