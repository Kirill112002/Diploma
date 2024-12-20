#include<string>;
#include<sstream>;
#include <map>;
#include "pblib/pb2cnf.h";

using namespace std;
using namespace PBLib;

//int n, k, d; //toDo add in new commit
map<string, int> ltoN;
int cur_fresh_var = 1;
vector<string> inequivs;
vector<int> answer_from_sat;
int switch_cout_mknf = 1, switch_cout_equi = 0;

//return current number of variables in our CNF-formula
static int get_vars_count()
{
	return cur_fresh_var - 1;
}

//print on screen pairs of number and variable a(step)_i..i(step)_j correspondes to this number
static void print_all_LtoN()
{
	cout << "ltoN:\n";
	for (auto i : ltoN)
		cout << i.first << " " << i.second << "\n";
}

//prepairing program to work with new code
void set0(int n1, int k1, int d1)
{
	map<string, int> empt_map;
	vector<string> empt_vec_str;
	vector<int> empt_vec_int;
	cur_fresh_var = 1;
	ltoN = empt_map;
	inequivs = empt_vec_str;
	answer_from_sat = empt_vec_int;
	//n = n1;
	//k = k1;
	//d = d1;
}

//create new variables
static string make_var(int step, string i, int j)
{
	string a_i_j = "a" + to_string(step) + "_" + i + "_" + to_string(j);
	if (ltoN.find(a_i_j) == ltoN.end())
	{
		ltoN.insert({ a_i_j, cur_fresh_var });
		cur_fresh_var++;
	}
	return a_i_j;
}

//creates and returns base variables
string first_step_variables(int n, int k)
{
	string res = "";
	for (int i = 1; i <= k; i++)
		for (int j = 1; j <= n - k; j++)
			res += to_string(ltoN[make_var(1, to_string(i), j)]) + " ";

	return res;
}


//CREATE CNF

//
string symmetry_breaking(int n, int k, int d)
{
	string ans = "[]", res = "[]";
	//step 1 вес хотя бы одной из строк равен d
	
	//res = res.substr(0, ans.size() - 1) + ans + "]";

	//step 2 лексикографическая сортировка столбцов 

	//experimental maybe FATAL but try
	for (int j = 1; j <= n - k; j++)
		if(j <= d - 1)
			ans = ans.substr(0, ans.size() - 1) + "[" + to_string(ltoN[make_var(1, "1", j)]) + "], ]";
		else
			ans = ans.substr(0, ans.size() - 1) + "[-" + to_string(ltoN[make_var(1, "1", j)]) + "], ]";
	res = ans.substr(0, ans.size() - 3) + "]";

	return res;
}


//create printable version of inequality
static string make_printable_inequality(int jmax, int d, int step, string i)// x1 + x2 + .. + x_jmax >= d - step
{
	string inequality = "", a_i_j;
	for (int j = 1; j <= jmax; j++)
	{
		a_i_j = make_var(step, i, j);
		if (j != 1)
			a_i_j = " + " + a_i_j;
		inequality += a_i_j;
	}
	inequality += " >= " + to_string(d - step);
	return inequality;
}

//transform inequality to cnf form (inequalities on step (step)), n1 = n - k
static string inequality_to_cnf(int n, int k, int d, int step, string i_main)
{
	PB2CNF pb2cnf;
	vector<int64_t> weights(n - k, 1);
	vector<int> literals(n - k);
	string a_i_j;
	for (int j = 1; j <= n - k; j++)
	{
		a_i_j = make_var(step, i_main, j);
		literals[j - 1] = ltoN[a_i_j];
	}

	vector<vector<int>> formula;
	int32_t firstFreshVariable = cur_fresh_var;
	firstFreshVariable = pb2cnf.encodeGeq(weights, literals, d - step, formula, firstFreshVariable) + 1;
	cur_fresh_var = firstFreshVariable;

	string mknf = "[]";
	for (int h = 0; h < formula.size(); h++) //() ^ ()
	{
		stringstream disunkt;
		disunkt << "[";
		if (formula[h].size() > 0)
		{
			for (int j = 0; j < formula[h].size() - 1; j++)//(a v b v c)
				disunkt << formula[h][j] << ", ";
			disunkt << formula[h][formula[h].size() - 1] << "]";

			if (mknf != "[]")
				mknf = mknf.substr(0, mknf.size() - 1) + ", ]";
			mknf = mknf.substr(0, mknf.size() - 1) + disunkt.str() + "]";
		}
	};
	return mknf;
}

