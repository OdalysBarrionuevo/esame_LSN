#include "cities.h"

using namespace std;

Cities::Cities (int size, Random& rnd) {
   
    size_cities = size;
    cities.reserve(size);
    Generate(rnd);
};

Cities::Cities (int size) {
   
    size_cities = size;
    cities.reserve(size);
};

//Metodo che genera numeri casuali sulla circonferenza di raggio 1
/*void Cities::Generate (Random& rnd) {
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
};*/

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

void Cities::Generate(Random& rnd){
    string filename = "cap_prov_ita.dat";
    ifstream file(filename);
    if (!file.is_open()) {
            cerr << "Cannot open file: " << filename << endl;
            return;
        }
    double a, b;
    while(file >> a >> b){
        vector <double> v;
        v.push_back(a);
        v.push_back(b);
        
        cities.push_back(v);
    }
};

vector <double> Cities::GetCity (int index) {
    vector <double> a;
    a.push_back(cities[index][0]);
    a.push_back(cities[index][1]);
    return a;
};
vector <vector<double>> Cities::Get_all(){
    return cities;
};

void Cities::SetCities(vector<vector<double>> a){
        cities = a;
};

void Cities::Print(string& filename){
    ofstream out;
    out.open(filename);
    for (int i = 0; i < size_cities; i++) {
        out << cities[i][0] << "\t" << cities[i][1] << endl;
    }
    out.close();
};




/*void generate(Random& rnd, int N, vector<double>& x, vector<double>& y){
	for(int i=0; i<N; i++){
		x[i] = rnd.Rannyu(-1,1);
		for(int j=0; j<i; j++){
			if(x[i] == x[j]) //faccio in modo di avere tutte le x diverse tra di loro
				i--;
		}
	}
	for(int i=0; i<N; i++) {
		if (rnd.Rannyu() > 0.5){
			y[i] = sqrt(1-pow(x[i],2));
		}else{
			y[i] = -sqrt(1-pow(x[i],2));
		}
	}
	
};*/
