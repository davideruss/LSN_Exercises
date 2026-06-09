#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "random.h"

using namespace std;

double error(double , double, int );

// Per non ripetere 2 volte lo stesso codice definisco una funzione che fa il data blocking 
// e prenderà in entrata una lambda function
template <typename Func>
void data_blocking(ofstream&, int, Random& , Func );

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
    
    double N=100; //lo uso sia come numero di blocchi che come numero di throws per blocco in modo da avere in totale 10^4 throws

//---------------- integrali con il primo metodo -------------------
    ofstream outputfile("integrali1.txt");

    data_blocking(outputfile, N, rnd, [](Random& rand){return M_PI/2 *cos(M_PI*rand.Rannyu()/2);});

    outputfile.close();
    
//---------------- integrali con il secondo metodo -------------------

    ofstream outputfile2("integrali2.txt");
    
    data_blocking(outputfile2, N, rnd, [](Random& rand){
        double s= rand.distribuzione1();
        return M_PI/2 *cos(M_PI*s/2)/(2*(1-s));
    });

    outputfile2.close();
    
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

template <typename Func>
void data_blocking(ofstream& outputfile, int N, Random& rnd, Func integrand){
    
    //solite variabili per il data blocking
    double sum_prog = 0;
    double sum_prog2 = 0;
    double ave = 0;

    for (int i = 0; i < N; i++) {
        
        for (int j = 0; j < N; j++) {
            ave+= integrand(rnd);
        }
        
        ave /= N;
        sum_prog += ave;
        sum_prog2 += std::pow(ave, 2);
        
        double average = sum_prog / (i + 1);
        double squared_average = sum_prog2 / (i + 1);
        double err = error(average, squared_average, i);
        
        outputfile << average << "\t\t" << err << "\n"; 
        ave=0;
    }
}