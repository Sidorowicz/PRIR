#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "mpi.h"

#define TAG 545563

using namespace std;
void dom_aukcyjny(int ilosc_kupcow)
{
    cout << "Witamy w domu aukcyjnym Wichrowy Strzech!" << endl;
    int produkty = (rand()%ilosc_kupcow) + 1;
    while(produkty > 0 && ilosc_kupcow > 0)
    {
        int czynnosc_kupca;
        MPI_Status status;
        MPI_Recv(&czynnosc_kupca,1,MPI_INT,MPI_ANY_SOURCE,TAG,MPI_COMM_WORLD, &status);

        switch (czynnosc_kupca)
        {
        case 1:
            cout << "kupiec opuszcza posesje" << endl;
            ilosc_kupcow--;
            if(produkty > 1)
                cout << "Pozostalo tylko " << --produkty << " produktów." << endl;
            else
                cout << "Sprzedano wszystko" << endl;
            break;

        case 2:
            cout << "kupiec musi sie jeszcze zastanowic!" << endl;
            cout << "Nic nie zostało sprzedane!" << endl;
            break;

        case 3:
            cout << "kupiec sprzedał produkt" << endl;
            cout << "Na sprzedaż jest  " << ++produkty << " produktów!" << endl;
            break;

        case 4:
            cout << "kupiec spłaca podatek" << endl;
            break;

        case 5:
            cout << "kupiec ogląda towary" << endl;
            break;

        case 6:
            ilosc_kupcow--;
            if(produkty >= 2)
            {
                cout << "kupiec zakupił pakiet produktow" << endl;
                produkty -= 2;
                if(produkty >= 1)
                    cout << "Pozostalo tylko " << produkty << " produktów" << endl;
                else
                    cout << "Magazyn pusty << endl;
            }
            else
            {
                cout << "kupiec chciał pakiet ale został ostatni egzemplarz" << endl;
            }
            break;

        default:
            cout << "dom aukcyjny zamkniety" << endl;
            ilosc_kupcow--;
            break;
        }

        cout << "Na posesji znajduje się " << ilosc_kupcow << " kupcow" << endl;
    }

    if (ilosc_kupcow == 0 && produkty == 0)
        cout << "Wszyscy kupcy zadowoleniu opuscili posesje." << endl;
    else if(ilosc_kupcow <= 0 )
        cout << "Wszyscy kupcy opuścili posesje." << endl;
    else if (produkty == 0)
        cout << "Cały nakład został sprzedany, dom handlowy zostanie zamknięty!" << endl;
}

void kupiec(int nr_kupca)
{
    while (true)
    {
        int czynnosc_kupca = rand()%7;
        switch (czynnosc_kupca)
        {
        case 1:
        {
            cout << "Kupuje kupiec nr. " << nr_kupca << " , gratuluje" << endl;
            return;
        }

        case 2:
        {
            cout << "Kupiec nr. " << nr_kupca << " nie jest pewien zakupu." << endl;
            break;
        }

        case 3:
        {
            cout << "Kupiec nr. " << nr_kupca << " sprzedał przedmiot." << endl;
            cout << "Szuka czegoś dla siebie." << endl;
            break;
        }

        case 4:
        {
            int podatek = 1 + (rand() % 100);
            cout << "Ja kupiec nr " << nr_kupca << " płacę wam " << podatek << "% zaległego podatku od sprzedaży" << endl;
            break;
        }

        case 5:
        {
            cout << "Kupiec nr " << nr_kupca << " przyszedł obejrzeć towary" << endl;
            break;
        }

        case 6:
        {
            cout << "Kupiec nr " << nr_kupca << " chcialby zakupić dwa produkty." << endl;
            return;
        }

        default:
        {
            cout << "Dom handlowy zamknięty!" << endl;
            cout << "Prosimy kupca nr. " << nr_kupca << " o opuszczenie posesji." << endl;
            return;
        }
        }
        MPI_Send(&czynnosc_kupca, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
        int czas_czekania = 1 + (rand() % 3);
        sleep(czas_czekania);
    }
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int nr_procesu;
    MPI_Comm_rank(MPI_COMM_WORLD, &nr_procesu);
    int ilosc_procesow;
    MPI_Comm_size(MPI_COMM_WORLD, &ilosc_procesow);
    srand(time(NULL) ^ 56621 ^ 185213<< nr_procesu);
    if(nr_procesu == 0){
        dom_aukcyjny(ilosc_procesow-1);
    }
    else{
        kupiec(nr_procesu);
    }
    MPI_Finalize();
}
