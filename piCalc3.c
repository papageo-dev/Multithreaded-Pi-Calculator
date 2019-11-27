#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include<stdlib.h>
#include<unistd.h>

#define ThreadsNum 4 //MEGISTOS ARITHMOS NHMATWN POU 8A KATASKEBASTOUN

/*DOMH GIA TA THREADS*/
typedef struct {
	int rank; //KATATAKSH/KWDIKOS TWN THREADS
    double sumPi; //KOINH METABLHTH GIA THN APOTHIKEUSH TOU Pi
}thread_data;

thread_data threads[ThreadsNum];

void *Calculator(void *rank){

    int thread_rank, sign, i;
    double h;

    thread_rank = *(int *)rank; //ARXIKOPOIHSH THS KATATAKSHS TWN THREADS
    h=0; //ARXIKOPOIHSH THS TIMHS GIA TON YPOLOGISMO
    i=thread_rank;

    if (i%2){ //AN O KWDIKOS TOU THREAD EINAI ZUGOS ARITHMOS
        sign=-1;
    }
    else{ //AN O KWDIKOS TOU THREAD EINAI MONOS ARITHMOS
        sign=1;
    }

    threads[thread_rank].sumPi = 4*((double)sign / (2 * thread_rank + 1));

    do{
        i+=ThreadsNum; //AYKSHSH TOU i KATA TON MEGISTO ARITHMO TWN THREADS

        if  (i%2){ //AN O KWDIKOS TOU THREAD EINAI ZUGOS ARITHMOS
            sign=-1;
        }
        else{ //AN O KWDIKOS TOU THREAD EINAI MONOS ARITHMOS
            sign=1;
        }

        /*EPIMEROUS YPOLOGISMOS TOU Pi KAI EKXWRHSH STON PINAKA TOU THREAD, STHN DOMH(Thread_data)*/
        h = (double)sign / (2 * i + 1);
		threads[thread_rank].sumPi += 4 * h;
    }
    while (fabs(h) > 1e-6);

	return NULL;
}

int main(){

    int rank;
    double pi;
    double PI25DT = 3.141592653589793238462643; //ARXIKOPOIHSH TOU PI25DT=3.14...

    rank=0;
    pi=0;

    pthread_t thread_ids[ThreadsNum];

    while (rank<ThreadsNum){
        threads[rank].rank=rank; //EKXWRHSH TOU KWDIKOU KATATAKSHS TOU THREAD STHN DOMH(Thread_data)
        pthread_create(&(thread_ids[rank]), NULL, Calculator, (void*)&threads[rank].rank); //DHMIOURGIA THREAD KAI KLHSH THS Calculator()
        rank++; //AUKSHSH TOU KWDIKOU KATATAKSHS TOU THREAD KATA 1
    }

    rank = 0;
	while(rank<ThreadsNum) {
		pthread_join(thread_ids[rank], NULL); //ANAMONH GIA TERMATISMO TWN NHMATWN
		rank++; //AUKSHSH TOU KWDIKOU KATATAKSHS TOU THREAD KATA 1
	}

	rank = 0;
	while(rank<ThreadsNum) {
        pi+=threads[rank].sumPi; //ATHROISH TWN APOTELESMATWN KATHE NTHREAD KAI EKXWRHSH STHN DOMH(Thread_data)
		rank++; //AUKSHSH TOU KWDIKOU KATATAKSHS TOU THREAD KATA 1
	}

	/*EKTYPWSH TOU Pi KAI TOU SFALMATOS STON YPOLOGISMO TOU*/
    printf("Pi is approximately %.16f. Error is %.16f\n", pi, fabs(pi - PI25DT));

return 0;
}
