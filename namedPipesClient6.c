#include "util6.h"

int main(int argc, char *argv[]) {
	char filename[MAXLEN], clientfifo[MAXLEN], serverfifo[MAXLEN], line[MAXLEN];
	sprintf(filename, "/tmp/%s", getenv("USER"));
	sprintf(clientfifo, "/tmp/%s-%d", getenv("USER"), getpid());
	mkfifo(clientfifo, 0666);

	FILE *fp = fopen(filename, "w");
	send_string(fp, clientfifo);
	fclose(fp);
	
	fp = fopen(clientfifo, "r");
	receive_string(fp, serverfifo);
	fclose(fp);
	remove(clientfifo);

	char errMessage[100];
	char hide[20];
//	printf("Enter a char: \n");
//	scanf("%c", line);
//	scanf("%c", temp);//catch extra \n

//	fp = fopen(serverfifo, "r");
//	receive_string(fp, line);

//	send_string(fp, line);
//	fflush(fp);
//	fclose(fp);

	while(1){
		fp = fopen(serverfifo, "r");

		if(receive_string(fp, hide)){

			fclose(fp);
			fp = fopen(serverfifo, "w");
			
			printf("(Guess) Enter a letter in word %s > ", hide);	
			
			scanf("%s", line);
			scanf("%c", hide);//catch extra \n
//			fprintf(stderr, "line: %s", line);


			if(strcmp(line, "") != 0){
				send_string(fp, line);
			}
			else
				send_string(fp, "Done.");
		}

		fclose(fp);
	}
	    
//	fclose(fp);
}
