#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdbool.h>

#define CUSTOMERS 0
#define BARBERS 1
#define MUTEX 2
#define CHAIRS 5

void up(int sem_id,int sem_num,struct sembuf *semaphore) {
    semaphore->sem_num=sem_num;
    semaphore->sem_op=1;
    semaphore->sem_flg=0;
    semop(sem_id,semaphore,1);
}

void down(int sem_id,int sem_num,struct sembuf *semaphore) {
    semaphore->sem_num=sem_num;
    semaphore->sem_op=-1;
    semaphore->sem_flg=0;
    semop(sem_id,semaphore,1);
}

void initSem(int sem_id,int sem_num,int val) {
    union semnum {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    }argument;
    argument.val=val;
    semctl(sem_id,sem_num,SETVAL,argument);
}

int main() {
    int sem_key=1111,shm_key=2222;
    int sem_id,shm_id;
    int *waiting;
    struct sembuf semaphore;
    int count=1;
    shm_id=shmget(shm_key,sizeof(int),IPC_CREAT|0666);
    sem_id=semget(sem_key,3,IPC_CREAT|0666);
    waiting=shmat(shm_id,NULL,0);
    *waiting=0;
    initSem(sem_id,CUSTOMERS,0);
    initSem(sem_id,BARBERS,0);
    initSem(sem_id,MUTEX,1);
    printf("There are %d chairs.\n",CHAIRS);
    if(fork()) {
        //Barber's part
        while(true) {
            down(sem_id,CUSTOMERS,&semaphore);
            down(sem_id,MUTEX,&semaphore);
            *waiting=*waiting-1;
            up(sem_id,BARBERS,&semaphore);
            up(sem_id,MUTEX,&semaphore);
            printf("Now barber cutting hair\n");
            sleep(6);   //cut hair
        }
    }
    else {
        //Customers part
        while(true) {
            sleep(1); //customer comes to barbershop
            down(sem_id,MUTEX,&semaphore);
            if(*waiting < CHAIRS) {
                printf("Customer %d enters barbershop.\n",count++);
                *waiting=*waiting+1;
                up(sem_id,CUSTOMERS,&semaphore);
                up(sem_id,MUTEX,&semaphore);
            }
            else {
                printf("Customer %d left the barbershop.\n",count++);
                up(sem_id,MUTEX,&semaphore);
            }
        }
    }
}