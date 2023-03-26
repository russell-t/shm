#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main (int argc, char *argv[]){

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

	// copy some values to the block
	memcpy(val, data, 10*sizeof(float));

	for (int i = 0; i < 10; i++)
		printf("%f\n", val[i]);

	// detach
	shmdt(data);

	return 0;
}
