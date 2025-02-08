#ifndef __functions__
#define __functions__

#include "functions.h"

//funsione che calcola l'errore
double error(vector <double> av, vector <double> av2, int n) {
  if (n == 0) {
    return 0;
  } else {
    return sqrt(abs(av2[n] - pow(av[n], 2)) / n);
  }
}

//probabilità data dalla funzione d'onda al quadrato dati i parametri x, mu e sigma
double prob(double x, double mu, double sigma) {
    return pow(exp(-pow(x-mu,2)/(2*pow(sigma,2)))+exp(-pow(x+mu,2)/(2*pow(sigma,2))),2);
}

//calcolo dell'energia dato un potenziale
double H_energy(double x, double mu, double sigma) {
    double V_potential = pow(x, 4)-(5./2.)*pow(x, 2); //potenziale
    //funzione d'onda
    double wave_fun = exp(-pow(x-mu,2)/(2*pow(sigma,2))) + exp(-pow(x+mu,2)/(2*pow(sigma,2)));
    //derivata seconda della funzione d'onda
    double d2_wave_fun = exp(-pow(x-mu,2)/(2*pow(sigma,2)))*(pow(-(x-mu)/pow(sigma,2),2)-pow(sigma,-2))+exp(-pow(x+mu,2)/(2*pow(sigma,2)))*(pow(-(x+mu)/pow(sigma,2),2)-pow(sigma,-2));
    
    return -0.5*(d2_wave_fun/wave_fun) + V_potential;
}

//implementazione dell'algoritmo di metropolis che restituisce un vettore di medie istantanee e vettore delle medie al quadrato
vector <vector <double>> Metropolis (Random& rnd, ofstream& out_x, double xn, double xn_1, double delta, double mu, double sigma, int N, int L) {
    
    vector <vector <double>> metro;
    vector <double> ave(N, 0.0);
    vector <double> ave_2(N, 0.0);

    int acc = 0;
    double prob_acc;
    
    cout << "Probabilità di accettazione per ogni blocco" << endl;
    
    for (int i = 0; i < N; i++) {
        double sum = 0.0;
        acc = 0.;
        for (int j = 0; j < L; j++){
            xn_1 = rnd.Rannyu(xn-delta, xn+delta);
            double p = prob(xn_1, mu, sigma)/prob(xn, mu, sigma);
            if(rnd.Rannyu() <= p){
                xn = xn_1;
                acc += 1;
                sum += H_energy(xn, mu, sigma);
                out_x << xn << endl; //stampo le posizioni accettate per istogramma
            }else{
                sum += H_energy(xn, mu, sigma);
            }
        }
        prob_acc = (double)acc/(double)L;
        ave[i] = sum/(double)L;
        ave_2[i] = pow(ave[i], 2);
        cout << i+1 << "    " << prob_acc << endl;
    }
    metro.push_back(ave);
    metro.push_back(ave_2);
    return metro;
}


#endif // __functions__
