/*****************************************************************************
*File: client.c
*Author: Aiden Escamilla
*Procedues: 
 *main	- Creates a client namedPipe and sends the name of the pipe to the
	  server. Then asks the user to input 9 single digits in row major
	  order for magic square testing. Sends the user inputted array 
	  to the server, and prints the result received.





******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXLEN 100
#define MAGICNUM 9



/**************************************************
*int main(int argc, char* argv[])
*Author: Aiden Escamilla
*Date: 14 September, 2021
*Description: Generates and creates a unique pipe
*	Sends the pipename to the server
*	Gets a user input of 9 digits
*	Puts them in an array and sends to server
*	Prints the result of the server if the digits are/aren't a magic square
*	Cleans up pipes
*Parameters:
*
*	argc  IP   int  	The number of arguments on the command line
*	argv  I/P  char*[]	The arguments on the command line
*	main  O/P   int		Status code (not currently used)
**************************************************/
int main(int argc, char * argv[]){
	//setup  char arrays to store pipe names and results
	char filename[MAXLEN], clientFifo[MAXLEN], result[MAXLEN];
	char magicSquare[MAGICNUM+1]; //+1 for null terminator

	sprintf(filename, "/tmp/%s", "aae180003");	//hardcoded server name
							//to connect to server
	sprintf(clientFifo, "/tmp/%s-%d", "aae180003", getpid());	//create special client pipe 
									//with same name as server
									//except add PID to the end, unique per client/process
	mkfifo(clientFifo, 0666);	//create client pipe
	
	FILE *fp = fopen(filename, "w");	//open serevr and write the name of client pipe to said server
	send_string(fp, clientFifo);		//function to facilitate sending (reused from past UNIX class with Prof. Jey)
	fclose(fp);				//close server pipe, not needed anymore


//prompt user and get 9 values  for magic square

	printf("Enter the magic square values (0-9 in Row major order): \n");

	for(int i = 0; i < MAGICNUM; i++){	//loop 9 times
		scanf("%c",&magicSquare[i]);
//		fgets();
		scanf("%c");	//get rid of user entered SINGLE white space
	}


	//open client specific pipe and send the array over 
	fp = fopen(clientFifo, "w");
	send_string(fp, magicSquare);
	fclose(fp);


	//open the client pipe and recieve string containing '0' or '1'
	fp = fopen(clientFifo, "r");
	receive_string(fp, result);
	fclose(fp);
	
	//print if recieved true
	if(result[0] == '1')
	printf("Proper magic square entered\n");
	else	//else false was receievd and print false
	printf("Not a magic square\n");



	remove(filename);	//cleans up server pipe 
	remove(clientFifo);	//cleans up client pipe
}
