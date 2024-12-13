#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <map>
#include <vector>
#include<direct.h>

using namespace std;

pair<long long, string> res_to_Dimacs(string res)
{
	long long counter = 0;
	stringstream ss(" " + res);
	string conjunkt;
	string ans = "";
	while (!ss.eof()) {
		getline(ss, conjunkt, ']');
		if (conjunkt != "")
		{
			conjunkt = conjunkt.substr(3, conjunkt.length() - 3);
			stringstream sc(conjunkt);
			string word;
			while (getline(sc, word, ','))
			{
				//cout << word;
				ans += word;
			}
			counter++;
			//cout << "\n";
			ans += "\n";
		}
	}
	return { counter, ans };
}

string check_m(int n, int k);

int vars_count();
string first_step_variables(int n, int k);
string generate_inequalities(int n, int k, int d, int step);// x1 + x2 + x3 >= d, mknf
string generate_equi(int n, int k, int d, int step);// equi;
void set0();

int reduction(int n, int k, int d, int want_check_matrix)
{
	map<string, int> letterToNumber;
	vector<string> numberToString;

	set0();

	string inequality;
	string a1_i_j;// a1_i_j -- xij; a2_(i1).(i2)_j -- y(i1)(i2)_j; .. 
	string mknf;
	string f1;
	string equi;
	string res = "[";

	if (want_check_matrix)
	{
		res = check_m(n, k);
	}

	int counter_var = 1;
	for (int step = 1; d > step && step <= k; step++)
	{
		//inequality
		cout << "step" << step << ":\n";

		string ans = "";
		if (step > 1)
		{
			ans = generate_equi(n, k, d, step);
			res = res.substr(0, res.size() - 1) + ",\n" + ans.substr(2, ans.size() - 1);
			cout << "\n";
		}

		ans = generate_inequalities(n, k, d, step);//inequalities + mknf
		if (res != "[")
			res = res.substr(0, res.size() - 1) + ",\n";
		res += ans.substr(2, ans.size() - 1);

		cout << "\n\n";
	}
	if (res == "[")
		res += "]";

	string answer_file_name = "../saved_answers/(" + to_string(n) + ", " + to_string(k) + ", " + to_string(d) + ")";
	const char* fname = answer_file_name.c_str();
	if (_mkdir(fname) == -1)
		cout << "already exists\n";
	else
		cout << "Directory created\n";

	ofstream out;  // поток для записи
	//out.open("../py/myconfig.py");  // открываем файл для записи
	out.open(answer_file_name + "/cnf.py");  // открываем файл для записи
	if (out.is_open()) {
		out << "clauses = " + res << std::endl;
	}
	out.close();
	out.open("../py/myconfig.py");  // открываем файл для записи
	if (out.is_open()) {
		out << "dir_name = \"" + answer_file_name + "/sat_result.txt\"" << std::endl << "clauses = " + res << std::endl;
	}
	out.close();
	cout << res << "\n";

	pair<long long, string> res2 = res_to_Dimacs(res);
	out.open(answer_file_name + "/dimacs.txt");  // открываем файл для записи
	if (out.is_open()) {
		out << "p cnf " << vars_count() - 1 << " " << res2.first << std::endl;
		out << res2.second;
	}
	out.close();
	cout << "Dimacs created";

	string res3 = first_step_variables(n, k);
	out.open(answer_file_name + "/dimacs_core_vars.txt");  // открываем файл для записи
	if (out.is_open()) {
		out << res3 << std::endl;
	}
	out.close();
	//cout << res2.second << "\n";

	///string Dimacs_res = res_to_Dimacs(res);

	return 0;
}