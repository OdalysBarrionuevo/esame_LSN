#include "functions.h"

int main(int argc, char *argv[]) {
    
    Random rnd;//oggetto di tipo Random
    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()) {
        Primes >> p1 >> p2;
    } else
        cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();
    
    ifstream input("seed.in");
    string property;
    if (input.is_open()) {
        while (!input.eof()) {
            input >> property;
            if (property == "RANDOMSEED") {
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed, p1, p2);
            }
        }
        input.close();
    } else
        cerr << "PROBLEM: Unable to open seed.in" << endl;
    
    
    
    
    int M=1e6;    //tot passi
    int N=100;    //blocchi
    int L=(M/N);  //passi per blocco
    double xn, xn_1; //passo corrente e passo successivo
    
    vector <double> average(N, 0.0); //vettore media
    vector <double> average_2(N, 0.0);//vettore media al quadrato
    vector <double> sum_prog(N, 0.0);//vettore somme progressive
    vector <double> sum_prog_2(N, 0.0);//vettore somme progressive al quadrato
    
    ofstream out_H, out_x; //file energia e posizioni
    out_H.open("H_energy_0_prova.txt");
    out_x.open("x_0_prova.txt");
    
    xn = 1.0; //passo iniziale
    double delta = 2.4; //passo
    //parametri psi
    double mu = 0.8018046198;
    double sigma = 0.607285428;
    
    out_x << xn << endl;
    
    //vettore che implementa l'algoritmo di metropolis che restituisce un vettore di medie istantanee e vettore delle medie al quadrato
    vector <vector <double>> metro = Metropolis(rnd, out_x, xn, xn_1, delta, mu, sigma, N, L);
    
    //riempio i vettori average e average_2
    for (int i = 0; i < N; i++){ //ciclo sui blocchi
        average[i] = metro[0][i];
        average_2[i] = metro [1][i];
        
    }
    
    for(int i=0; i<N; i++) {//ciclo sui blocchi
        for(int j=0; j<i+1; j++) { //ciclo per le somme progressive
            sum_prog[i] += average[j];
            sum_prog_2[i] += average_2[j];
        }
        sum_prog[i]/=(i+1);
        sum_prog_2[i]/=(i+1);
        out_H << sum_prog[i] << "    " << error(sum_prog, sum_prog_2, i) << endl; //stampo su file l'energia con errore
        }
    
    
    out_x.close();
    out_H.close();
    return 0;
    
}
