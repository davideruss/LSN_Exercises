
#include "Funzioni.h"

using namespace std;

vector<double> error(vector<double>& av, vector<double>& av2, int n){
    
    if(n>0){
        return sqrt((av2- av*av)/static_cast<double>(n));
    } else{
        return vector<double>();
    }

}

void passo1(vector<array<double, 3>> & traiettoria, Random & rnd, int i){
    //passo random walk in un reticolo
    
    int direzione = rnd.randint(0,2);
    int verso = rnd.choice(-1, 1);
    
    traiettoria[i] = traiettoria[i-1];
    traiettoria[i][direzione] += verso;
}


void passo2(vector<array<double, 3>>& traiettoria, Random & rnd, int i){
    //passo random walk nel continuo
    
    double cos_theta = rnd.Rannyu(-1, 1);
    double sin_theta = sqrt(1- pow(cos_theta, 2));
    double phi = rnd.Rannyu(0, 2*pi);

    traiettoria[i][0]= traiettoria[i-1][0]+sin_theta*cos(phi);
    traiettoria[i][1]= traiettoria[i-1][1]+sin_theta*sin(phi);
    traiettoria[i][2]= traiettoria[i-1][2]+cos_theta;
}

void RW1(vector<array<double, 3>>& traiettoria, int n_passi, Random & rnd){
    for(int i= 1; i <n_passi ; i++ ){

        passo1(traiettoria, rnd, i);
    }
}

void RW2(vector<array<double, 3>>& traiettoria, int n_passi, Random & rnd){
    for(int i= 1; i <n_passi; i++ ){

        passo2(traiettoria, rnd, i);
    }
}

void analisi(int n_passi, int N, int M, ofstream &outputfile, Random& rnd, bool a){

    vector <array<double, 3>> traiettoria(n_passi);
    vector <double> averages(n_passi);
    vector <double> averages_squared(n_passi);
    vector <double> sum_prog(n_passi);
    vector <double> sum_prog2(n_passi);
    vector <double> err(n_passi);
    
    if(a==false){
        for(int i =0; i<N; i++){

            fill(averages.begin(), averages.end(), 0. );
            fill(averages_squared.begin(),averages_squared.end(),0. );

            for(int j=0; j<M; j++){
                traiettoria[0] ={};
                RW1(traiettoria, n_passi, rnd );
                averages+= modulo(traiettoria);
            }
        
            averages/=static_cast<double>(M);
            averages_squared= averages*averages;
            sum_prog += averages;
            sum_prog2 += averages_squared;

        }   
    } else{

        for(int i =0; i<N; i++){

            fill(averages.begin(), averages.end(), 0. );
            fill(averages_squared.begin(),averages_squared.end(),0. );

            for(int j=0; j<M; j++){
                traiettoria[0] ={};
                RW2(traiettoria, n_passi, rnd );
                averages+= modulo(traiettoria);
            }
        
            averages/=static_cast<double>(M);
            averages_squared= averages*averages;
            sum_prog += averages;
            sum_prog2 += averages_squared;

        }  

    }
    sum_prog/=static_cast<double>(N);
    sum_prog2/=static_cast<double>(N);
    err = error(sum_prog, sum_prog2, N-1);

    for(int i=0; i<n_passi; i++){

        double val= sqrt(sum_prog[i]);
        outputfile << i << " " << val << " " << 0.5*err[i]/val << endl;

    }

}

vector<double> modulo(const vector<array<double,3>>& vec){

    vector<double> ris(vec.size());
    for(size_t i=0; i<vec.size(); i++){

        ris[i]= pow(vec[i][0],2)+ pow(vec[i][1], 2) + pow(vec[i][2], 2);
        
    }

    return ris;
}

vector<double>& operator +=(vector<double> &a, const vector<double>& b){

    for(size_t i=0; i<a.size(); i++){

        a[i]+=b[i];
    }

    return a;
}

vector<double> operator -(const vector<double> &a, const vector<double>& b){
    vector<double> c(a.size());
    
    for(size_t i=0; i<a.size(); i++){

        c[i]=a[i]-b[i];
    }

    return c;
}

vector<double>& operator /=(vector<double> & a, const double& b){

    for(size_t i=0; i<a.size(); i++){

        a[i]/=b;
    }

    return a;
}

vector<double> operator /(const vector<double> & a, const double& b){
    
    vector<double> c(a.size());

    for(size_t i=0; i<a.size(); i++){

        c[i]=a[i]/b;
    }

    return c;
}

vector<double> operator*(const vector<double> & a ,const vector<double>& b){
    
    vector<double>ris(a.size());
    for(size_t i=0; i< a.size(); i++){

        ris[i]= a[i]*b[i];
    }
    return ris;
}

vector<double> sqrt(const vector<double>& a){
    vector<double> b(a.size());

    for(size_t i=0; i<a.size(); i++){

        b[i] = sqrt(a[i]);
    }

    return b;
}