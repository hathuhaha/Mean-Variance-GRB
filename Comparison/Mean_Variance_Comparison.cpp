#include <iostream>
#include <string>
#include <fstream>
#include <map>
long long totalTime[2][2][3], cnt[2][2][3];
void INPUT(std::string filename, bool solver, int model, bool type) {
	std::fstream check(filename);
	std::fstream input(filename);
	if (!check.is_open() || !input.is_open())
		return;
	int cur_choose = -1;
	while (true) {
		try {
			std::string line = "";
			std::getline(check, line);
			if (line == "" && (cur_choose == 10 || cur_choose == -1))
				break;
			if (line == "") {
				continue;
			}
			if (line[0] == 'c') {
				std::string X, Y;
				input >> X >> Y >> cur_choose;
				//std::cout << X << ' ' << Y << ' ' << cur_choose << '\n';
			}
			else {
				if (model < 2) {
					double mVal, returns, variance, time;
					input >> mVal >> returns >> variance >> time;
					totalTime[type][solver][model] += time;
					cnt[type][solver][model]++;
					//std::cout << mVal << ' ' << returns << ' ' << variance << ' ' << time << '\n';
				}
				else {
					double lambda, weight, time;
					input >> lambda >> weight >> time;
					totalTime[type][solver][model] += time;
					cnt[type][solver][model]++;
					//std::cout << lambda << ' ' << weight << ' ' << time << '\n';
				}
			}
		}
		catch (...) {
			break;
		}
	}
	check.close();
	input.close();
}
int main() {
	std::string modelName[3] = {"Ans_Maximize_Profit", "Ans_Minimize_Risk", "Ans_Profit_Risk_Optimize"};
	std::string solverName[2] = {"", "_GRB"};
	std::string typeName[2] = { "Unconstrained_", ""};
	std::ofstream output("result.csv");
	output << ", unconstrained (CPLEX), unconstrained (GRB), constrained (CPLEX), constrained (GRB)\n";
	for (int file = 3; file <= 3; file++) {
		for (int solver = 0; solver < 2; solver++) {
			for (int model = 0; model < 3; model++) {
				for (int type = 0; type < 2; type++) {
					std::string filename = typeName[type] + modelName[model] + solverName[solver] + std::to_string(file) + ".txt";
					INPUT(filename, solver, model, type);
				}
			}
		}
		for (int model = 0; model < 3; model++) {
			output << "model " << model + 1;
			for (int type = 0; type < 2; type++) {
				for (int solver = 0; solver < 2; solver++) {
					output << ", " << (double(totalTime[type][solver][model]) / double(cnt[type][solver][model])) / 1e3;
				}
			}
			output << '\n';
		}
		for (int solver = 0; solver < 2; solver++) {
			for (int model = 0; model < 3; model++) {
				for (int type = 0; type < 2; type++) {
					totalTime[type][solver][model] = cnt[type][solver][model] = 0;
				}
			}
		}

	}
}

