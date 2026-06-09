#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include "random.h"


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
double error(double av, double av2, int n);

//funzioni per il calcolo dell'integrale

double psi_t(variabili & var);
double psi_t(double &x, double &mu , double &sigma);

double H_psi_t(variabili &var);
 
void metro(variabili &var );

void integrale(variabili &var);

void Calcolo_H_e_Psi(variabili &var, const std::string& fileH, const std::string& filePsi);

void metro_mu(variabili &var);

void metro_sigma(variabili &var);

void Calcolo_psi_analitica(variabili &var, const std::string& file_psi_a);



void Print(variabili& var, std::ofstream& outputfile);