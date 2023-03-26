#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

int main (int argc, char *argv[]){

	float val[10] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0};
//	float val[10] = { 0.0 };
	
	// get the key to the file
	key_t key = ftok("shared_1000.txt", 0);
	if (key == -1){
		printf("key\n");
		return -1;
	}

	// find the block or create it if it doesn't exist
	int shm_id = shmget(key, sizeof(float)*10, 0644 | IPC_CREAT);
	if (shm_id == -1){
		printf("shmget\n");
		return -1;
	}

	// attack block to virtual memory
	float *data = shmat(shm_id, NULL, 0);
	if (data == (float *)(-1)){
		printf("shmat\n");
		return -1;
	}

	// copy some values to the block
	memcpy(data, val, 10*sizeof(float));

	// detach
	shmdt(data);

	return 0;
}