//creates 
string generate_cnf_inequalities(int n, int k, int d, int step, bool verification_matrix_mode)// x1 + x2 + x3 >= d
{
	std::vector<bool> v(k);
	std::fill(v.begin(), v.begin() + step, true);
	string res = "", ans = "", i_main = "";
	do {
		i_main = "";
		for (int i = 0; i < k; ++i) {
			if (v[i]) {
				if (i_main != "")
					i_main += ".";
				i_main += to_string(i + 1);
			}
		}
		inequivs.push_back(make_printable_inequality(n - k, d, step, i_main));
		if (switch_cout_mknf)
			cout << inequivs[inequivs.size() - 1] << "\n";
		if (verification_matrix_mode && (d > (step + 1)))
		{
			/*
			for (int j = d - step - 1; j > 0; j--)
			{
				ans = inequality_to_cnf(n, k, d - j, step, i_main);
				res = res.substr(0, res.size() - 1) + ",\n" + ans.substr(1, ans.size() - 1);
			}*/
			ans = inequality_to_cnf(n, k, step + 1, step, i_main);
			res = res.substr(0, res.size() - 1) + ",\n" + ans.substr(1, ans.size() - 1);

		}
		ans = inequality_to_cnf(n, k, d, step, i_main);
		res = res.substr(0, res.size() - 1) + ",\n" + ans.substr(1, ans.size() - 1);
	} while (std::prev_permutation(v.begin(), v.end()));
	return res;
}

static string make_equi(string newLet, string let1, string let2)// x1 + x2 + x3 >= d
{
	string equi = newLet + " equiv " + let1 + " xor " + let2;
	return equi;
}

//return 1 if for this parameters from a: y equivalent to x1 xor x2
static bool func_y_equi_x1_xor_x2(int a)//y equi x1 xor x2
{
	int y, x1, x2;
	x2 = a % 2;
	a /= 2;
	x1 = a % 2;
	a /= 2;
	y = a % 2;
	return !((x1 + x2 + y) % 2);
}

//adding conditions for new variable that equivalents to xor of 2 already exizting variables
static string adding_new_xor_variable(int newLet, int let1, int let2)//equi(введение новой переменной) работает за константу, не требует remake
{
	string equi = "[";
	for (int line = 1; line <= pow(2, 3); line++)
	{
		if (!func_y_equi_x1_xor_x2(line))
		{
			// \/ //toDo delete copypaste
			stringstream disunkt;
			disunkt << "[";
			int linecopy = line;
			int a = newLet, a_i1 = let1, a1_i2 = let2;
			if (linecopy % 2)
				a1_i2 = -1 * let2;
			linecopy /= 2;
			if ((linecopy % 2))
				a_i1 = -1 * let1;
			linecopy /= 2;
			if ((linecopy % 2))
				a *= -1;
			linecopy /= 2;
			disunkt << a << ", " << a_i1 << ", " << a1_i2;
			disunkt << "]";
			// /\

			if (equi != "[")
				equi += ", ";
			equi += disunkt.str();
		}
	}
	equi += "]";
	return equi;

}

string generate_equi(int n, int k, int d, int step)
{
	std::vector<bool> v(k);
	std::fill(v.begin(), v.begin() + step, true);
	string res = "", ans = "", i_main = "", last = "";
	do {
		i_main = "";
		last = "";
		for (int i = 0; i < k; i++) {
			if (v[i]) {
				i_main += last;
				last = "";
				if (i_main != "")
					last += ".";
				last += to_string(i + 1);
			}
		}
		last = last.substr(1, last.length() - 1);
		for (int j = 1; j <= n - k; j++)
		{
			if (switch_cout_equi)
				cout << make_equi(make_var(step, i_main + "." + last, j), make_var(step - 1, i_main, j), make_var(1, last, j)) << "\n";
			ans = adding_new_xor_variable(ltoN[make_var(step, i_main + "." + last, j)], ltoN[make_var(step - 1, i_main, j)], ltoN[make_var(1, last, j)]);
			res = res.substr(0, res.size() - 1) + ",\n" + ans.substr(1, ans.size() - 1);
		}
	} while (std::prev_permutation(v.begin(), v.end()));
	return res;
}

//check_right_part //toDo correct leftpartchecking
string check_m(int n, int k)
{
	string res = "[]", conjunkt, a;
	int sj;
	for (int i = 1; i < k + 1; i++)
	{
		conjunkt = "[";
		for (int j = 1; j < n + 1; j++)
		{
			cin >> sj;
			if (j <= k)
				a = make_var(0, to_string(i), j);
			else
				a = make_var(1, to_string(i), j - k);
			if ((sj + 1) % 2)
				conjunkt += "-";
			conjunkt += to_string(ltoN[a]) + "], [";
		}
		conjunkt = conjunkt.substr(0, conjunkt.size() - 3);
		//cout << conjunkt << "\n";
		if (res != "[]")
			res = res.substr(0, res.size() - 1) + ",\n]";
		res = res.substr(0, res.size() - 1) + conjunkt + "]";
	}
	return res;
}

