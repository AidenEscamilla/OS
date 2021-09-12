#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXLEN 100
#define MAGICNUM 9

int main(int argc, char * argv[]){

	char filename[MAXLEN], clientFifo[MAXLEN];
	char magicSquare[MAGICNUM+1];
//	int magicSquare[9];
	sprintf(filename, "/tmp/%s", "aae180003");
	sprintf(clientFifo, "/tmp/%s-%d", "aae180003", getpid());
	mkfifo(clientFifo, 0666);
	
	FILE *fp = fopen(filename, "w");
	send_string(fp, clientFifo);
	fclose(fp);

	printf("ClientFifioBB: %s\n", clientFifo);


	printf("Enter 9 int values: \n");
//	fflush(stdin);
	for(int i = 0; i < MAGICNUM; i++){
		scanf("%c",&magicSquare[i]);
		scanf("%c");
//		printf("%c\n",magicSquare[i]);
	}

	printf("ClientFifioAA: %s\n", clientFifo);
	printf("MAGICSQUARE: %s\n", magicSquare);

	fp = fopen(clientFifo, "w");
	send_string(fp, magicSquare);
	fclose(fp);

//	remove(clientFifo);
//	fprintf(fp, "hello server im a client\n");
//	fflush(fp);
//	fclose(fp);

}
