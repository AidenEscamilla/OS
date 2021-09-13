#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
//#include <errno.h>
#include <limits.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


#define MAXLEN 100
#define MAGICNUM 9


bool isMagicSquare(char *magicSquare);

int main(int argc, char *argv[]){

	//these are the variables in the gethostname() man pages
	//had issues with functions returning error
	//HOST_NAME_MAX + 1 fixed the error by giving space for the string
	size_t len = HOST_NAME_MAX + 1;
	char name[len];

	//struct is used for the uname function, taken from the man pages
	struct utsname buffer;

	//This is the getdomainname function from the man pages
	//then it prints the name
	getdomainname(name, len);
	printf("\n");
	printf("Domain name is: %s", name);


	gethostname(name, len);
	printf("\n");
	printf("Host name is: %s", name);


	printf("\n");
	uname(&buffer);
	printf("uname system: %s\n", buffer.sysname);
	printf("uname node name: %s\n", buffer.nodename);
	printf("uname release: %s\n", buffer.release);
	printf("uname version: %s\n", buffer.version);
	printf("uname machine: %s\n", buffer.machine);
	printf("uname domain name: %s\n", buffer.__domainname);


	char filename[MAXLEN], clientFifo[MAXLEN], magicSquare[MAGICNUM + 1];// line[MAXLEN];

	sprintf(filename, "/tmp/%s", "aae180003");
	mkfifo(filename, 0666);
	
	FILE *fp = fopen(filename, "r");


	while(1){
		if(receive_string(fp, clientFifo)){
			FILE *client = fopen(clientFifo, "r");
			
			while(1)
			{

			if(receive_string(client, magicSquare)){
				for(int i = 0; i < MAGICNUM; i++){
					printf("%c ", magicSquare[i]);
				}
				printf("\n");


//				 printf("%d\n",isMagicSquare(magicSquare));
				fclose(client);
				client = fopen(clientFifo, "w");
				char result[MAXLEN];
				

				if(isMagicSquare(magicSquare))
					sprintf(result, "1");
				else
					sprintf(result, "0");

				send_string(client, result);
				break;
				fclose(client);
			}
			fclose(fp);
			fp = fopen(filename, "r");
			}
//			remove(clientFifo);
//			fclose(fp);
		}

	}

}


bool isMagicSquare(char *magicSquare){

int sumRow = 0, sumCol = 0, sumDiag = 0, sum = 0;
int square[3][3];
int counter = 0;

for(int i = 0; i < 3; i++){
	for(int j = 0; j < 3; j++){
		square[i][j] = (magicSquare[counter] - '0');
		printf("%d%d: %d\n", i,j, square[i][j]);
		counter++;
	}
}
//set sum to row1
for(int i = 0; i < 3; i++)
	sum += square[i][0];

//set row and col sum
for(int i = 0; i < 3; i++){
	sumRow = 0;
	sumCol = 0;
    for(int j = 0; j < 3; j++){
	sumRow += square[i][j];
	sumCol += square[j][i];
    }
	if(sumRow != sum){return false;}
	if(sumCol != sum){return false;}
}

for(int i = 0; i < 3; i++)
	sumDiag += square[i][i];

if(sumDiag != sum){return false;}
sumDiag = 0;

for(int i = 0; i < 3; i++){
	int j = 2;
	sumDiag += square[i][j--];
}
printf("second diag: %d\n", sumDiag);

if(sumDiag != sum){return false;}

return true;
}
