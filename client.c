#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXLEN 100

int main(int argc, char * argv[]){

	char filename[MAXLEN], serverFifo[MAXLEN], clientFifo[MAXLEN];

	sprintf(filename, "/tmp/%s", "aae180003");
	sprintf(clientFifo, "/tmp/%s-%d", "aae180003", getpid());
	mkfifo(clientFifo, 0666);
	
	FILE *fp = fopen(filename, "w");
	send_string(fp, clientFifo);
	fclose(fp);
	remove(clientFifo);
//	fprintf(fp, "hello server im a client\n");
//	fflush(fp);
//	fclose(fp);

}
