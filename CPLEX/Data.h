#ifndef DATA_H
#define DATA_H
#include "ilcplex\cplex.h"
#include "ilcplex\ilocplex.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
using namespace std;
void read_data(const string& datafile, int& N, int& T, vector<double>& Mean, vector<vector<double>>& Covariance);
#endif 
