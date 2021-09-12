#include <stdio.h>
#include <unistd.h>
//#include <errno.h>
#include <limits.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


#define MAXLEN 100
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


	char filename[MAXLEN], clientFifio[MAXLEN], serverFifo[MAXLEN], line[MAXLEN];

	sprintf(filename, "/tmp/%s", "aae180003");
	mkfifo(filename, 0666);
	
	FILE *fp = fopen(filename, "r");


	while(1){


		if(receive_string(fp, line)){
			fclose(fp);
			fp = fopen(filename, "r");
		}

	}

}
