#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#define ThreadsNum 4 //MEGISTOS ARITHMOS NHMATWN POU 8A KATASKEBASTOUN
#define RectNum 10000000 //MEGISTOS ARITHMOS EPANALHPSEWN

struct thread_data{ //DOMH GIA TA NHMATA
    int id; //ARITHMOS NHMATOS
    double *sumPi; //MOIRAZOMENOS METRHTHS GIA THN APOTHIKEUSH TOU Pi
    pthread_mutex_t *lock; //METABLHTH KLEIDWMATOS
};


void *Calculator(void *pArg){

    int i, my_id;
    double h, my_sum, x;
    struct thread_data *my_data; //METABLHTH/DEIKTHS THS DOMHS TWN NHMATWN(thread_data)

    my_data = (struct thread_data *)pArg;
    my_id = my_data->id;

    my_id=*((int *)pArg); //ARXIKOPOIHSH TOU ARITHMOU TOU NHMATOS
    h=2.0/RectNum; //ARXIKOPOIHSH THS EMBELIAS TWN TIMWN GIA TOUS YPOLOGISMOUS
    my_sum=0.0; //ARXIKOPOIHSH TOU SUNOLOU TWN PRAKSEWN KATHE NHMATOS, GIA TON YPOLOGISMO TOU Pi

    for (i=my_id; i<RectNum; i+=ThreadsNum){
        x=-1+(i+0.5f)*h; //EN MERH YPOLOGISMOS TOU Pi
        my_sum+=sqrt(1.0-x*x)*h; //YPOLOGISMOS TOU APOTELESMATOS GIA KATHE NHMA
    }

    /*KLEIDWMA TOU KRISIMOU SHMEIOU, OTAN TO KATHE THREAD AYKSANEI TON KOINO METRHTH TOU Pi(my_data->SumPi)*/
    pthread_mutex_lock(my_data->lock); //KLEIDWMA my_data->SumPi
    *(my_data->sumPi)+=my_sum; //PROSTHESH TOU YPOLOGISMOU KATHE NHMATOS STON MOIRAZOMENO METRHTH TOU Pi(my_data->SumPi)
    pthread_mutex_unlock(my_data->lock); //KSEKLEIDWMA my_data->SumPi

return 0;
}


int main(int argc, char *argv[])
{
    int i;
    double PI25DT = 3.141592653589793238462643; //ARXIKOPOIHSH TOU PI25DT=3.14...

    double shared_counter; //PROSWRINOS KOINPS METRHTHS
    pthread_mutex_t shared_mutex; //PROSWRINH METABLHTH KLEIDWMATOS
    pthread_t *threads; //NHMATA

    struct thread_data *pArg; //METABLHTH TUPOU thread_data GIA TA NHMATA

    threads = (pthread_t *)malloc(sizeof(pthread_t)*ThreadsNum);
    pArg = (struct thread_data *)malloc(sizeof(struct thread_data)*ThreadsNum);

    shared_counter=0.0; //ARXIKOPOIHSH KOINOU METRHTH

    pthread_mutex_init(&shared_mutex, NULL); //DHMIOURGIA KLEIDWMATOS

    for (i=0; i<ThreadsNum; i++){
        pArg[i].id=i; //APOTHIKEUSH ARITHMOU NHMATOS STHN DOMH thread_data
       	pArg[i].sumPi = &shared_counter; //APOTHIKEUSH APOTELESMATOS(KOINOS METRHTHS) STHN DOMH thread_data
     	pArg[i].lock = &shared_mutex; //APOTHIKEUSH THS METABLHTHS KLEIDWMATOS THN DOMH thread_data
    }

    for (i=0; i<ThreadsNum; i++){
        pthread_create(&threads[i], NULL, Calculator, (void*)&pArg[i]); //DHMIOURGIA TOU NHMATOS KAI KLHSH THS calculator()
    }

    for (i=0; i<ThreadsNum; i++){
        pthread_join(threads[i], NULL); //ANAMONH GIA TON TERMATISMO KATHE THREAD POU YPOLOGIZEI
    }
    *(pArg->sumPi)*=2.0;

    /*EKTYPWSH THS TIMHS TOU Pi KAI TOU SFALAMATOS STON YPOLOGISMO THS TIMHS TOU*/
    printf("Pi is approximately %.16f. Error is %.16f\n", *(pArg->sumPi), fabs(*(pArg->sumPi) - PI25DT));

    pthread_mutex_destroy(&shared_mutex); //KATASTROFH KLEIDWMATOS

return 0;
}


