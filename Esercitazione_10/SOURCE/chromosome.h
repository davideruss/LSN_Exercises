#include <armadillo>
#include "random.h"
#ifndef __Chromosome__
#define __Chromosome__

class Chromosome{

private:
    int ndim;
    arma::ivec chr;
    arma::mat dist;
    double l_cost;

public: // Function declarations
    Chromosome() = default;
    Chromosome(int,const arma::mat &);
    Chromosome(const Chromosome& a);

    void changeconf(const arma::ivec& chr) ; 
    const arma::ivec getconf() const{return chr;}
    void cost(); 
    double get_cost() const;
    void swap(int , int );
    
};

#endif // __Chromosome__