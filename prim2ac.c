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

pthread_mutex_t escriurePrim;
pthread_mutex_t sumarPrim;

typedef struct{
    int principi;
    int final;
}rang;

void *buscar_primer(void *arg){
    rang *rang_local= (rang *)arg;
    int principi = rang_local->principi;
    int final = rang_local->final;
    int primers_locals = 0;



    for(int i = principi; i <= final; i++){
        int local_div = 0;
        for(int j = 1; p[j]*p[j] <= i && !local_div; j++) local_div=local_div || (i % p[j]);
        if(!local_div){
            primers_locals++;
            pthread_mutex_lock(&escriurePrim);
            p[indexPrim++] = i;
            pthread_mutex_unlock(&escriurePrim);
        }
    }

    pthread_mutex_lock(&sumarPrim);
    pp += primers_locals;
    pthread_mutex_unlock(&sumarPrim);

}




int main(int na,char* arg[])
{
    
    pthread_t threads[MAX_THREADS];
    int i;
    int nn,num, num_threads;
    clock_t start, end, start_constant, end_constant;

    pthread_mutex_init(&escriurePrim, NULL);
    pthread_mutex_init(&sumarPrim, NULL);

    assert(na==3);	// nombre d'arguments
    nn = atoi(arg[1]);
    num_threads = atoi(arg[2]);
    int args[2];

    printf("Tots els primers fins a %d\n",nn);

    p[0] = 2;
    p[1] = 3;
    pp = 2;
    num = 5;

    start_constant = clock();
    while (pp < INI)  //no paralelitzar
    {
    for (i=1; p[i]*p[i] <= num ;i++)
        if (num % p[i] == 0) break;
    if (p[i]*p[i] > num) p[pp++]=num;
    num += 2;
    }
    end_constant = clock();
    indexPrim += pp;


    /*
    Crear el semafor i els threads, necitem 2 semafors i dividi l'array en parts 
    al no tenir ordenada suposo l'array final de primers haurem de tenir un max suposo

    */
    // Dividim el rang en parts
    int rang_total = nn - INI + 1;
    int mida_per_part = rang_total / num_threads;
    int resta = rang_total % num_threads; // Mirem quantes parts necitaran un número extra per a poder executar-se correctament ja que les divisions no tenen perque ser exactes

    int principi = INI;
    int final;

    for(int i = 0; i < num_threads; i++){
        final = principi + mida_per_part - 1;
        if(i < resta) final++;
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
    /*
    Fer el join i calcular aqui el clock
    */
    //clock
    /*start = clock();
    for(;num<nn;num+=2) 
    {
        int div = 0; // No divisible
        for (i=1; p[i]*p[i] <= num && !div;i++)
            div = div || !(num % p[i]);
        if (!div) p[pp++]=num;
    }
    end = clock();*/
    //clock -> diferenica es lo temps que tarda en fer-se

    double dif_part_principal = (double)(end-start)/CLOCKS_PER_SEC;
    double dif_constant = (double)(end_constant-start_constant)/CLOCKS_PER_SEC;
    printf("El temps que ha tardat en executar-se es: %f \n", dif_part_principal);
    printf("El temps de la part constant es: %f \n", dif_constant);
    printf("Hi ha %d primers\n",pp-1);
    printf("Darrer primer trobat %d\n",p[pp-1]);
    //for(i=0;i<pp;i++) printf("%d\n",p[i]);
    exit(0);
}

