
#ifndef __Population__
#define __Population__

#include <armadillo>
#include <vector>
#include <fstream>
#include "chromosome.h"

class Population{

private:
    int nist;
    int ndim;
    std::vector<Chromosome> pop;
    std::vector<Chromosome> pop_new;
    double l_ave;
    Chromosome best;
    Random rnd;
    arma::mat dist;

    double prob_crossover; //probabilità che avvenga il crossover
    double prob_mutation1; //probabilità che avvenga una mutazione
    double prob_mutation2;
    double prob_mutation3;
    double prob_mutation4;
    double val_selection; //esponente nella funzione selection

    int rank;
    int size;


public: 
    Population(int dim, int ist, const Random &var_rand,const arma::mat &distances, int ran, int siz);     
    void initialize();
    void sort_new();
    void crossover(int a, int b);
    int selection();
    void new_pop();
    void mutation1();
    void mutation2();
    void mutation3();
    void mutation4();
    void l_average();
    double get_l_ave(){ return l_ave;};
    void move();
    void check();
    Chromosome get_best(){return best;};
    Chromosome get_zero(){return pop[0];};
    void print_best();
    void print_l_ave();
    void migrant(arma::ivec &);

};



#endif // __Population__