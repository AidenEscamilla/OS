#include "util6.h"
bool send_string(FILE *fp, char *str) {
	if (fprintf(fp, "%s\n", str)) {
		printf("Sent %s\n", str);
		fflush(fp);
		return true;
	} 
	return false;
}

bool receive_string(FILE *fp, char *newStr) {
	char line[MAXLEN];
	if (fgets(line, MAXLEN, fp) == NULL)
		return false;
	strcpy(newStr, strtok(line, "\n"));
	printf("Received %s\n", newStr);
	return true;
}