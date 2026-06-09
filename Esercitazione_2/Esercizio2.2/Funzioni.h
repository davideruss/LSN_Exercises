
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include "random.h"

const double pi= M_PI;

std::vector<double> error(std::vector<double>&, std::vector<double>&, int);

void passo1(std::vector<std::array<double, 3>>&, Random &, int);

void passo2(std::vector<std::array<double, 3>>&, Random &, int);

void RW1(std::vector<std::array<double, 3>>&, int, Random &);

void RW2(std::vector<std::array<double, 3>>&, int, Random &);

void analisi(int n_passi, int N, int M, std::ofstream &outputfile, Random& rnd, bool);

std::vector<double> modulo(const std::vector<std::array<double, 3>>& vec);

std::vector<double>& operator +=(std::vector<double> &a, const std::vector<double>& b);

std::vector<double> operator -(const std::vector<double> &a, const std::vector<double>& b);

std::vector<double>& operator /=(std::vector<double> & a, const double& b);

std::vector<double> operator /(const std::vector<double> & a, const double& b);

std::vector<double> operator*(const std::vector<double> & a ,const  std::vector<double>& b);

std::vector<double> sqrt(const std::vector<double>&);


