#include "cities.h"

using namespace std;

//Costruttore
Cities::Cities (int size, Random& rnd) {
   
    size_cities = size;
    cities.reserve(size);
    Generate(rnd);
};

//Metodo che genera numeri casuali sulla circonferenza di raggio 1
void Cities::Generate (Random& rnd) {
    for (int i = 0; i < size_cities; i++) {
        double u = rnd.Rannyu();
        double theta = 2*M_PI*u;
        double x = cos(theta);
        double y = sin(theta);
    
        // vector <double> v = {x, y};
        vector <double> v;
        v.push_back(x);
        v.push_back(y);
        
        cities.push_back(v);
    }
};

//Metodo che genera numeri casuali all'interno di un quadrato di lato 2
/*void Cities::Generate (Random& rnd) {
    for (int i = 0; i < size_cities; i++){
        double x = rnd.Rannyu(-1, 1);
        double y = rnd.Rannyu(-1, 1);
        
        vector <double> v;
        v.push_back(x);
        v.push_back(y);
        
        cities.push_back(v);
    }
};*/

//Metodo che restitusce le coordinate della città index
vector <double> Cities::GetCity (int index) {
    vector <double> a;
    a.push_back(cities[index][0]);
    a.push_back(cities[index][1]);
    return a;
};

//L'argomento privato cities deve contenere le città generate casualmente
void Cities::SetCities(vector<vector<double>> a){
        cities = a;
};

//Stampo le città generale su file
void Cities::Print(string& filename){
    ofstream out;
    out.open(filename);
    for (int i = 0; i < size_cities; i++) {
        out << cities[i][0] << "\t" << cities[i][1] << endl;
    }
    out.close();
};
