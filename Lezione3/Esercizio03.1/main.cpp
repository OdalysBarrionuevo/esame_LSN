#include "random.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

double error(double, double, int);

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

    int M = 10000;//lanci totali
    int N = 100;//numero blocchi
    int L = M / N;//numero lanci per blocco
    double T = 1;//data di scadenza
    double So = 100; //prezzo di partenza (al tempo t=0)
    double sigma = 0.25; //volatilità
    double K = 100; //prezzo pattuito
    double r = 0.1; //tasso d'interesse privo di rischi
    vector<double> C(N);
    vector<double> C2(N);
    vector<double> P(N);
    vector<double> P2(N);

    ofstream out, out1;
    out.open("Call.txt");
    out1.open("Put.txt");

	//Un solo step
    for (int i = 0; i < N; i++) {
        double sumC = 0;
        double sumP = 0;
        for (int j = 0; j < L; j++) {
            double z = rnd.Gauss(0, 1);
            double S = So * exp((r - 0.5 * pow(sigma, 2)) * T + sigma * sqrt(T) * z);
            sumC += exp(-r * T) * max(0., (double)(S - K));
            sumP += exp(-r * T) * max(0., (double)(K - S));
        }
        C[i] = sumC / (double)L;
        C2[i] = pow(C[i], 2.);
        P[i] = sumP / (double)L;
        P2[i] = pow(P[i], 2.);
    }

	//A più step
	double Sf = 0, Si = 0;
	vector <double> C_step(N);
	vector <double> C2_step(N);
	vector <double> P_step(N);
	vector <double> P2_step(N);

	for(int i = 0; i < N; i++){
		double sumC_step = 0;
		double sumP_step = 0;
		for (int j = 0; j < L; j++){
            for (int k = 0; k < 100; k++) {
                double z = rnd.Gauss(0, 1);
                if (k == 0) {
                    Sf = So;
                } else {
                    Sf = Si * exp((r - 0.5 * pow(sigma, 2)) * 0.01 + sigma * sqrt(0.01) * z);
                }
                Si = Sf;
            }
			sumC_step += exp(-r * T) * max(0., (double)(Si - K));
			sumP_step += exp(-r * T) * max(0., (double)(K - Si));
        }
		C_step[i] = sumC_step / (double)L;
		C2_step[i] = pow(C_step[i], 2);
		P_step[i] = sumP_step / (double)L;
		P2_step[i] = pow(P_step[i], 2);
	}
	//media ed errore a blocchi
	vector<double> sumC_prog(N);
	vector<double> sumP_prog(N);
	vector<double> sumC2_prog(N);
    vector<double> sumP2_prog(N);
	vector<double> errC_prog(N);
	vector<double> errP_prog(N);
	vector<double> sumC_step_prog(N);
	vector<double> sumP_step_prog(N);
	vector<double> sumC2_step_prog(N);
	vector<double> sumP2_step_prog(N);
	vector<double> errC_step_prog(N);
	vector<double> errP_step_prog(N);

    for (int i = 0; i < N; i++) { //ciclo sui blocchi
        for (int j = 0; j < i + 1; j++) {//ciclo per le somme progressive
            sumC_prog[i] += C[j];
            sumC2_prog[i] += C2[j];
            sumP_prog[i] += P[j];
            sumP2_prog[i] += P2[j];
			sumC_step_prog[i] += C_step[j];
			sumC2_step_prog[i] += C2_step[j];
			sumP_step_prog[i] += P_step[j];
			sumP2_step_prog[i] += P2_step[j];
		}
		sumC_prog[i] /= (double)(i + 1);  // media cumulata Call
		sumC2_prog[i] /= (double)(i + 1); // media cumulata al quadrato Call
		sumC_step_prog[i] /= (double)(i + 1);
		sumC2_step_prog[i] /= (double)(i + 1);
		errC_prog[i] = error(sumC_prog[i], sumC2_prog[i], i);
		errC_step_prog[i] = error(sumC_step_prog[i], sumC2_step_prog[i], i);
		sumP_prog[i] /= (double)(i + 1);  // media cumulata Put
		sumP2_prog[i] /= (double)(i + 1); // media cumulata al quadrato Put
		sumP_step_prog[i] /= (double)(i + 1);
		sumP2_step_prog[i] /= (double)(i + 1);
		errP_prog[i] = error(sumP_prog[i], sumP2_prog[i], i);
		errP_step_prog[i] = error(sumP_step_prog[i], sumP2_step_prog[i], i);

        //stampo su file sia caso ad uno step che a più step per la CALL e la PUT
		out << scientific << setprecision(10) << sumC_prog[i] << " " << scientific << setprecision(10) << errC_prog[i] << " " << scientific << setprecision(10) << sumC_step_prog[i] << " " << scientific << setprecision(10) << errC_step_prog[i] << endl;
		out1 << scientific << setprecision(10) << sumP_prog[i] << " " << scientific << setprecision(10) << errP_prog[i] << " " << scientific << setprecision(10) << sumP_step_prog[i] << " " << scientific << setprecision(10) << errP_step_prog[i] << endl;
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
