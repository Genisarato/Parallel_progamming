/*
@Author Jan Torres and Genis Aragones
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>



#define N 100000000
#define INI 1500
#define MAX_THREADS 256

int p[N/10];

int indexPrim;
int pp;

pthread_mutex_t escriurePrim = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sumarPrim = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
    int principi;
    int final;
}rang;

void *buscar_primer(void *arg){
    rang *rang_local= (rang *)arg;
    clock_t start_thread, end_thread;
    int local_div;
    int principi = rang_local->principi;
    int final = rang_local->final;
    int primers_locals = 0;

    /*
    Si la part es parell li sumem 1 ja que parell no serà mai prim
    i així utilizem el bucle donat que va sumant 2 en 2 als imparells
    */
    if((principi % 2) == 0) principi++;
    /*Bucle que només sincronitza si hem d'escriure*/
    for (int i = principi; i <= final; i += 2) {  
        local_div = 0;
    
        for (int j = 1; p[j]*p[j] <= i && !local_div; j++) {  
            local_div = local_div || !(i % p[j]);
        }

        if (!local_div) {
            pthread_mutex_lock(&escriurePrim);
            p[pp++] = i;  
            pthread_mutex_unlock(&escriurePrim);
            primers_locals++;
        }
    }
    printf("El thread tarda: %f\n ", dif_thread);

    free(rang_local);  
    pthread_exit(NULL);  

}




int main(int na,char* arg[])
{
    
    pthread_t threads[MAX_THREADS];
    int i;
    int nn,num, num_threads;
    clock_t start, end, start_constant, end_constant;

    pthread_mutex_init(&escriurePrim, NULL);
    pthread_mutex_init(&sumarPrim, NULL);

    assert(na==3);	
    nn = atoi(arg[1]);
    num_threads = atoi(arg[2]);
    int args[2];

    printf("Tots els primers fins a %d\n",nn);

    p[0] = 2;
    p[1] = 3;
    pp = 2;
    num = 5;

    start_constant = clock();
    while (pp < INI)
    {
    for (i=1; p[i]*p[i] <= num ;i++)
        if (num % p[i] == 0) break;
    if (p[i]*p[i] > num) p[pp++]=num;
    num += 2;
    }
    end_constant = clock();
    indexPrim = pp;

    // Dividim el rang en parts
    int rang_total = nn - num;      //Al passar-li número par sempre serà impar
    int mida_per_part = rang_total / num_threads;   //Sempre donara impar
    int resta = rang_total % num_threads; // Mirem quantes parts necitaran un número extra per a poder executar-se correctament ja que les divisions no tenen perque ser exactes


    int principi = num;
    int final;
    for(int i = 0; i < num_threads; i++){
        if(i < num_threads/2) final = principi + mida_per_part * 1.15 + 1 ; /*Li sumem un 15% més de feina a les n_threads/2 primers threads i arrodonim per a no pedre números*/
        else final = principi + mida_per_part * 0.85 - 1; /*Li restem un 15% més de feina a les n_threads/2 primers threads i arrodonim per a no pedre números*/
        if (i == num_threads-1) final = nn;
        if(i < resta) final++;  /*Li sumem 1 per a no pedre numero al dividi en parts*/
        rang *rang_thread= malloc(sizeof(rang));
        rang_thread->principi=principi;
        rang_thread->final=final;
        pthread_create(&threads[i], NULL, buscar_primer, (void *)rang_thread);
        principi = final + 1;
    }

    start = clock();
    for(int i = 0; i < num_threads; i++){
         pthread_join(threads[i], NULL);
    }
    end = clock();

    double dif_part_principal = (double)(end-start)/CLOCKS_PER_SEC;
    double dif_constant = (double)(end_constant-start_constant)/CLOCKS_PER_SEC;
    printf("El temps que ha tardat en executar-se es: %f \n", dif_part_principal);
    exit(0);
}