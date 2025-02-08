#include "random.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

double error(double, double, int);//funzione che calcola l'errore

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

  	int M = 10000;//numero totale di lanci
  	int N = 100; //numero di blocchi
  	int L = (M / N); //numero di lanci per blocco

  	double sum1;
	double sum2;
  	vector <double> I1(N);//vettore media
  	vector <double> I2(N);//vettore media al quadrato
	vector <double> I_IS1(N);//vettore media IS
	vector <double> I_IS2(N);//vettore media al quadrato IS
  	vector <double> I1_prog(N);//vettore somme progressive media
	vector <double> I2_prog(N);//vettore somme progressive media al quadrato
	vector <double> I_IS1_prog(N);//vettore somme progressive media IS
	vector <double> I_IS2_prog(N);//vettore somme progressive media al quadrato IS

	ofstream out, out1; //file output
	out.open("Int_media.txt"); //media = metodo MC della media
	out1.open("Int_IS.txt"); //IS = importance sampling
	

  	for (int i = 0; i < N; i++) {//ciclo cui blocchi
    	sum1 = 0;
		sum2 = 0;
    	for (int j = 0; j < L; j++) {//ciclo sui lanci per blocco
      		double r = rnd.Rannyu();
			double x = 1 - sqrt(1 - r); //x distribuita come 2(1-x)
      		sum1 += M_PI / 2 * cos(M_PI * r / 2); //calcolo integranda per metodo della media
			sum2 += M_PI / 2 * cos(M_PI * x / 2)/(2*(1-x)); //importance sampling
    	}
    	I1[i] = sum1/L; //Valore dell'integrale nell'intervallo [0,1] per ogni blocco
    	I2[i] = pow(I1[i], 2);
		I_IS1[i] = sum2/L; //Valore dell'integrale (importance sampling) nell'intervallo [0,1] per ogni blocco
		I_IS2[i] = pow(I_IS1[i], 2);
	
  	}

	//sfrutto il teorema del limite centrale per calcolare l'integrale e il suo errore
  	for (int i = 0; i < N; i++) { //ciclo sui blocchi
    	for (int j = 0; j < i + 1; j++) {//ciclo per le somme progressive
      		I1_prog[i] += I1[j];
      		I2_prog[i] += I2[j];
			I_IS1_prog[i] += I_IS1[j];
			I_IS2_prog[i] += I_IS2[j];
    	}
    	I1_prog[i] /= (i + 1);
    	I2_prog[i] /= (i + 1);
		I_IS1_prog[i] /= (i + 1);
		I_IS2_prog[i] /= (i + 1);
		out << I1_prog[i] << " " << error(I1_prog[i], I2_prog[i], i) << endl; //stampo su file
		out1 << I_IS1_prog[i] << " " << error(I_IS1_prog[i], I_IS2_prog[i], i) << endl;//stampo su file
		
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
