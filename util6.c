/*
*File:  util6.c
*Author: Jey veerasam
*Procedures:
*
*
*send_string	-uses fprintf to send a string in the pipe and flushes the pipe after
*		for smooth sending 
*
*
*
*receive_string	-uses fgets to get the string from pipe
*
*
*/


#include "util6.h"

/**********************************
*
*bool send_string(FILE *fp, char *str)
*Author: Jey Veerasam
*Date: 2021 Spring Semester
*Description: puts the parameter str into the pipe 
		opened with file descriptor *fp and flushes to ensure
*		proper sending of messages
*
*
*Parameters:
*       fp      I/P     FILE*   - file descriptor for the open pipe
*       str  	O/P     char*   - string to be sent to the pipe
*
*
*********************************/
bool send_string(FILE *fp, char *str) {
	if (fprintf(fp, "%s\n", str)) {
		printf("Sent %s\n", str);
		fflush(fp);
		return true;
	} 
	return false;
}

/**********************************
*bool receive_string(FILE *fp, char *newStr)
*Author: Jey Veerasam
*Date: 2021 Spring Semester
*Description: Takes the string in pipe accessed by file descriptor fp
*		and puts it in line[MAXLEN]. Prints what was received
*
*Parameters:
*	fp	I/P	FILE*	-file descriptor for the open pipe
*	newStr	O/P	char*	-recieved string copied into here for printing output
*
*********************************/

bool receive_string(FILE *fp, char *newStr) {
	char line[MAXLEN];
	if (fgets(line, MAXLEN, fp) == NULL)
		return false;
	strcpy(newStr, strtok(line, "\n"));
	printf("Received %s\n", newStr);
	return true;
}
