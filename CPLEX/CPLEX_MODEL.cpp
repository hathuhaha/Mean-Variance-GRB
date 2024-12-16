#include "CPLEX_MODEL.h"


static bool Solve_and_Print(ofstream& output, IloCplex& cplex, const double& Limit, IloExpr& Profit, IloExpr& Risk, string type) {
	if (cplex.solve()) {
		// duration.count() cplex.getTime()
		output << Limit << ' ' << fixed << setprecision(8) << cplex.getValue(Profit) << ' ' << cplex.getValue(Risk) << ' ';
		cplex.clear();
		return true;
	}
	cplex.clear();
	return false;
}

bool Profit_Maximize(ofstream& output, IloEnv& env, IloModel& model, const int& N, const int& T, const vector<vector<double>>& Covariance, const vector<double>& Mean, vector<IloNumVar>& w, const double& MRisk, const int& eli) {
	IloExpr weight(env);
	IloExpr Profit(env);
	IloExpr Risk(env);
	vector<IloNumVar> choose;
	choose.resize(N);
	for (int i = 0; i < N; i++) {
		weight += w[i];
		choose[i] = IloNumVar(env, 0, 1, ILOBOOL);
	}
	model.add(weight == 1);
	IloExpr EliSum(env);
	for (int i = 0; i < N; i++) {
		EliSum += choose[i];
	}
	model.add(EliSum <= eli);
	for (int i = 0; i < N; i++) {
		model.add(w[i] <= choose[i]);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Risk += w[i] * Covariance[i][j] * w[j];
		}
	}

	model.add(Risk <= MRisk);

	for (int i = 0; i < N; i++) 
		Profit += Mean[i] * w[i];

	model.add(IloMaximize(env, Profit));
	IloCplex cplex(model);
	cplex.extract(model);
	cplex.setParam(IloCplex::Param::OptimalityTarget, 3);
	cplex.setParam(IloCplex::Param::TimeLimit, 3600);
	cplex.setParam(IloCplex::Param::MIP::PolishAfter::Time, 3000);
	return Solve_and_Print(output, cplex, MRisk, Profit, Risk, "Profit");

}
bool Risk_Minimize(ofstream &output, IloEnv& env, IloModel& model, const int& N, const int& T, const vector<vector<double>>& Covariance, const vector<double>& Mean, vector<IloNumVar>& w, const double& MProfit, const int& eli) {
	IloExpr weight(env);
	IloExpr Profit(env);
	IloExpr Risk(env);
	vector<IloNumVar> choose;
	choose.resize(N);
	for (int i = 0; i < N; i++) {
		weight += w[i];
		choose[i] = IloNumVar(env, 0, 1, ILOBOOL);
	}
	model.add(weight == 1);
	IloExpr EliSum(env);
	for (int i = 0; i < N; i++) {
		EliSum += choose[i];
	}
	model.add(EliSum <= eli);

	for (int i = 0; i < N; i++) {
		model.add(w[i] <= choose[i]);
	}
	for (int i = 0; i < N; i++)
		Profit += Mean[i] * w[i];
	model.add(Profit == MProfit);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Risk += w[i] * Covariance[i][j] * w[j];
		}
	}
	model.add(IloMinimize(env, Risk));

	IloCplex cplex(model);
	cplex.extract(model);
	cplex.setParam(IloCplex::Param::OptimalityTarget, 3);
	cplex.setParam(IloCplex::Param::TimeLimit, 3600);
	cplex.setParam(IloCplex::Param::MIP::PolishAfter::Time, 3000);
	return Solve_and_Print(output, cplex, MProfit, Profit, Risk, "Risk");

}

bool Profit_Risk_Optimize(ofstream& output, IloEnv& env, IloModel& model, const int& N, const int& T, const vector<vector<double>>& Covariance, const vector<double>& Mean, vector<IloNumVar>& w, const double& lambda, const int& eli) {
	IloExpr weight(env);
	IloExpr Profit(env);
	IloExpr Risk(env);
	vector<IloNumVar> choose;
	choose.resize(N);
	for (int i = 0; i < N; i++) {
		weight += w[i];
		choose[i] = IloNumVar(env, 0, 1, ILOBOOL);
	}
	model.add(weight == 1);
	IloExpr EliSum(env);
	for (int i = 0; i < N; i++) {
		EliSum += choose[i];
	}
	model.add(EliSum <= eli);

	for (int i = 0; i < N; i++) {
		model.add(w[i] <= choose[i]);
	}
	for (int i = 0; i < N; i++)
		Profit += Mean[i] * w[i];
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Risk += w[i] * Covariance[i][j] * w[j];
		}
	}
	IloExpr Object;
	Object = lambda * Risk - (1.0 - lambda) * Profit;
	model.add(IloMinimize(env, Object));
	IloCplex cplex(model);
	cplex.extract(model);
	cplex.setParam(IloCplex::Param::OptimalityTarget, 3);
	cplex.setParam(IloCplex::Param::TimeLimit, 3600);
	cplex.setParam(IloCplex::Param::MIP::PolishAfter::Time, 3000);
	if (cplex.solve()) {
		output << lambda << ' ' << cplex.getValue(Object) << ' ';
		return 1;
	}
	cplex.clear();
	return 0;
}