//CHECK SAT RESULT


//checks all inequalities created by the generating matrix
bool check_inequivs(string str)
{
	stringstream st(str.substr(1, str.size() - 2));
	string string_var_i = "";
	int left_summ = 0, right_part = 0, flag = 0, var_i;

	answer_from_sat.push_back(0);
	while (getline(st, string_var_i, ','))
	{
		var_i = stoi(string_var_i);
		answer_from_sat.push_back(((var_i / abs(var_i)) + 1) / 2); // -a -> 0 , a -> 1
	}
	for (int i = 0; i < inequivs.size(); i++)
	{
		left_summ = 0; right_part; flag = 0;
		stringstream ineq(inequivs[i]);
		while (getline(ineq, string_var_i, ' '))
		{
			if (string_var_i == "+")
				continue;
			if (string_var_i == ">=")
			{
				flag = 1;
				continue;
			}
			if (flag)
			{
				right_part = stoi(string_var_i);
				continue;
			}
			left_summ += answer_from_sat[ltoN[string_var_i]];
		}
		if (left_summ < right_part)
			return false;
	}
	//all inequivs are correct
	return true;
}

//generate next binary vector and count of one's in it
static pair<vector<int>, int> next_vec(vector<int> vec, int c)
{
	vector<int> b = vec;
	int count = c;
	for (int i = vec.size() - 1; i >= 0; i--)
	{
		if (vec[i] == 0)
		{
			b[i] = 1;
			count++;
			return { b, count };
		}
		else
		{
			b[i] = 0;
			count--;
		}
	}
	return { b, count };
}

//check for all enumerations that they higher or equal Hamming distance(d) // toDo output "+ >=" correct
bool check_enumeration(string str, int n, int k, int d)
{
	vector<int> used_i(k, 0);
	int count = 0, summ = 0;
	pair<vector<int>, int> cur_vec;
	string a_i_j;
	for (int h = 0; h < pow(2, k) - 1; h++)
	{
		cur_vec = next_vec(used_i, count);
		used_i = cur_vec.first;
		count = cur_vec.second;

		a_i_j = "a" + to_string(count) + "_";
		for (int i = 1; i <= k; i++)
			if (used_i[i - 1])
			{
				a_i_j += to_string(i) + ".";
				cout << "1 + ";
			}
			else
				cout << "0 + ";
		
		a_i_j = a_i_j.substr(0, a_i_j.size()-1) + "_";

		cout << "| ";
		summ = 0;
		for (int j = 1; j <= n - k; j++)
		{
			if (answer_from_sat[ltoN[a_i_j + to_string(j)]])
				summ++;
			cout << answer_from_sat[ltoN[a_i_j + to_string(j)]] << " + ";
		}
		cout << "= " << summ + count << " >= " << d << "\n";

		if (summ + count < d)
		{
			return false;
		}
	}
	//all enumerations(except enumerate with all zeros) checked 
	return true;
}


//RESULT


//return result generating matrix
string print_matrix(int n, int k)
{
	string res, a_i_j;
	for (int j = 0; j < k; j++)
	{
		for (int i = 0; i < n; i++)
		{
			if (i < k)
			{
				if (i == j)
					res += "1 ";
				else
					res += "0 ";
			}
			else
			{
				a_i_j = "a1_" + to_string(j + 1) + "_" + to_string(i - k + 1);
				res += to_string(answer_from_sat[ltoN[a_i_j]]) + " ";
			}
		}
		res += "\n";
	}
	return res;
}

//convert string version of res with square brackets to Dimacs format //toDo check in later versions how it works with [], [[1, 2, 3]]
string res_to_Dimacs(string res)
{
	long long counter = 0;
	stringstream ss(" " + res);
	string conjunсt, disjunct, ans = "";
	while (!ss.eof()) {
		getline(ss, conjunсt, ']');
		if (conjunсt != "")
		{
			conjunсt = conjunсt.substr(3, conjunсt.length() - 3);
			stringstream sc(conjunсt);
			while (getline(sc, disjunct, ','))
			{
				ans += disjunct;
			}
			counter++;
			ans += "\n";
		}
	}
	return "p cnf " + to_string(get_vars_count()) + " " + to_string(counter) + "\n" + ans;
}

//print to file
void output_res_to_file(string file_name, string text)
{
	ofstream out;
	out.open(file_name);
	if (out.is_open()) {
		out << text;
	}
	out.close();
}
