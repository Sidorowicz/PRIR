#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
float f(float x){
    return 4*x-6*x+5;
}
float metodaTrapezow(int n,float xp,float xk){

    float dx,calka;

    dx=(xk-xp)/(float)n;
    calka=0;
    for(int i=1; i<n;i++)
    {
        calka+=f(xp+i*dx);
    }
    calka+=(f(xp)+f(xk))/2;
    calka*=dx;
    return calka;
}
float Leibniz(int n){
    float suma = 0;
    float wynik;
    for (int i=1; i<=n-1; i++)
    {
        wynik = powf(-1, i-1) / (2 * i - 1);
        suma += wynik;
    }
    return 4 * suma;
}
float oblicz(){
int liczba;
	printf("Podaj liczbe procesów?\n");
    scanf("%d", &liczba);
    for(int i=0; i<liczba; i++)
    {
            if(fork()==0)
        {
            srand(time(NULL) ^(getpid()<<14));

            float xp=rand()%30+1,xk=rand()%34+2;
            int n = 100 + rand()%5000;
            float wynik = metodaTrapezow(n,xp,xk);
            printf("n= %i poczatek: %f koniec: %f \n ",n,xp,xk);
            printf("Wynik metoda trapezow:  %f \n ", wynik);
            float pi = Leibniz(n);
            printf("n=: %i ",n);
            printf("PI =  %f \n", pi);
            exit(0);
        }
    }
}
int main()
{
    oblicz();
}
