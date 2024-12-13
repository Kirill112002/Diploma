#include <iostream>
#include <fstream>
#include <string>
#include <direct.h>

using namespace std;

void set0();													// zeroing, prepare for another task
string check_m(int n, int k);									// add conditions for base parametrs (equal to given)
string generate_equi(int n, int k, int d, int step);			// a(step)_i1i2..i(step)_j ≡ a(step-1)_i1i2..i(step-1)_j ⊕ a1_i(step)_j   -----> cnf
string generate_inequalities(int n, int k, int d, int step);	// Σ[j = 1..n-k](a(step)_i1.i2..i(step)_j) ⩽ d - step   -----> cnf
string first_step_variables(int n, int k);						// return all a1_i_j numbers
int vars_count();												// return count of variables
pair<long long, string> res_to_Dimacs(string res);				// return pair {count of base variables, cnf clauses in dimacs form}
void output_res_to_file(string file_name, string text);  // printing results to files


int reduction(int n, int k, int d, int want_check_matrix)
{
	//part 0 -- zeroing
	string res = "[]"; 
	pair<long long, string> res_Dimacs;
	string ans = "";
	string answer_file_name = "../saved_answers/(" + to_string(n) + ", " + to_string(k) + ", " + to_string(d) + ")";
	const char* fname = answer_file_name.c_str();
	string base_variable_numbers;
	set0();

	//part 1 -- if we already have code matrix add conditions (this will speed up SAT-Solver result generation)
	if (want_check_matrix)
		res = check_m(n, k);
	
	//part 2 -- create conditions

	for (int step = 1; d > step && step <= k; step++)
	{
		cout << "step" << step << ":\n";
		ans = "";

		//part 2.1 -- create new variables and equivalent conditions for them // a(step)_i1i2..i(step)_j ≡ a(step-1)_i1i2..i(step-1)_j ⊕ a1_i(step)_j
		if (step > 1)
		{
			ans = generate_equi(n, k, d, step);
			res = res.substr(0, res.size() - 1) + ",\n" + ans.substr(2, ans.size() - 1);
		}

		//part 2.2 -- create condition, that sum in a row smaller then d // Σ[j = 1..n-k](a(step)_i1.i2..i(step)_j) ⩽ d - step
		ans = generate_inequalities(n, k, d, step);
		if (res != "[]")
			res = res.substr(0, res.size() - 1) + ",\n";
		res = res.substr(0, res.size() - 1) + ans.substr(2, ans.size() - 1) + "]";
	}

	//part 3 save results
	//part 3.1 create directory
	if (_mkdir(fname) == -1)
		cout << "Directory already exists\n";
	else
		cout << "New directory created\n";

	//part 3.2 save CNF results for pySat
	output_res_to_file(answer_file_name + "/cnf.py", "clauses = " + res);
	output_res_to_file("../py/myconfig.py", "dir_name = \"" + answer_file_name + "/sat_result.txt\"\nclauses = " + res);
	cout << res << "\n";

	//part 3.3 save CNF in Dimacs format + save base variables for quicker sat-solver working
	res_Dimacs = res_to_Dimacs(res);
	base_variable_numbers = first_step_variables(n, k);
	output_res_to_file(answer_file_name + "/dimacs.txt", "p cnf " + to_string(vars_count()) + " " + to_string(res_Dimacs.first) + "\n" + res_Dimacs.second);
	output_res_to_file(answer_file_name + "/dimacs_core_vars.txt", base_variable_numbers);
	cout << "Dimacs created\n";

	return 0;
}