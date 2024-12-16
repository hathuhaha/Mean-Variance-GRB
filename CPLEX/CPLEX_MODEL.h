#ifndef CPLEX_MODEL_H
#define CPLEX_MODEL_H
#include <string>
#include <vector>
#include <ilcplex/cplex.h>
#include <iostream>
#include <ilcplex/ilocplex.h>
#include <fstream>
#include <chrono>

using namespace std;
bool Profit_Maximize(ofstream& output, IloEnv& env, IloModel& model, const int& N, const int& T, const vector<vector<double> >& Covariance, const vector<double>& Mean, vector<IloNumVar>& w, const double& MRisk, const int& eli);
bool Risk_Minimize(ofstream& output, IloEnv& env, IloModel& model, const int& N, const int& T, const vector<vector<double> >& Covariance, const vector<double>& Mean, vector<IloNumVar>& w, const double& MProfit, const int& eli);
bool Profit_Risk_Optimize(ofstream& output, IloEnv& env, IloModel& model, const int& N, const int& T, const vector<vector<double> >& Covariance, const vector<double>& Mean, vector<IloNumVar>& w, const double& lambda, const int& eli);
#endif 
