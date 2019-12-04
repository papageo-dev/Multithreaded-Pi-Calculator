#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define ThreadsNum 4 //Max number of threads

//Threads structure
typedef struct {
	int rank; //Thread's rank/code
    double sumPi; //Shared counter, to save summary of Pi
}thread_data;

thread_data threads[ThreadsNum]; //Array with threads

/*Calculate Pi*/
void *Calculator(void *rank){

    int thread_rank, sign, i;
    double h;

    thread_rank = *(int *)rank; //Initialize thread's rank
    h=0;
    i=thread_rank;

    if (i%2){ //If thread's code/rank is even number
        sign=-1;
    }
    else{ //If thread's code/rank is odd number
        sign=1;
    }

    threads[thread_rank].sumPi = 4*((double)sign / (2 * thread_rank + 1));

    do{
        i+=ThreadsNum; //Increase i at the maximum thread's number

        if  (i%2){ //If thread's code/rank is even number
            sign=-1;
        }
        else{ //If thread's code/rank is odd number
            sign=1;
        }

        /*Partial calculation of Pi and add it in the Array(threads[]), of structure(Thread_data)*/
        h = (double)sign / (2 * i + 1);
		threads[thread_rank].sumPi += 4 * h;
    }
    while (fabs(h) > 1e-6);

	return NULL;
}

/*Main Program*/
int main(){

    int rank;
    double pi;
    double PI25DT = 3.141592653589793238462643; //Initialize PI25DT=3.14...

    rank=0; //Initialize thread's code/rank
    pi=0; //Initialize summary of Pi

    pthread_t thread_ids[ThreadsNum]; //Array with threads' ids

    while (rank<ThreadsNum){
        threads[rank].rank=rank; //Store current thread's code/rank to structure: thread_data
        pthread_create(&(thread_ids[rank]), NULL, Calculator, (void*)&threads[rank].rank); //Create a thread and call calculator()
        rank++; //Increase rank by 1
    }

    rank = 0;
	while(rank<ThreadsNum) {
		pthread_join(thread_ids[rank], NULL); //Waiting for each thread to finish its calculation
		rank++; //Increase rank by 1
	}

	rank = 0;
	while(rank<ThreadsNum) {
        pi+=threads[rank].sumPi; //Add result of each thread's calculation to the Pi's summary(pi)
		rank++; //Increase rank by 1
	}

	/*Print Pi value and its error value in calculation*/
    printf("Pi is approximately %.16f. Error is %.16f\n", pi, fabs(pi - PI25DT));

return 0;
}
