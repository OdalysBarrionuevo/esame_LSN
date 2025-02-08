#include "individuo.h"

using namespace std;

Individuo::Individuo(int size, Random& rnd){
   
    size_individuo = size;
    individuo.reserve(size);
    Fill(rnd);
};

void Individuo::Fill(Random& rnd){
    for( int i = 1; i <= size_individuo; i++){
        individuo.push_back(i);
    }
    Swap(rnd);
};

void Individuo::Check(){ 
    // Creo un vettore di flag inizializzati a falso
    vector <bool> visited(size_individuo + 1, false);

    // Verifico se ogni numero intero da 1 a size_individuo è presente una sola volta nell'individuo
    for (int index_city : individuo) {
        //cout << visited[index_city] <<"  " << index_city << endl;
        //BOOL E INDEX_CITY OK
        if (index_city < 1){
            cerr << "Invalid Individual!, città <= 0" << endl;
            return;
        }
        if( index_city > size_individuo){
            cerr << "Invalid Individual!, città > 34" << endl;
            return;
        }
        if( visited[index_city] == true){
            cerr << "Invalid Individual!, città di troppo" << endl;
            return;
        }
    
        visited[index_city] = true;
    }

    // Verifico se tutti i numeri da 1 a size_individuo sono presenti
    for (int i = 1; i <= size_individuo; ++i) {
        if (!visited[i]) {
            cerr << "Incomplete Individual!" << endl;
            return;
        }
    }
};

vector<vector<double>> Individuo::GetCities(Cities& C){
    vector<vector<double>> a;
    a.reserve(C.GetSize());
    for (int i = 0; i < size_individuo; i++) {
        int n;
        n = individuo[i];
        a.push_back(C.GetCity(n-1));
    }
    return a;
};

void Individuo::Swap(Random& rnd){
    vector <int>& a = individuo;
    for (int i = 1; i < size_individuo; i++) {
        int randomIndex = int(rnd.Rannyu() * (size_individuo - 1)) + 1;  // Escludo il primo indice, che devo tenere fissato
        int temp = a[i];
        a[i] = a[randomIndex];
        a[randomIndex] = temp;
    }
};

double Individuo::CostFunction(Cities& C){
    double distance = 0.0;

    for (int i = 0; i < size_individuo; i++) {
        int n, m;
        
        if (i != size_individuo - 1) {
            n = individuo[i];
            m = individuo[i+1];
        }
        else {
            n = individuo[i];
            m = individuo[0];
        }
        distance += dist(C.GetCity(n-1), C.GetCity(m-1));
    }
    return distance;
};

void Individuo::PrintIndividuo(){
    for (int i : individuo) {
            cout << i << " ";
    }
    cout << endl;
};

void Individuo::Mutation1(Random& rnd){
    int index1 = int(rnd.Rannyu() * (size_individuo - 1)) + 1;
    int index2 = int(rnd.Rannyu() * (size_individuo - 1)) + 1;
    
    if (index1 != index2) {
        swap(individuo[index1], individuo[index2]);
    }
};

void Individuo::Mutation2(Random& rnd){
    int m = (int)(rnd.Rannyu(2, size_individuo));  // Gli ultimi m elementi dell'individuo cui sono soggetti a shift [2, size_individuo - 1]
    int n = (int)(rnd.Rannyu(1, m));  // Numero di shifts [1, m - 1]
    // Indice di partenza
    int startIndex = size_individuo - m;
    // Eseguo uno shift circolare n volte degli ultimi m elementi dell'individuo
    for (int i = 0; i < n; i++){
        rotate(individuo.begin() + startIndex, individuo.begin() + startIndex + 1, individuo.begin() + size_individuo);
    }
};

void Individuo::Mutation3(Random& rnd){
    int n = (int)(rnd.Rannyu(1, size_individuo-3));  // Punto dell'individuo dove considero il primo intervallo soggetto alla mutazione [1, size_individuo - 4]
    int width = (int)(rnd.Rannyu(2, (double)(size_individuo-n)/2.0));  // Larghezza dell'intervallo
    int m = (int)(rnd.Rannyu(n+width+1, size_individuo-width));  // Punto dell'individuo dove considero il secondo intervallo (anch'esso di lunghezza width)
    
    for (int i = 0; i < width; i++) {
        swap(individuo[n+i], individuo[m+i]);
    }
};

void Individuo::Mutation4(Random& rnd){
    int n = (int)(rnd.Rannyu(1, size_individuo-2));  // Inizio intervallo [1, size_inidividuo - 3]
    int m = (int)(rnd.Rannyu(size_individuo-2, size_individuo));  // Fine intervallo [size_individuo - 2, size_individuo - 1]
    for (int i = 0; i < (m-n)/2; i++) {
        swap(individuo[n+i], individuo[m-i]);
    }
};

