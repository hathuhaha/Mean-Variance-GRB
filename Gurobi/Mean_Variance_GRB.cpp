#include "gurobi_c++.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "data.h"
#include "GRB_model.h"
#include <filesystem>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

int main() {
    try {
        int N, T;
        vector<double> Mean;
        vector<vector<double>> Covariance;

        for (int stt = 1; stt <= 1; stt++) {
            string filename = "data" + to_string(stt) + ".txt";
            read_data(filename, N, T, Mean, Covariance);

            string fileout = "Ans_Maximize_Profit_GRB" + to_string(stt) + ".txt";
            ofstream output(fileout);
            fileout = "Ans_Minimize_Risk_GRB" + to_string(stt) + ".txt";
            ofstream out(fileout);
            fileout = "Ans_Profit_Risk_Optimize_GRB" + to_string(stt) + ".txt";
            ofstream outfile(fileout);

            for (int P = 2; P <= 10; P++) {
                output << "choose = " << P << "\n";
                out << "choose = " << P << "\n";
                outfile << "choose = " << P << "\n";
                for (double MRisk = 0.0001; MRisk <= 0.002; MRisk += 0.0001) {
                    SetupModel(output, N, Mean, Covariance, P, "Profit", MRisk);
                }

                for (double MProfit = 0.0001; MProfit <= 0.02; MProfit += 0.0001) {
                    SetupModel(out, N, Mean, Covariance, P, "Risk", MProfit);
                }
                for (double lambda = 0.00; lambda <= 1.0; lambda += 0.01) {
                    SetupModel(outfile, N, Mean, Covariance, P, "Balance", lambda);
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
    catch (GRBException& e) {
        cerr << "Error code = " << e.getErrorCode() << endl; cerr << "Error message = " << e.getMessage() << endl; return 1;
    }
    catch (...) {

    }
}
