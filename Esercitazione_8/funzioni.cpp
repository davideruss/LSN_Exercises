#include "funzioni.h"

using namespace std;

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

void Calcolo_psi_analitica(variabili &var, const string& file_psi_a){
    
    ofstream outputfile(file_psi_a);
    
    outputfile << fixed << setprecision(6);
    for(double t=-5; t<5.01; t+= 0.05){

        outputfile << t << setw(12) << psi_t(t, var.mu, var.sigma) << "\n";
    }
    outputfile.close();

}


void Print(variabili& var, ofstream& outputfile){
    outputfile << fixed << setprecision(6);
    outputfile << var.T << setw(12) <<  var.H << setw(12) << var.sigma_H << setw(12) << var.mu << setw(12) << var.sigma << endl;
}