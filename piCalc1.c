#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#define ThreadsNum 4 //Max number of threads
#define RectNum 10000000 //Max number of repetitions

//Threads structure
struct thread_data{
    int id; //thread id
    double *sumPi; //Shared counter, to save summary of Pi
    pthread_mutex_t *lock; //Lock variable
};


/*Calculate Pi*/
void *Calculator(void *pArg){

    int i, my_id;
    double h, my_sum, x;
    struct thread_data *my_data; //Pointer to threads structure (thread_data)

    my_data = (struct thread_data *)pArg;
    my_id = my_data->id;

    my_id=*((int *)pArg); //Initialize thread id
    h=2.0/RectNum; //Initialize the range of values, for the calculations
    my_sum=0.0; //Initialize the summary of each thread's calculation

    for (i=my_id; i<RectNum; i+=ThreadsNum){
        x=-1+(i+0.5f)*h; //Partial calculation of Pi
        my_sum+=sqrt(1.0-x*x)*h; //Calculate each thread's result and add it to thread's summary
    }

    /*Lock the critical section, when a thread increase the shared counter of Pi's summary(my_data->SumPi)*/
    pthread_mutex_lock(my_data->lock); //Lock: my_data->SumPi
    *(my_data->sumPi)+=my_sum; //Add each thread's result(my_sum) to the shared counter of Pi's summary(my_data->SumPi)
    pthread_mutex_unlock(my_data->lock); //Unlock: my_data->SumPi

return 0;
}


/*Main Program*/
int main(int argc, char *argv[])
{
    int i;
    double PI25DT = 3.141592653589793238462643; //Initialize PI25DT=3.14...

    double shared_counter; //Temporary shared counter
    pthread_mutex_t shared_mutex; //Temporary lock variable
    pthread_t *threads; //Pointer to threads

    struct thread_data *pArg; //Pointer to thread_data structure

    threads = (pthread_t *)malloc(sizeof(pthread_t)*ThreadsNum); //Initialize threads
    pArg = (struct thread_data *)malloc(sizeof(struct thread_data)*ThreadsNum); //Initialize pArg

    shared_counter=0.0; //Initialize share counter, at 0.0

    pthread_mutex_init(&shared_mutex, NULL); //Initialize the Lock variable

    for (i=0; i<ThreadsNum; i++){
        pArg[i].id=i; //Store current thread's id to thread_data structure
       	pArg[i].sumPi = &shared_counter; //Store the result(shared counter) to thread_data structure
     	pArg[i].lock = &shared_mutex; //Store the Lock variable to thread_data structure
    }

    for (i=0; i<ThreadsNum; i++){
        pthread_create(&threads[i], NULL, Calculator, (void*)&pArg[i]); //Create a thread and call calculator()
    }

    for (i=0; i<ThreadsNum; i++){
        pthread_join(threads[i], NULL); //Waiting for each thread to finish its calculation
    }
    *(pArg->sumPi)*=2.0; //Multiply Pi's summary with 2.0

    /*Print Pi value and its error value in calculation*/
    printf("Pi is approximately %.16f. Error is %.16f\n", *(pArg->sumPi), fabs(*(pArg->sumPi) - PI25DT));

    pthread_mutex_destroy(&shared_mutex); //Destroy the Lock variable

return 0;
}


