#ifndef __functions__
#define __functions__

#include "functions.h"

//calcolo dell'errore
double error(double av, double av2, int n) {
  if (n == 0) {
    return 0;
  } else {
    return sqrt(abs(av2 - pow(av, 2)) / n);
  }
}

//probabilità data dalla funzione d'onda al quadrato
double prob(double x, double mu, double sigma) {
    return pow(exp(-pow(x-mu,2)/(2*pow(sigma,2)))+exp(-pow(x+mu,2)/(2*pow(sigma,2))),2);
}

//calcolo energia
double H_energy(double x, double mu, double sigma) {
    double V_potential = pow(x, 4)-(5./2.)*pow(x, 2); //potenziale
    //funzione d'onda
    double wave_fun = exp(-pow(x-mu,2)/(2*pow(sigma,2))) + exp(-pow(x+mu,2)/(2*pow(sigma,2)));
    //derivata seconda della funzione d'onda
    double d2_wave_fun = exp(-pow(x-mu,2)/(2*pow(sigma,2)))*(pow(-(x-mu)/pow(sigma,2),2)-pow(sigma,-2))+exp(-pow(x+mu,2)/(2*pow(sigma,2)))*(pow(-(x+mu)/pow(sigma,2),2)-pow(sigma,-2));
    
    return -0.5*(d2_wave_fun/wave_fun) + V_potential;
}

//implementazione del calcolo dell'energia e dell'errore ad un passo successivo. sfrutta metropolis
vector <double> Metropolis (Random& rnd, double xn, double xn_1, double delta, double mu, double sigma, int N, int L) {
    
    vector <double> metro;
    vector <double> ave(N, 0.0);
    vector <double> ave_2(N, 0.0);
    vector <double> sum_prog(N, 0.0);
    vector <double> sum_prog_2(N, 0.0);
    vector <double> err(N, 0.0);
    
    for (int i = 0; i < N; i++) {
        double sum = 0.0;
        for (int j = 0; j < L; j++){
            xn_1 = rnd.Rannyu(xn-delta, xn+delta);
            double p = prob(xn_1, mu, sigma)/prob(xn, mu, sigma);
            if(rnd.Rannyu() <= p){
                xn = xn_1;
                sum += H_energy(xn, mu, sigma);
            }else{
                sum += H_energy(xn, mu, sigma);
            }
        }
        ave[i] = sum/(double)L;
        ave_2[i] = pow(ave[i], 2);
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < i+1; j++){
            sum_prog[i] += ave[j];
            sum_prog_2[i] += ave_2[j];
        }
        sum_prog[i] = sum_prog[i]/(double)(i+1);
        sum_prog_2[i] = sum_prog_2[i]/(double)(i+1);
        err[i] = error(sum_prog[i], sum_prog_2[i], i);
        
    }
    metro.push_back(sum_prog[N-1]);
    metro.push_back(err[N-1]);
    return metro;
}


#endif // __functions__
