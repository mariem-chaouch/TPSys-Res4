#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>

static unsigned short random_state[3];

void init_rand(void)
{
    srand48(time(NULL) ^ getpid());
}

long getrand(void)
{
    return lrand48();
}

void semcall(int semid, int sem,int op)
{
    struct sembuf sop;

    sop.sem_num = sem;
    sop.sem_op = op;
    sop.sem_flg = 0;

    if (semop(semid, &sop, 1) == -1) {
        perror("acq_sem");
        exit(1);
    }
}

void wait_sem(int semid, int sem)
{
    semcall(semid, sem, 0);
}
void acq_sem(int semid, int sem)
{
    semcall(semid, sem, -1);
}
void lib_sem(int semid, int sem)
{
    semcall(semid, sem, 1);
}