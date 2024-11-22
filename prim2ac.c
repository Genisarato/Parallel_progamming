#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define N 100000000
#define INI 1500

int p[N/10];

int main(int na,char* arg[])
{
int i;
int nn,pp,num;

assert(na==2);	// nombre d'arguments
nn = atoi(arg[1]);

printf("Tots els primers fins a %d\n",nn);

p[0] = 2;
p[1] = 3;
pp = 2;
num = 5;

while (pp < INI)  //no paralelitzar
 {
  for (i=1; p[i]*p[i] <= num ;i++)
    if (num % p[i] == 0) break;
  if (p[i]*p[i] > num) p[pp++]=num;
  num += 2;
 }
//clock
for(;num<nn;num+=2) 
 {
  int div = 0; // No divisible
  for (i=1; p[i]*p[i] <= num && !div;i++)
    div = div || !(num % p[i]);
  if (!div) p[pp++]=num;
 }
//clock -> diferenica es lo temps que tarda en fer-se
printf("Hi ha %d primers\n",pp-1);
printf("Darrer primer trobat %d\n",p[pp-1]);
//for(i=0;i<pp;i++) printf("%d\n",p[i]);
exit(0);
}

