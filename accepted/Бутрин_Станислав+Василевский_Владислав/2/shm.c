#include <stdio.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
	
int main (){
	int segment_id;
	char* shared_memory;
	struct shmid_ds shmbuffer;
	int segment_size;
	const int shared_segment_size = 0x6400;
    srand(time(NULL));
  
	
	/* Выделить сегмент  общей памяти. */
	segment_id = shmget (IPC_PRIVATE, shared_segment_size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	
	/* Подключить сегмент общей памяти.  */
	shared_memory = (char*) shmat (segment_id, 0, 0);
	printf ("Общая память подключена по адресу %p\n", shared_memory);
	
	/* Определить размер сегмента. */
	shmctl (segment_id, IPC_STAT, &shmbuffer);
	segment_size = shmbuffer.shm_segsz;
	printf ("Размер сегмента: %d\n", segment_size);
	
	/* Написать строку в сегмент общей памяти. */
	sprintf (shared_memory, "%d", rand()%42);
	/* Отключить сегмент общей памяти. */ 
	shmdt (shared_memory);
	
	/* Снова подключить сегмент общей памяти, по различным адресам. */ 
	shared_memory = (char*) shmat (segment_id, (void*) 0x5000000, 0);
	printf ("Общая память переподключена по адресу %p\n", shared_memory);
	

    char* args[] = {shared_memory, NULL };
    if(fork()==0){
    execve("p2.out", args, __environ);
    }
    if(fork()==0){
    execve("p1.out", args, __environ);}
	
	/* Отключить сегмент общей памяти. */ 
	//shmdt (shared_memory);
	
	/* Освободить сегмент общей памяти.  */
	//shmctl (segment_id, IPC_RMID, 0);
	return 0;
}
