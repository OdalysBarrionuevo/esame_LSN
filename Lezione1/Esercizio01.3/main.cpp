//ESERCIZIO 3: Buffon’s experiment
#include <iostream>
#include <fstream>
#include <string>
#include "random.h"
#include <cmath>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <numeric>
#include <iomanip>

using namespace std;

double error (double, double, int);

int main (int argc, char *argv[]){

    Random rnd;
       int seed[4];
       int p1, p2;
       /*int p1=0, p2=0;*/
       ifstream Primes("Primes");
       if (Primes.is_open()){
          Primes >> p1 >> p2 ;
       } else cerr << "PROBLEM: Unable to open Primes" << endl;
       Primes.close();

       ifstream input("seed.in");
       string property;
       if (input.is_open()){
          while ( !input.eof() ){
             input >> property;
             if( property == "RANDOMSEED" ){
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed,p1,p2);
             }
          }
          input.close();
       } else cerr << "PROBLEM: Unable to open seed.in" << endl;
    
    //file di output
    ofstream out;
    out.open("output.txt");
    if(out.fail()){
        cerr << "PROBLEM: Unable to open output.txt" << endl;
        exit(1);
    }
    
    //variabili per stima di pi-greco e per blocking method
    double r, theta; //distanza centro dalla linea più vicina e angolo rispetto all'asse orizzontale
    double x, y;//variabili per il calcolo di theta senza usare M_PI della libreria cmath
    double x2y2 = 0.;
    double d = 1.;   //distanza linee
    double L = 0.6;  //lunghezza ago
    int hit = 0;    //conteggio delle volte che l'ago interseca la linea
    int tot = 100000; //lanci totali
    int N = 100; //numero blocchi
    int Z = tot/N; //numero lanci per blocco
    
    //vettori per bocking method
    vector <double> ave (N);
    vector <double> ave2 (N);
    vector <double> sum_prog(N);
    vector <double> sum2_prog(N);
    vector <double> err_prog(N);
    
    for (int i = 0; i < N; i++){ //ciclo sui blocchi
        for (int j = 0; j < Z; j++){ //ciclo sul numero di lanci per blocco
            r = rnd.Rannyu(0.,d/2.);
            do {
                x = rnd.Rannyu(0., 1.);
                y = rnd.Rannyu(0., 1.);
                x2y2 = pow(x, 2.) + pow(y, 2.);
            } while (x2y2 > 1.);
            theta = acos(x/(double)(sqrt(x2y2)));
            if (r <= (L/2.)*sin(theta)){//se la distanza dal centro alla linea più vicina
                                        //è più piccola della proiezione di metà ago
                                        //sull'asse verticale, allore l'ago interseca la linea
                hit++;
            }
        }
        ave[i] = 2.*L*(double)Z/(hit*d); //valore pigreco con Z lanci
        ave2[i] = pow(ave[i], 2); //pigreco quadro dopo Z lanci
        hit = 0;
    }
    
    for(int i = 0; i < N; i++){//ciclo sui blocchi
        for(int j = 0; j < i+1; j++){//ciclo per le somme progressive
            sum_prog[i] += ave[j];
            sum2_prog[i] += ave2[j];
        }
        sum_prog[i] = sum_prog[i]/(double)(i+1); // valore pigreco cumulato
        sum2_prog[i] = sum2_prog[i]/(double)(i+1); // pigreco quadro cumulato
        err_prog[i] = error(sum_prog[i], sum2_prog[i], i); //errore cumulato
        out << sum_prog[i] << " " << err_prog[i] << endl;//stampo su file

    }
    
    out.close();
    rnd.SaveSeed();
   
    return 0;
    
}


//funzione che calcola l'errore
double error (double av, double av2, int n){
    if(n == 0){
        return 0;
    }
    else {
        return sqrt(abs(av2 - pow(av, 2))/n);
    }
}
