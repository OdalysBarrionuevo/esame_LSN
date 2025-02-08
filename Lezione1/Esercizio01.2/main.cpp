#include <iostream>
#include <fstream>
#include <string>
#include "random.h"
#include <cmath>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <numeric>

using namespace std;

int main (int argc, char *argv[]){
    //generatore di numeri casuali
    Random rnd;//oggetto di tipo Random
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


	//Apro file di output
    ofstream out1, out2, out3, out4;
	out1.open("somme1.txt");
	out2.open("somme2.txt");
	out3.open("somme10.txt");
	out4.open("somme100.txt");
	if(out1.fail()){
		cerr << "PROBLEM: Unable to open somme1.txt" << endl;
		exit(1);
	}
	if(out2.fail()){
		cerr << "PROBLEM: Unable to open somme2.txt" << endl;
		exit(2);
	}
	if(out3.fail()){
		cerr << "PROBLEM: Unable to open somme10.txt" << endl;
		exit(3);
	}
	if(out4.fail()){
		cerr << "PROBLEM: Unable to open somme100.txt" << endl;
		exit(4);
	}

	//numero delle somme
	int M = 10000;

	//Vettori per le somme della distribuzione standard
	vector <double> sum1(M);
	vector <double> sum2(M);
	vector <double> sum10(M);
	vector <double> sum100(M);
	//Vettori per le somme della distribuzione esponenziale
	vector <double> sum1_exp(M);
	vector <double> sum2_exp(M);
	vector <double> sum10_exp(M);
	vector <double> sum100_exp(M);
	//Vettori per le somme della distribuzione lorentziana
	vector <double> sum1_lor(M);
	vector <double> sum2_lor(M);
	vector <double> sum10_lor(M);
	vector <double> sum100_lor(M);

	//10000 "somme" per N=1 e stampo su file somme1.txt
	for (int i = 0; i < M; i++){
		sum1[i] = rnd.Rannyu();
		sum1_exp[i] = rnd.Exp(1);
		sum1_lor[i] = rnd.Cauchy_Lorentz(0, 1);
		out1 << sum1[i] <<" "<< sum1_exp[i] <<" "<< sum1_lor[i] << endl;
	}
	//10000 somme per N=2 e stampo su file somme2.txt
	for (int i = 0; i < M; i++){
		for(int j = 0; j < 2; j++){
			sum2[i] += rnd.Rannyu();
			sum2_exp[i] += rnd.Exp(1);
			sum2_lor[i] += rnd.Cauchy_Lorentz(0, 1);
		}
		sum2[i] = sum2[i]/(double)2;
		sum2_exp[i] = sum2_exp[i]/(double)2;
		sum2_lor[i] = sum2_lor[i]/(double)2;
		out2 << sum2[i] <<" "<< sum2_exp[i] <<" "<< sum2_lor[i] << endl;
	}
	//10000 somme per N=10 e stampo su file somme10.txt
	for (int i = 0; i < M; i++){
		for(int j = 0; j < 10; j++){
			sum10[i] += rnd.Rannyu();
			sum10_exp[i] += rnd.Exp(1);
			sum10_lor[i] += rnd.Cauchy_Lorentz(0, 1);
		}
		sum10[i] = sum10[i]/(double)10;
		sum10_exp[i] = sum10_exp[i]/(double)10;
		sum10_lor[i] = sum10_lor[i]/(double)10;
		out3 << sum10[i] <<" "<< sum10_exp[i] <<" "<< sum10_lor[i] << endl;
	}
	//10000 somme per N=100 e stampo su file somme100.txt
	for (int i = 0; i < M; i++){
		for(int j = 0; j < 100; j++){
			sum100[i] += rnd.Rannyu();
			sum100_exp[i] += rnd.Exp(1);
			sum100_lor[i] += rnd.Cauchy_Lorentz(0, 1);
		}
		sum100[i] = sum100[i]/(double)100;
		sum100_exp[i] = sum100_exp[i]/(double)100;
		sum100_lor[i] = sum100_lor[i]/(double)100;
		out4 << sum100[i] <<" "<< sum100_exp[i] <<" "<< sum100_lor[i] << endl;
	}

	out1.close();
	out2.close();
	out3.close();
	out4.close();
	
	return 0;
}
