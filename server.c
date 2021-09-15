/*****************************************************
*File: client.c
*Author: Aiden Escamilla
*Procedues: 
* main	- The function opens it's server pipe, handles the new 
*	  client pipe, and tells the client if it recived a
*	  valid magic square or not.
*
* isMagicSquare  - This function takes the char array and determines
*		   if it is a magic square or not
*
*
******************************************************/

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

/*
*int main(int argc, char* argv[])
*Author: Aiden Escamilla
*Date: 15 September 2021
*
*
*Description: 
*
*
*
*Parameters
*	argc	I/P	int 	the number of arguments on the command line
*	argv	I/P	char*[]	The arguments of the command line
*	main	O/P	int	status code (not currently used)
*/
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

	//GetHostName from man pages, prints output
	gethostname(name, len);
	printf("\n");
	printf("Host name is: %s\n", name);

	//uname from man pages, print entire struct
	printf("\n");
	uname(&buffer);
	printf("uname system: %s\n", buffer.sysname);
	printf("uname node name: %s\n", buffer.nodename);
	printf("uname release: %s\n", buffer.release);
	printf("uname version: %s\n", buffer.version);
	printf("uname machine: %s\n", buffer.machine);
	printf("uname domain name: %s\n", buffer.__domainname);

	//char arrays to hold filenames
	char filename[MAXLEN], clientFifo[MAXLEN], magicSquare[MAGICNUM + 1];// line[MAXLEN];
	//hardcode and make server pipe every client should know
	sprintf(filename, "/tmp/%s", "aae180003");
	mkfifo(filename, 0666);
	//set pointer to file descriptor
	FILE *fp = fopen(filename, "r");

	//continue to look for client
	while(1){
		if(receive_string(fp, clientFifo)){	//if a client pings you, store the special client pipe that was sent
			FILE *client = fopen(clientFifo, "r");	//open private client communication pipe
			
			while(1)	// keep waiting for client to type magic square input
			{

			if(receive_string(client, magicSquare)){	//when client sends magic square as a char array, store it in array
/*This block was used for testing as during writting
*helpful for debugging if changing this later on
*				for(int i = 0; i < MAGICNUM; i++){
*					printf("%c ", magicSquare[i]);
*				}
*				printf("\n");
*

*				 printf("%d\n",isMagicSquare(magicSquare));
*/


				fclose(client); //open client pipe for sending back the result
				client = fopen(clientFifo, "w");
				char result[MAXLEN];
				

				if(isMagicSquare(magicSquare))	//calc if it's a magic square
					sprintf(result, "1");	//put true in result if it passed the function
				else
					sprintf(result, "0");	//else put false

				send_string(client, result);	//send result
				fclose(client);			//close client pipe, need this before the break statment;
				fclose(fp);	//close server pipe, maybe remove because clients close for us
				remove(filename);//clean up server pipe
//				break;	//break out of loop
				exit(0);	//end code				//could also use exit(0); for simple code
			}
			}
//			remove(clientFifo); //is this needed for pipe clean up?
		}
	}
}


/*
*
*bool isMagicSquare(char* magicSquare)
*Author: Aiden Ecamilla
*Date: 15 September, 2021
*
*
*Description: returns boolean result after determening if the 
*		given char array is or isnt a magic square. Uses for loops and additive sums to make sure
*		all the rows cols and diags match.
*
*Parameters: 	
*	magicSquare	I/P	char*	9 character array with magic square ints in it
*/
bool isMagicSquare(char *magicSquare){

//sums
int sumRow = 0, sumCol = 0, sumDiag = 0, sum = 0;
int square[3][3];	//2d array
int counter = 0;	//for indexing 1d array


//nested for loops to populate 2d int array from 1d char array
for(int i = 0; i < 3; i++){	//iterate rows
	for(int j = 0; j < 3; j++){	//iterate columns
		square[i][j] = (magicSquare[counter] - '0');	//subtract '0' to subtract proper ASCII value
//		printf("%d%d: %d\n", i,j, square[i][j]); used for testing
		counter++;
	}
}
//set sum to row1
for(int i = 0; i < 3; i++)
	sum += square[i][0];

//set row and col sum
for(int i = 0; i < 3; i++){	//iterate start, then middle, then end rows&cols 
	sumRow = 0;	//reset to 0 after every col and row
	sumCol = 0;
    for(int j = 0; j < 3; j++){	//iterate down the entire row/col that i is set to
	sumRow += square[i][j];	//track sum using +=
	sumCol += square[j][i];
    }
	if(sumRow != sum){return false;}	//check each row matches sum
	if(sumCol != sum){return false;}	//check each col matches sum
}
//add diagonal values
for(int i = 0; i < 3; i++)
	sumDiag += square[i][i];
//check if it equals sum
if(sumDiag != sum){return false;}
sumDiag = 0;
//checl opposite diagonal
for(int i = 0; i < 3; i++){
	int j = 2;
	sumDiag += square[i][j--];
}
//check if equal to sum
if(sumDiag != sum){return false;}
//if it passed all the test, must be a true magic square
return true;
}
