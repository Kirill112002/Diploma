#include <iostream>
#include <fstream>
#include <string>
#include <direct.h>

using namespace std;

void set0(int n, int k, int d, int mode);									// zeroing, prepare for another task
string check_m(int n, int k);									// add conditions for base parametrs (equal to given)
string symmetry_breaking(int n, int k, int d); 									// printing results to files
string generate_equi(int n, int k, int d, int step);			// a(step)_i1i2..i(step)_j ≡ a(step-1)_i1i2..i(step-1)_j ⊕ a1_i(step)_j   -----> cnf
string generate_cnf_inequalities(int n, int k, int d, int step, bool verification_matrix_mode);	// Σ[j = 1..n-k](a(step)_i1.i2..i(step)_j) ⩽ d - step   -----> cnf
string first_step_variables(int n, int k);						// return all a1_i_j numbers
string res_to_Dimacs(string res);								// return Dimacs format of cnf "p cnf vars_count disjunct_count\n" for(i = 1; i <= disjunct_count; i++)"disjunct_i\n"
void output_res_to_file(string file_name, string text);			// printing results to files
void set_vars_count(int cur);
void print_all_LtoN();

int reduction(int n, int k, int d, int mode)
{
	//part 0 -- zeroing
	string res = "[]";
	pair<long long, string> res_Dimacs;
	string ans = "";

	//part 0.1 -- create n,k,d constants needed for code
	set0(n, k, d, mode);
	const string answer_file_name = "../saved_answers/(" + to_string(n) + ", " + to_string(k) + ", " + to_string(d) + ")";
	const char* fname = answer_file_name.c_str();
	const string base_variable_numbers = first_step_variables(n, k);
	bool verification_matrix_mode = true;

	//part 1 -- if we already have code matrix add conditions (this will speed up SAT-Solver result generation) //toDo write after conditions generate
	if (mode == 2)
		res = check_m(n, k);
	print_all_LtoN();
	res = symmetry_breaking(n, k, d);// +=

	//cout << res << "\n";
	
	//part 2 -- create conditions
	int cur_con = 4;
	for (int step = 1; d > step && step <= k; step++)
	{
		cout << "step" << step << ":\n";
		ans = "";

		//part 2.1 -- create new variables and equivalent conditions for them // a(step)_i1i2..i(step)_j ≡ a(step-1)_i1i2..i(step-1)_j ⊕ a1_i(step)_j
		if (step > 1)
		{
			//ans = generate_equi(n, k, d, step);
			if(step <= cur_con)//cur_con
				ans = generate_equi(n - k, k, d, step);
			else
				ans = generate_equi(n - k, step, d, step);

			res = res.substr(0, res.size() - 1) + ",\n" + ans.substr(2, ans.size() - 1);
		}

		//part 2.2 -- create condition, that sum in a row smaller then d // Σ[j = 1..n-k](a(step)_i1.i2..i(step)_j) ≥ d - step
		//ans = generate_cnf_inequalities(n, k, d, step, verification_matrix_mode);
		if (step <= cur_con)
			ans = generate_cnf_inequalities(n - k, k, d, step, verification_matrix_mode);
		else
			ans = generate_cnf_inequalities(n - k, step, d, step, verification_matrix_mode);
		if (res != "[]")
			res = res.substr(0, res.size() - 1) + ",\n]";
		cout << ans << "\n\n";
		res = res.substr(0, res.size() - 1) + ans.substr(2, ans.size() - 1);
	}

	if (mode == 1)
	{
		output_res_to_file("../py/myconfig2_extra.py", "clauses2 = " + res);
	}
	else
	{
		output_res_to_file("../py/myconfig.py", "dir_name = \"" + answer_file_name + "/sat_result.txt\"\nclauses = " + res);//myconfig.py
		//output_res_to_file("../py/myconfig2_extra.py", "clauses2 = []");
	}
	//verification_matrix();
	
	//part 3 save results
	//part 3.1 create directory
	if (_mkdir(fname) == -1)
		cout << "Directory already exists\n";
	else
		cout << "New directory created\n";

	//part 3.2 save CNF results for pySat

	output_res_to_file(answer_file_name + "/cnf.py", "clauses = " + res);
	cout << res << "\n";

	//part 3.3 save CNF in Dimacs format + save base variables for quicker sat-solver working
	output_res_to_file(answer_file_name + "/dimacs.txt", res_to_Dimacs(res));
	output_res_to_file(answer_file_name + "/dimacs_core_vars.txt", base_variable_numbers);
	cout << "Dimacs created\n";
	
	return 0;
}