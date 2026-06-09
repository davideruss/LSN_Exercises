#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "random.h"

using namespace std;

//Uso questa stuct per potermi poi semplificare i conti dopo nel data blocking
struct Option{
    double call;
    double put;
};

//questa funzione fa il data blocking in automatico in modo da non dover ripetere il codice
//E' fatta per fare il calcolo delle put option e delle call option insieme in modo da ridurre il numero di numeri generati
template <typename Func>
void data_blocking(const string& , const string &, int, int , Random& , Func );


double error(double , double, int );

int main(){

    Random rnd;
    int seed[4];
    int p1, p2;
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

    //valori forniti per il calcolo
    double S0 = 100;
    double T= 1;
    double K=100;
    double r=0.1;
    double sigma = 0.25; 

    int N=100; //numero di blocchi
    int M=100; //numero di step per blocco
    double e = exp(-r*T);
    
//--------------------- Call-option e Put-option diretta ---------------------
    
    data_blocking("Call-option_dir.dat", "Put-option_dir.dat", N, M, rnd, [&](Random& rand){
        double Si= S0*exp((r-sigma*sigma/2)*T + sigma*rand.Gauss(0,1)*sqrt(T));
        return Option{
            e*max(0., Si-K),
            e*max(0., K-Si)
        };

    });

//--------------------- Call-option e Put-option discreta ---------------------
    data_blocking("Call-option_dis.dat", "Put-option_dis.dat",  N, M, rnd, [&](Random& rand){
        int n_steps = 100;
        double dt = T/static_cast<double>(n_steps);
        double Si = S0;

        for(int i=0; i <n_steps; i++){
            Si = Si*exp((r - sigma*sigma * 0.5) *dt + sigma * rand.Gauss(0,1)*sqrt(dt));
        }
        return Option{
            e*max(0., Si-K),
            e*max(0., K-Si)
        };
    });

    return 0;
    
}

template <typename Func>
void data_blocking(const string& s1, const string& s2, int N, int M, Random& rnd, Func funzione){
    
    ofstream outputfile_c(s1);
    ofstream outputfile_p(s2);
    //solite variabili per il data blocking
    double sum_prog_c = 0;
    double sum_prog2_c = 0;
    double sum_prog_p = 0;
    double sum_prog2_p = 0;

    double ave_c= 0;
    double ave_p= 0;

    for (int i = 0; i < N; i++) {
        
        for (int j = 0; j < M; j++) {
            Option a = funzione(rnd);
            
            ave_c += a.call;
            ave_p += a.put;
        }
        
        ave_c /= M;
        ave_p /= M;
        sum_prog_c += ave_c;
        sum_prog2_c += std::pow(ave_c, 2);
        sum_prog_p += ave_p;
        sum_prog2_p += std::pow(ave_p, 2);
        
        double average_c = sum_prog_c / (i + 1);
        double squared_average_c = sum_prog2_c / (i + 1);
        double err_c = error(average_c, squared_average_c, i);
        
        outputfile_c << average_c << "\t\t" << err_c << "\n"; 
        ave_c=0;

        double average_p = sum_prog_p / (i + 1);
        double squared_average_p = sum_prog2_p / (i + 1);
        double err_p = error(average_p, squared_average_p, i);
        
        outputfile_p << average_p << "\t\t" << err_p << "\n"; 
        ave_p=0;
    }

    outputfile_c.close();
    outputfile_p.close();
}

double error(double av, double av2, int n){
    
    if(n>0){
        return sqrt((av2- av*av)/static_cast<double>(n));
    } else if(n==0){
        return 0;
    } else{
        cerr << "error in the error function"<< endl;
        return 0 ;
    }

}



