#include <ilcplex/cplex.h>
#include <ilcplex/ilocplex.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Data.h"
#include "CPLEX_MODEL.h"
#include <chrono>
using namespace std;

vector<pair<int, int> > ;
int main() {
	for (int file = 2; file <= 4; file++) {
		vector<vector<double> > Covariance;
		string datafile = "data" + to_string(file) + ".txt";
		int N, T;
		vector<double> Mean;
		read_data(datafile, N, T, Mean, Covariance);
		string filename;

		filename = "Ans_Maximize_Profit" + to_string(file) + ".txt";
		ofstream output(filename);

		filename = "Ans_Minimize_Risk" + to_string(file) + ".txt";
		ofstream out(filename);

		filename = "Ans_Profit_Risk_Optimize" + to_string(file) + ".txt";
		ofstream outfile(filename);

		for (int eli = 2; eli <= 10; eli++) {
			try {
				output << "choose = " << eli << "\n";
				out << "choose = " << eli << "\n";
				outfile << "choose = " << eli << '\n';
				for (double MRisk = 0.0001; MRisk <= 0.002; MRisk += 0.0001) {
					IloEnv env;
					IloModel model(env);
					vector<IloNumVar> w;
					w.resize(N);
					for (int i = 0; i < N; i++)
						w[i] = IloNumVar(env, 0, 1, ILOFLOAT);
					auto st = std::chrono::steady_clock::now();
					if (Profit_Maximize(output, env, model, N, T, Covariance, Mean, w, MRisk, eli)) {
						auto ed = std::chrono::steady_clock::now();
						auto duration = chrono::duration_cast<chrono::milliseconds>(ed - st);
						output << duration.count() << '\n';
					}
					model.end();
					env.end();
				}

				for (double MProfit = 0.0001; MProfit <= 0.02; MProfit += 0.0001) {
					IloEnv env;
					IloModel model(env);
					vector<IloNumVar> w;
					w.resize(N);
					for (int i = 0; i < N; i++)
						w[i] = IloNumVar(env, 0, 1, ILOFLOAT);
					auto st = std::chrono::steady_clock::now();
					if (Risk_Minimize(out, env, model, N, T, Covariance, Mean, w, MProfit, eli)) {
						auto ed = std::chrono::steady_clock::now();
						auto duration = chrono::duration_cast<chrono::milliseconds>(ed - st);
						out << duration.count() << '\n';
					}
					model.end();
					env.end();
				}
				for (double lambda = 0; lambda <= 1; lambda += 0.01) {
					IloEnv env;
					IloModel model(env);
					vector<IloNumVar> w;
					w.resize(N);
					for (int i = 0; i < N; i++)
						w[i] = IloNumVar(env, 0, 1, ILOFLOAT);
					auto st = std::chrono::steady_clock::now();
					if (Profit_Risk_Optimize(outfile, env, model, N, T, Covariance, Mean, w, lambda, eli)) {
						auto ed = std::chrono::steady_clock::now();
						auto duration = chrono::duration_cast<chrono::milliseconds>(ed - st);
						outfile << duration.count() << '\n';
						cout << duration.count() << '\n';
					}
					model.end();
					env.end();
				}
			}
			catch (IloException& e) {
				cout << "Error " << e << '\n';
			}
			catch (...) {
				cout << "Oh no\n";
			}
			output << '\n';
			out << '\n';
			outfile << '\n';
		}
		output.close();
		out.close();
		outfile.close();
	}
}