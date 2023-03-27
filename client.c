#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#define SEMNAMEONE "/sem-1001-start"
#define SEMNAMETWO "/sem-1001-done"

int main (int argc, char *argv[]){

	sem_t* semStart = sem_open(SEMNAMEONE, 0);
	sem_t* semDone = sem_open(SEMNAMETWO, 0);

	float val[10] = { 1.0 };
	
	// get the key to the file
	key_t key = ftok("shared_1000.txt", 0);
	if (key == -1){
		return -1;
	}

	// find the block or create it if it doesn't exist
	int shm_id = shmget(key, sizeof(float)*10, 0644 | IPC_CREAT);
	if (shm_id == -1){
		return -1;
	}

	// attack block to virtual memory
	float *data = shmat(shm_id, NULL, 0);
	if (data == (float *)(-1)){
		return -1;
	}

	while(1){
		sem_wait(semStart);
		// copy some values to the block
		memcpy(val, data, 10*sizeof(float));
		for (int j = 0; j < 10; j++)
			printf("%f\n", val[j]);
		printf("\n");
		sem_post(semDone);
	}

	// detach
	shmdt(data);

	return 0;
}
