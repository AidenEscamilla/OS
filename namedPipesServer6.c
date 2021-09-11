#include "util6.h"

int main() {
	char filename[MAXLEN], line[MAXLEN], serverfifo[MAXLEN], clientfifo[MAXLEN];
	sprintf(filename, "/tmp/%s", getenv("USER"));
	mkfifo(filename, 0666); 
	FILE *fp = fopen(filename, "r");
	//try to create just in case if it is not already there


	char words[84095][20];
        FILE *readIn = fopen("dictionary.txt", "r");
        int i = 0;
        while(fgets(line, sizeof(line), readIn)){
                strcpy(words[i], line);
                i++;
        }

	srand(time(0));
        int guess = rand() * 85000  % 84095;
	if(guess < 0)
		guess *= -1;




	while (1) {
		//new client request
		if (receive_string(fp, clientfifo)) {
			if (fork() > 0) //child can handle the transaction
				continue;	//parent can go and wait for the next client

			//create a new specific pipe for the client to write back
			sprintf(serverfifo, "/tmp/%s-%d", getenv("USER"), getpid());
			mkfifo(serverfifo, 0666);

			//write to client's fifo pipe
			FILE *fp2 = fopen(clientfifo, "w");
			send_string(fp2, serverfifo);

			FILE *fp3 = fopen(serverfifo, "w");

                                        char word[20];
                                        char hide[20];
					char already[20];
                                        for(int j = 0; j < strlen(words[guess]); j++){
                                            word[j] = words[guess][j];
                                            hide[j] = '*';
					    already[j] = ' ';
                                        }
                                        hide[strlen(words[guess]) - 1] = '\0';
                                        fprintf(stderr, "word: %s\n", word);
                                        fprintf(stderr, "hide: %s\n", hide);

                                send_string(fp3, hide);
				fclose(fp3);

			int missCounter = 0;
			while(1){
			    //read the final response from the client
				fp3 = fopen(serverfifo, "r");
					
				if(receive_string(fp3, line)){

					
					bool gameOver = true;
					bool inWord = false;

					fclose(fp3);
					fp3 = fopen(serverfifo, "w");

                                        for(int h = 0; h < strlen(words[guess]); h++){
						if(line[0] == already[h]){
						    fprintf(stderr, "%s is already in the word\n", line);
						    	gameOver = false;
							break;//skip and try again
						}
						if(line[0] == words[guess][h]){
							hide[h] = words[guess][h];
							already[h] = words[guess][h];
							inWord = true;
						}
						if(h == strlen(words[guess]) - 1 && !inWord){
						    fprintf(stderr, "%s is not in the word\n", line);
						    missCounter++;
						}
						if(hide[h] == '*')
							gameOver = false;
					}

					if(gameOver){//if the game is over
						char final[100];
						sprintf(final,"the word is %s. You missed %d time(s)" ,words[guess], missCounter);
						send_string(fp3, final);
						exit(0);
					}

					send_string(fp3, hide);
					fflush(fp3);
				}

			
			    fclose(fp3);
			}
			//it is time for clean up!
			fclose(fp2);
			fclose(fp3);
			remove(serverfifo);
			exit(0);
		}
	}
	fclose(fp);
}
