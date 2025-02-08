#include <iostream>
#include <fstream>
#include <string>
#include "random.h"
#include <cmath>
#include <vector>
#include <fstream>
#include <cstdlib>

using namespace std;

double error (double, double, int); //funzione che calcola l'errore

int main (int argc, char *argv[]){

   //Generatore di numeri casuali
   Random rnd; //oggetto di tipo Random
   int seed[4];
   /*int p1, p2;*/
   int p1=0, p2=0;
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
	ofstream out, out1, out3;
	out.open("output.txt");
	out1.open("output1.txt");
    out3.open("chi2.txt");
	if(out.fail()){
		cerr << "PROBLEM: Unable to open output.txt" << endl;
		exit(1);
	}
	if(out1.fail()){
		cerr << "PROBLEM: Unable to open output1.txt" << endl;
		exit(2);
	}
    if(out3.fail()){
        cerr << "PROBLEM: Unable to open chi2.txt" << endl;
        exit(3);
    }


	int M = 100000; //numeri casuali
	int N = 100; //numero blocchi
	int L = M/N; //numeri casuali per blocco
    double r; //numero casuale
    
    
	vector <double> ave (N); //vettore media
	vector <double> ave2 (N); //vettore media al quadrato
	//vector <double> err (N);
	vector <double> sum_prog(N); //vettore somme progressive media
	vector <double> sum2_prog(N); // vettore somme progressive media al quadrato
	vector <double> err_prog(N); //vettore errore della media per blocco
	vector <double> ave_var (N); //vettore media varianza
	vector <double> ave2_var (N);//vettore media varianza al quadrato
	vector <double> sum_prog_var (N);//vettore somme progressive varianza
	vector <double> sum2_prog_var (N);//vettore somme progressive varianza al quadrato
	vector <double> err_prog_var (N);//vettore errore della varianza per blocco


	for (int i = 0; i < N; i++) { //ciclo sui blocchi
		double sum = 0;
		double sum_var = 0;
		for (int j = 0; j < L; j++) { //ciclo sui lanci per blocco
			r = rnd.Rannyu();
			sum_var += pow((r-0.5), 2);
			sum += r;
		}
		ave[i] = sum/(double)L;
		ave2[i] = pow(ave[i], 2);
		ave_var[i] = sum_var/(double)L;
		ave2_var[i] = pow(ave_var[i], 2);
	}

	for(int i = 0; i < N; i++){ //ciclo sui blocchi
		for(int j = 0; j < i+1; j++){ //ciclo per somme progressive
			sum_prog[i] += ave[j];
			sum2_prog[i] += ave2[j];
			sum_prog_var[i] += ave_var[j];
			sum2_prog_var[i] += ave2_var[j];
		}
		sum_prog[i] = sum_prog[i]/(double)(i+1);
		sum2_prog[i] = sum2_prog[i]/(double)(i+1);
		err_prog[i] = error(sum_prog[i], sum2_prog[i], i); //errore medie progressive
		sum_prog_var[i] = sum_prog_var[i]/(double)(i+1);
		sum2_prog_var[i] = sum2_prog_var[i]/(double)(i+1);
		err_prog_var[i] = error(sum_prog_var[i], sum2_prog_var[i], i); //errore varianze progressive
		//cout << "Media " << sum_prog[i] << " errore " << err_prog[i] << endl;
		out << sum_prog[i] << " " << err_prog[i] << endl; //stampo su file
		out1 << sum_prog_var[i] << " " << err_prog_var[i] << endl; //stampo su file
	}
    
    //chi 2
    int M1 = 100;
    int N1 = 10000; //numeri casuali totali
    double L1 = 100.;
    double sum3;
    vector<double> oss (M1);
    
    for(int k=0; k<100; k++){ //calcolo di 100 chi2
        
        for (int i=0; i<N1; i++){ //uso N1 numeri casuali da inserire in uno dei 100 intervalli di [0,1)
            r = rnd.Rannyu();
            for(int j=0; j<M1; j++){
                if (r >= (double)j/L1 && r < (double)(j+1)/L1) //intervallo in cui cade r
                    oss[j] += 1;
            }
        }
        sum3 = 0.;
        
        for(int j=0; j<M1; j++){
            sum3 += pow(oss[j]-(double)N1/(double)M1,2); //calcolo chi2
            oss[j] = 0;
        }
        
        out3 << sum3/(double)(N1/M1) << endl; //stampo su file i risultati di chi2
    }
    
    
    out.close();
	out1.close();
    out3.close();
	
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
