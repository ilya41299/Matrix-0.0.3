#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

bool read_name(string & s1, char & op, string & s2) {
	bool f = false;
	string line;
	getline(cin, line);
	istringstream stream(line);
	if (stream >> s1 && stream >> op) {
		if (op == 'T' || op == 'R') {
			f = !f;
			return f;
		}
		else {
			if (stream >> s2) {
				f = !f;
			}
		}
	}
	return f;
}

bool vvod_mas(float ** &mas, int &rows, int &columns, string s) {
	char op;
	ifstream fin;
	fin.open(s.c_str());
	if (!fin.is_open()) {
		return false;
	}

	if (fin >> rows && fin >> op && op == ',' && fin >> columns) {
		mas = new float *[rows];
		for (int i = 0; i < rows; i++) {
			mas[i] = new float[columns];
			for (int j = 0; j < columns; j++) {
				fin >> mas[i][j];
			}
		}
	}
	fin.close();
	return true;
}

void destroy(int rows1, float ** &mas1, float ** &result_mas)
{
	for (int i = 0; i<rows1; ++i) {
		delete[]mas1[i];
	}
	delete[]mas1;
	delete[]result_mas;
}

void add(float ** mas1, float ** mas2, int rows, int columns, float ** &result_mas) {
	result_mas = new float *[rows];
	for (int i = 0; i < rows; i++) {
		result_mas[i] = new float[columns];
		for (int j = 0; j < columns; j++) {
			result_mas[i][j] = (mas1[i][j] + mas2[i][j]);
		}
	}
}

void sub(float ** mas1, float ** mas2, int rows, int columns, float ** &result_mas) {
	result_mas = new float *[rows];
	for (int i = 0; i < rows; i++) {
		result_mas[i] = new float[columns];
		for (int j = 0; j < columns; j++) {
			result_mas[i][j] = (mas1[i][j] - mas2[i][j]);
		}
	}
}

void mult(float ** mas1, float ** mas2, int rows1, int columns1, int rows2, int columns2, float ** &result_mas) {
	int result_rows = rows1;
	int result_columns = columns2;
	result_mas = new float *[result_rows];
	for (int i = 0; i<result_rows; ++i) {
		result_mas[i] = new float[result_columns];
		for (int j = 0; j<result_columns; ++j) {
			float result = 0;
			for (int k = 0; k<columns1; ++k) {
				result += mas1[i][k] * mas2[k][j];
			}
			result_mas[i][j] = result;
		}
	}
}

void Trans(float ** &result_mas, float ** mas1, int rows, int columns) {
	result_mas = new float *[rows];
	for (int i = 0; i < columns; i++) {
		result_mas[i] = new float[columns];
		for (int j = 0; j < rows; j++) {
			result_mas[i][j] = mas1[j][i];
		}
	}
}

bool revers_mas(float ** &result_mas, float ** mas1, int rows) {
	float a, b, result;                          // Результирующая сатрица принимает вид единичной матрицы
	for (int i = 0; i < rows; i++) {
		result_mas[i] = new float[rows];
		for (int j = 0; j < rows; j++) {
			if (i == j) {
				result_mas[i][j] = 1;
			}
			else result_mas[i][j] = 0;
		}
	}
	for (int i = 0; i < rows; i++) {
		a = mas1[i][i];
		for (int j = i + 1; j < rows; j++) {
			b = mas1[j][i];
			for (int k = 0; k < rows; k++) {
				mas1[j][k] = mas1[i][k] * b - mas1[j][k] * a;
				result_mas[j][k] = result_mas[i][k] * b - result_mas[j][k] * a;
			}
		}
	}
	for (int i = 0; i < rows; i++) {
		for (int j = rows - 1; j >= 0; j--) {
			result = 0;
			for (int k = rows - 1; k > j; k--) {
				result += mas1[j][k] * result_mas[k][i];
				if (mas1[j][j] == 0) {
					for (i = 0; i < rows; i++) {
						delete[] result_mas[i];
						delete[]result_mas;
					}
				}
			}
			if (mas1[j][j] == 0) {
				cout << "There is no reverse matrix" << endl;
				return false;
			}
			result_mas[j][i] = (result_mas[j][i] - result) / mas1[j][j];
		}
	}
	return true;
}


int main() {
	string s1, s2;
	char op;
	if (!(read_name(s1, op, s2))) {
		cout << "ERROR" << endl;
		return -1;
	}
	float ** mas1;
	int rows1, columns1;
	if (!(vvod_mas(mas1, rows1, columns1, s1))) {
		cout << "An error has occured while reading input data vvod";
		return -1;
	}
	float **result_mas = new float *[columns1];
	if (op == 'T') {
		Trans(result_mas, mas1, rows1, columns1);
	}
	if (op == 'R' && rows1 == columns1) {

		if (!(revers_mas(result_mas, mas1, rows1))) {
			return -1;
		}
	}
	if (op == '*') {
		float ** mas2; 
		int rows2, columns2;
		if (vvod_mas(mas2, rows2, columns2, s2) && rows1 == columns2) {
			mult(mas1, mas2, rows1, columns1, rows2, columns2, result_mas);
		}
		else {
			cout << "An error has occured while reading input data" << endl;
			destroy(rows1, mas1, result_mas);
			return -1;
		}
	}
	if (rows1 == columns1 && (op == '-' || op == '+')) {
		float ** mas2;
		int rows2, columns2;
		if (vvod_mas(mas2, rows2, columns2, s2) && rows1 == rows2 && columns1 == columns2) {
			switch (op) {
			case '+': {
				add(mas1, mas2, rows1, columns1, result_mas);
				break;
			}
			case '-': {
				sub(mas1, mas2, rows1, columns1, result_mas);
				break;
			}
			default: cout << "An error has occured while reading input data";
			}
		}
	}
	
	if (op != 'T' && op != 'R' && op != '+' && op != '-' && op != '*') {
		cout << "An error has occured while reading input data";
		destroy(rows1, mas1, result_mas);
		return -1;
	}

	cout << endl;
	for (int i = 0; i < rows1; i++) {
		for (int j = 0; j < columns1; j++) {
			cout << result_mas[i][j] << ' ';
		}
		cout << endl;
	}
	destroy(rows1, mas1, result_mas);
}
