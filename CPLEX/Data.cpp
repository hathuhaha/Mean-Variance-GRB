#include "Data.h"
#define _CRT_SECURE_NO_WARNINGS
#define input cin
void read_data(const string& datafile, int& N, int& T, vector<double>& Mean, vector<vector<double>>& Covariance) {
	FILE* file;
	freopen_s(&file, datafile.c_str(), "r", stdin);

	input >> N >> T;
	Covariance.resize(N);
	for (int i = 0; i < N; i++)
		Covariance[i].resize(N);

	vector<vector<double> > stock_price(N, vector<double>(T + 1));
	vector<vector<double> > stock_return(N, vector<double>(T));

	for (int i = 0; i < N; i++) {
		for (int j = 0; j <= T; j++) {
			input >> stock_price[i][j];
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < T; j++) {
			stock_return[i][j] = (stock_price[i][j + 1] - stock_price[i][j]) / stock_price[i][j];
		}
	}

	Mean.resize(N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < T; j++)
			Mean[i] += stock_return[i][j];
		Mean[i] /= T;
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			double sum = 0;
			for (int k = 0; k < T; k++)
				sum += (stock_return[i][k] - Mean[i]) * (stock_return[j][k] - Mean[j]);

			Covariance[i][j] = sum / (T - 1);
		}
	}
}
