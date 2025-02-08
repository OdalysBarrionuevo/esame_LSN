#include "random.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

double error(double, double, int); //calcola l'errore
double dist(vector<double>, vector<double>, int ); //calcola la distanza tra due vettori

int main(int argc, char *argv[]) {

	Random rnd;
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
    
    ofstream out, out1;
    out.open("RW_discreto.txt");
    out1.open("RW_continuo.txt");

  	//RANDOM WALK DISCRETO NELLO SPAZIO 3D
    
    int RW = 10000;//numero totale di RW
    int blocks = 100;
    int L = RW/blocks; //numero RW per blocco
    int steps = 100; //numero passi per RW
    int dim = 3; //dimensione spazio
    double a = 1;//valore passo
    
    vector<double> pos (dim, 0.); //vettore posizione
    vector<double> o (dim, 0.); //vettore origine
    vector<double> sum_dist (steps, 0.);//ogni componente rappresenta la somma delle distanze dall'origine per ogni passo
    vector<double> sum_blocks (steps); //somma dei valori medi delle distanze dall'origine dei vari blocchi per ogni passo
    vector<double> sum2_blocks (steps); //somma valori medi quadri delle distanze dall'origine dei vari blocchi per ogni passo
    
    for (int i = 0; i < blocks; i++){ //ciclo sui blocchi
        for (int j = 0; j < L; j++){ //100 RW per blocco
            for (int k = 1; k < steps; k++){ //ogni RW fa 100 passi
                double r = rnd.Rannyu(0.,6.);
                if (r < 1.)
                    pos[0] += a;
                if (r >= 1. and r < 2.)
                    pos[0] -= a;
                if ( r >= 2. and r < 3.)
                    pos[1] += a;
                if ( r >= 3. and r < 4.)
                    pos[1] -= a;
                if ( r >= 4. and r < 5.)
                    pos[2] += a;
                if ( r >= 5. and r < 6.)
                    pos[2] -= a;
                sum_dist[k] += pow(dist(o, pos, dim), 2.); //accumulo le distanze al quadrato dall'o del passo k-esimo per tutti i 100 RW
            }
            for (int l = 0; l < dim; l++)
                pos[l] = 0.;
        }
        for (int m = 0; m < L; m++){ //ciclo sul numero di RW per blocco
            sum_blocks[m] += sqrt(sum_dist[m]/(double)L);
            sum2_blocks[m] += sum_dist[m]/(double)L;
            sum_dist[m] = 0.;
        }
        //sum2_blocks[i] = pow(sum_blocks[i]/(double)L, 2.);
    }
    
    for(int n = 0; n < blocks; n++){ //ciclo sul numero di blocchi per stampare su file
        sum_blocks[n] /= (double)blocks;
        sum2_blocks[n] /= (double)blocks;
        out<< sum_blocks[n] << " " << error(sum_blocks[n], sum2_blocks[n], n) <<endl;
    }
    
    //RANDOM WALK CONTINUO NELLO SPAZIO 3D
    
    double theta, phi; //angoli delle coordinate sferiche
   
    //_c sta per continuo
    vector<double> sum_dist_c (steps, 0.);//ogni componente rappresenta la somma delle distanze dall'origine per ogni passo
    vector<double> sum_blocks_c (steps); //somma dei valori medi delle distanze dall'origine dei vari blocchi per ogni passo
    vector<double> sum2_blocks_c (steps); //somma valori medi quadri delle distanze dall'origine dei vari blocchi per ogni passo
    
    for (int i = 0; i < blocks; i++){
        for (int j = 0; j < L; j++){
            for (int k = 1; k < steps; k++){
                theta = rnd.Rannyu(0., M_PI);
                phi = rnd.Rannyu(0., 2.*M_PI);
                pos[0] += a*sin(theta)*cos(phi);//aggiorno posizione x con angoli theta e phi
                pos[1] += a*sin(theta)*sin(phi);//aggiorno posizione y con angoli theta e phi
                pos[2] += a*cos(theta);//aggiorno posizione z con angoli theta e phi
                sum_dist_c[k] += pow(dist(o, pos, dim), 2.);
            }
            for (int l = 0; l < dim; l++)
                pos[l] = 0.;
        }
        for (int m = 0; m < L; m++){ //ciclo sul numero di RW per blocco
            sum_blocks_c[m] += sqrt(sum_dist_c[m]/(double)L);
            sum2_blocks_c[m] += sum_dist_c[m]/(double)L;
            sum_dist_c[m] = 0.;
        }
    }
    
    for(int n = 0; n < blocks; n++){ //ciclo sul numero di blocchi per stampare su file
        sum_blocks_c[n] /= (double)blocks;
        sum2_blocks_c[n] /= (double)blocks;
        out1<< sum_blocks_c[n] << " " << error(sum_blocks_c[n], sum2_blocks_c[n], n) <<endl;
    }
    
    
    out.close();
    out1.close();
    rnd.SaveSeed();
    
    
  	return 0;
}

// funzione che calcola l'errore
double error(double av, double av2, int n) {
  if (n == 0) {
    return 0;
  } else {
    return sqrt(abs(av2 - pow(av, 2)) / n);
  }
}

//funzione che calcola la distanza tra due vettori
double dist(vector<double> A, vector<double> B, int n){
    if (n==0)
        return 0;
    else{
        double sum=0;
        for(int i = 0; i < n; i++)
            sum += pow(A[i]-B[i], 2);
    return sqrt(sum);
    }
}
