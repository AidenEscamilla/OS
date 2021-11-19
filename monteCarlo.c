// Program 3: montecarlo.c
// Programmer: Aiden Escamilla
// Professor Goodrum, CS 4348.002

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

int FIFO(int wss, int data[]);
int LRU(int wss, int data[]);
int Clock(int wss, int data[]);
int randNum(int mu, int  sigma);

int main()
{
int LRUResults[21], FIFOResults[21], ClockResults[21];
int data[1000];

for(int k = 0; k < 21; k++){
	LRUResults[k] = 0;
	FIFOResults[k] = 0;
	ClockResults[k] = 0;
}

for (int i = 0;i < 1000; i++) // Experiments loop
{
	for(int j = 0;j < 1000; j++){ //Trace loop{
		data[j] = ( 10 * ((int) ( j / 100 )) ) + randNum( 10, 2 );
		}

// Generate a random number from a normal distribution
// with a mean of ten and a standard deviation of two.
//randNum(10, 2);
// There are ten regions which have their own base address.


for(int wss = 4; wss <= 20; wss++)
	{
	// Determine and accumulate the number of page
	// faults for each algorithm base on the current
	// working set size and the current trace.
	LRUResults  [wss] += LRU( wss, data );
	FIFOResults [wss] += FIFO( wss, data );
	ClockResults[wss] += Clock( wss, data );
	}

}

for(int wss = 4;wss <= 20;wss++)
{
// Output statistics
printf("%d LRU: %d\n", wss, LRUResults[wss]/1000);
printf("%d FIFO: %d\n", wss, FIFOResults[wss]/1000);
printf("%d Clock: %d\n\n", wss, ClockResults[wss]/1000);
}

return 0;
}

/************************************
*************************************/
int randNum(int mu, int sigma)
{
double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;

if (call == 1)
    {
      call = !call;
return (int) (mu + sigma * (double) X2);
    }

do
{
U1 = -1 + ((double) rand () / RAND_MAX) * 2;
      U2 = -1 + ((double) rand () / RAND_MAX) * 2;
      W = pow(U1, 2) + pow(U2, 2);
    }
while (W >= 1 || W == 0);

mult = sqrt((-2 * log(W)) / W);
X1 = U1 * mult;
X2 = U2 * mult;

call = !call;

return (int) (mu + sigma * (double) X1);
}



int LRU(int wss, int data[]){
int lru[wss], temp[wss];
int pointer = 0, pageFault = 0;
bool found = false;
int indexFound = 0;

for(int h = 0; h < wss; h++){		//initialize working set
	lru[h] = -1;
}

for(int j = 0; j < 1000; j++){		//for every data element
	if(lru[pointer] == -1){			//initialize: if it's an un-used frame
		lru[pointer] = data[j];		//fill it and update the pointer
		pointer = (pointer + 1) % wss;
	}
	else{
		for(pointer = 0; pointer < wss; pointer++){	//search the working set
			if(lru[pointer] == data[j]){		//if you find a matching page
				found = true;			//flag that it's found and record index
				indexFound = pointer;
			}
		}
	
		if(found){
			for(int e = 0; e < indexFound; e++)
				temp[e] = lru[e];		//copy all previous LRU pages

			temp[wss-1] = lru[indexFound];		//move found page to end or array (most recent spot)

			for(int p = indexFound; p < wss - 1; p++)
				temp[p] = lru[p+1];		//copy everything forward a.k.a incrementing when it was last used


			for(int y = 0; y < wss; y++)		
				lru[y] = temp[y];		//copy it back
		}
		else{
			for(int t = 0; t < wss-1; t++)	//copy everything forward losing lru[0]
				temp[t] = lru[t+1];

			temp[wss-1] = data[j];		//replace end (most recent spot) with new page
			for(int q = 0; q < wss; q++)	//copy array back over
				lru[q] = temp[q];
			pageFault++;			//add pageFault
		}

		found = false;				//reset found flag
	}

}



return pageFault;
}



int FIFO(int wss, int data[]){
int fifoArray[wss];
int pointer = 0, pageFault = 0;

for(int y = 0; y < wss; y++){		//initialize working set
	fifoArray[y] = -1;
}

for(int j = 0; j < 1000; j++)		//for every data element
{
	if(fifoArray[pointer] == -1){		//if it's an un-used frame
		fifoArray[pointer] = data[j];	//populate it
		pointer = (pointer + 1) % wss;	//update pointer (will circle around to start)
	}
//	else if(fifoArray[pointer] == data[j]){			//if the page is mathching
								//DO NOTHING was shown on the slides pointer
//	}
	else if(fifoArray[pointer] != data[j]){							//if it's not matching
		fifoArray[pointer] = data[j];			//replace with new page
		pointer = (pointer + 1) % wss;			//update pointer to round robin replace
		pageFault++;					//increment page fault
	}
}

return pageFault;
}




int Clock(int wss, int data[]){
int clock[wss], useBit[wss], pointer = 0;
int pageFault = 0;

for(int u = 0; u < wss; u++){ 				//initialize arrays
	clock[u] = -1;
	useBit[u] = 0;
}

for(int j = 0; j < 1000; j++){				//for every element in data
    for(int c = 0; c < wss; c++){			//clock around the whole working set

	if(clock[pointer] ==  -1){			//if it's empty (ony used at start)
		clock[pointer] = data[j];		//populate
		useBit[pointer] = 1;			//set use bit
		pointer = (pointer + 1) % wss;		//increment pointer (it will circle around)
		break;					//break out of clock loop
	}
	else if(clock[pointer] == data[j]){		//if page matches
		useBit[pointer] = 1;			//set use bit to 1 regardless of what it was before
		break;					//dont move pointer, break clock loop
	}
	else if(useBit[pointer] == 1){			//if the pages dont match, but use bit is 1 
		useBit[pointer] = 0;			//set use bit to 0
		pointer = (pointer + 1) % wss;		//increment pointer and clock loop will continue from here
	}
	else{						//else you found usebit = 0 and pages dont match
		clock[pointer] = data[j];		//replace page
		useBit[pointer] = 1;			//update use bit
		pointer = (pointer + 1) % wss;		//increment pointer
		pageFault++;				//increment the pagefaults
		break;					//break out of clock loop
	}
    }

}

return pageFault;
}
