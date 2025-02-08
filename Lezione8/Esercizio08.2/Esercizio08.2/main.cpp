#include "functions.h"

int main(int argc, char *argv[]) {
    
    Random rnd; //oggetto di tipo Random
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
    
    
    
    //per calcolo energia
    int M=1e6;    //tot passi
    int N=100;    //blocchi
    int L=(M/N);  //passi per blocco
    
    double xn, xn_1; //passo corrente e passo successivo
    double delta = 1.4; //passo
    
    vector <double> H_t(N, 0.0);//vettore energia fissata la temperatura
    vector <double> H_t_2(N, 0.0);//vettore energia al quadrato fissata la temperatura
    vector <double> H_err_t(N, 0.0);//vettore errore fissata la temperatura
    vector <double> H_err_t_2(N, 0.0);//vettore errore al quadrato fissata la temperatura
    
    vector <double> H_t_prog (200, 0.0);//vettore energia per ogni temperatura
    vector <double> H_t_2_prog(200, 0.0);//vettore energia al quadrato per ogni temperatura
    vector <double> H_err_prog (200, 0.0);//vettore errore per ogni temperatura
    vector <double> H_err_prog_2 (200, 0.0);//vettore errore al quadrato per ogni temperatura
    
    ofstream out;
    out.open("Results_prova.txt");
    
    xn = 1.0; //passo iniziale
    //parametri psi iniziali
    double mu = 1.0;
    double sigma = 0.5;
    int b; //beta (inverso della temperatura)
    int b_max = 200; //beta massimo
    
    //vettore contenente un primo valore dell'energia e l'errore calcolati sfruttando l'algoritmo di metropolis
    vector <double> H_and_err = Metropolis(rnd, xn, xn_1, delta, mu, sigma, N, L);
    double sum_t;
    double sum_err_t;
    double d_mu;
    double d_sigma;
    double p;
    int acc;
    double prob_acc;
    
    cout << "Probabilità di accettazione per ogni beta e blocco" << endl;
    for(int k = 0; k < b_max; k++) { //ciclo sulla temperatura
        b = k+1; //aggiorno beta
        cout << "beta =  " << b; //stampo beta
        for(int n = 0; n < 5; n++) { //ciclo su 5 blocchi per fare il data blocking per ogni temperatura
            cout << " Blocco  " << n+1 << "  ";
            sum_t = 0;
            sum_err_t = 0;
            acc = 0;
            for(int l = 0; l < 10; l++) {//ciclo su 10 passi per blocco
                d_mu = rnd.Rannyu(-0.3, 0.3);
                d_sigma = rnd.Rannyu(-0.3, 0.3);
                mu += d_mu; //aggiorno mu
                sigma += d_sigma; //aggiorno sigma
                //energia ed errore calcolati man mano con parametri aggiornati (usa metropolis)
                vector <double> H1_and_err = Metropolis(rnd, xn, xn_1, delta, mu, sigma, N, L);
                p = exp(-b*(H1_and_err[0]-H_and_err[0])); //probabilità
                if (rnd.Rannyu() <= p) { //condizioni di accettazione
                    H_and_err = H1_and_err; //aggiorno energia ed errore
                    sum_t += H_and_err[0];
                    sum_err_t += H_and_err[1];
                    acc += 1;
                }else{
                    //torno indietro se l'accettazione non è avvenuta
                    mu -= d_mu;
                    sigma -= d_sigma;
                    sum_t += H_and_err[0];
                    sum_err_t += H_and_err[1];
                }
            }
            prob_acc = (double)acc/10.;
            H_t[n] = sum_t/10.; //energia per ognuno dei 5 blocchi a t fissata
            H_t_2[n] = pow(H_t[n], 2);
            H_err_t[n] = sum_err_t/10.;
            H_err_t_2[n] = pow(H_err_t[n], 2);
            cout << prob_acc << endl;
        }
        for(int i = 0; i < 5; i++){//per ogni beta tengo l'ultimo valore delle somme progressive dell'energia con il rispettivo errore
            H_t_prog[k] += H_t[i]/5.;
            H_t_2_prog[k] += H_t_2[i]/5.;
            H_err_prog[k] += H_err_t[i]/5.;
            H_err_prog_2[k] += H_err_t_2[i]/5.;
            
        }
        //stampo valori dell'energia con errore al variare della temperatura e i valori di mu e sigma
        out << b << "    " << H_t_prog[k] << "    " << fixed << setprecision(10) << sqrt(pow(error(H_err_prog[k], H_err_prog_2[k], 5), 2)+ pow(error(H_t_prog[k], H_t_2_prog[k], 5),2)) << "    " << mu << "    " << sigma << endl;
    }
    
    out.close();
    return 0;
    
}

