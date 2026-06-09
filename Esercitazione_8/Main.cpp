#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include "random.h"

using namespace std;

//Scrivo una struct per essere più comodo nelle funzioni in cui userò Montecarlo
struct variabili{
    int N,M; //valori per i blocchi
    double delta_x, delta_sigma, delta_mu; //variazioni per la proposta di mossa
    double x, sigma, mu; //valori della funzione
    int acceptance, acc_mu, acc_sigma; 
    Random &rnd;
    double H, sigma_H;
    double T;
};

//Funzione per calcolare l'errore
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

//funzioni per il calcolo dell'integrale

double psi_t(variabili & var){
    return exp(-pow((var.x-var.mu)/var.sigma, 2)/2)+ exp(-pow((var.x+var.mu)/var.sigma, 2)/2);
}

double psi_t(double &x, double &mu , double &sigma){
    return exp(-pow((x-mu)/sigma, 2)/2)+ exp(-pow((x+mu)/sigma, 2)/2);
}

double H_psi_t(variabili &var){
    return (var.sigma*var.sigma -(var.x*var.x+ var.mu*var.mu) +2*var.mu*var.x*tanh(var.mu*var.x/(var.sigma*var.sigma)))/(2*pow(var.sigma,4))+ pow(var.x, 4)- var.x*var.x*5./2. ;
}
 
void metro(variabili &var ){

    double x_new = var.rnd.Rannyu(-1., 1.)* var.delta_x + var.x;
    double val = pow(psi_t(x_new, var.mu, var.sigma ),2)/pow(psi_t(var), 2) ;    
    
    if(var.rnd.Rannyu()<val){
        var.acceptance++;
        var.x = x_new;    
    }

}

void integrale(variabili &var){
    double sum_prog=0;
    double sum_prog2=0;
    double ave=0;

    for(int i=0; i<var.N; i++){
        for(int j=0; j<var.M; j++){
            metro(var);
            ave += H_psi_t(var);
        }

        ave/=double(var.M);
        sum_prog+=ave;
        sum_prog2+= ave*ave;


        ave=0;      
    }
    
    var.H = sum_prog/double(var.N);
    var.sigma_H =error(var.H, sum_prog2/double(var.N), var.N-1);

}

void Calcolo_H_e_Psi(variabili &var, const string& fileH, const string& filePsi){
    double sum_prog=0;
    double sum_prog2=0;
    double ave=0;

    ofstream outputfile1(fileH);
    ofstream outputfile2(filePsi);

    for(int i=0; i<var.N; i++){
        for(int j=0; j<var.M; j++){
            metro(var);
            ave += H_psi_t(var);

            outputfile2 << var.x << "\n";
        }

        ave/=double(var.M);
        sum_prog+=ave;
        sum_prog2+= ave*ave;

        
        double average = sum_prog/(i+1);
        double err =  error(average, sum_prog2/(i+1), i);
        outputfile1 << fixed << setprecision(6);
        outputfile1 << average << setw(12) << err << endl;
        
        ave=0;   
    }

    outputfile1.close();
    outputfile2.close();
    

}

void metro_mu(variabili &var){
    double mu_old = var.mu;
    var.mu = var.rnd.Rannyu(-1., 1.)*var.delta_mu + var.mu;
    double H= var.H;
    double sigma_H = var.sigma_H;

    integrale(var);

    double prob= exp(-(var.H- H)/var.T);
    
    if(var.rnd.Rannyu()< prob){
        var.acc_mu++;
    } else{
        var.mu= mu_old;
        var.H= H;
        var.sigma_H = sigma_H;
    }

}

void metro_sigma(variabili &var){
    double sigma_old = var.sigma;
    var.sigma = var.rnd.Rannyu(-1., 1.)*var.delta_sigma + var.sigma;
    double H= var.H;
    double sigma_H = var.sigma_H;

    integrale(var);

    double prob= exp(-(var.H- H)/var.T);
    
    if(var.rnd.Rannyu()< prob){
        var.acc_sigma++;
    } else{
        var.sigma= sigma_old;
        var.H= H;
        var.sigma_H = sigma_H;
    }
    
}




void Print(variabili& var, ofstream& outputfile){
    outputfile << fixed << setprecision(6);
    outputfile << var.T << setw(12) <<  var.H << setw(12) << var.sigma_H << setw(12) << var.mu << setw(12) << var.sigma << endl;
}

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
    
    variabili var= {
        .N=100,
        .M=100,
        .delta_x=2.,
        .delta_sigma= 0.22,
        .delta_mu=0.25,
        .x= 1.,
        .sigma = 1.,
        .mu= 1.,
        .acceptance = 0,
        .acc_mu = 0,
        .acc_sigma = 0,
        .rnd= rnd,
        .H=0,
        .sigma_H=0,
        .T = 2.0,
    };

    ofstream outputfile;
    outputfile.open("Dati.dat");

    int N=50;
    int M=100;

    for(int i=0; i<N; i++){

        var.acceptance = 0;
        var.acc_mu = 0;
        var.acc_sigma = 0;
        
        for(int j=0; j<M; j++){
            metro_mu(var);
            metro_sigma(var);
            
        }

        double tentativi_mu_sigma = static_cast<double>(M); 
        
        double tentativi_x = static_cast<double>(M * 2 * var.N * var.M);

        cout << "Accettanza = " << double(var.acceptance / tentativi_x) << "\n";
    
        cout<< "Accettanza = " <<double(var.acc_mu / tentativi_mu_sigma) << "\n" ;
        
        cout << "Accettanza = " << double(var.acc_sigma / tentativi_mu_sigma) << "\n" ;

        var.T*=0.9;

        Print(var, outputfile);
    }

    var.M=1000;

    Calcolo_H_e_Psi(var, "H.dat", "posizioni.dat");

    outputfile.close();

    return 0;

}


