#include<string>;
#include<sstream>;
#include <map>;
#include "pblib/pb2cnf.h";


//#pragma comment(lib, "pblib.lib")

using namespace std;
using namespace PBLib;

map<string, int> ltoN;
int cur_fresh_var = 1;
vector<string> inequivs;
vector<int> answer_from_sat;
int switch_cout_mknf = 0;
int switch_cout_equi = 0;

int vars_count()
{
	return cur_fresh_var;
}

void set0()
{
	map<string, int> empt_map;
	vector<string> empt_vec;
	cur_fresh_var = 1;
	ltoN = empt_map;
	inequivs = empt_vec;
}

void allLtoN()
{
	cout << "ltoN\n";
	for (auto i : ltoN)
		cout << i.first << " " << i.second << "\n";
}

string make_var(int step, string i, int j)
{
	string a = "a" + to_string(step) + "_" + i + "_" + to_string(j);
	if (ltoN.find(a) == ltoN.end())
	{
		ltoN.insert({ a, cur_fresh_var });
		cur_fresh_var++;
	}

	return a;
}

string first_step_variables(int n, int k)
{
	string res = "";
	for (int i = 1; i <= k; i++)
	{
		for (int j = 1; j <= n - k; j++)
		{
			res += to_string(ltoN[make_var(1, to_string(i), j)]) + " ";
		}
	}
	return res;
}

string check_m(int n, int k)
{
	string res = "[]";
	for (int i = 1; i < k + 1; i++)
	{
		string conjunkt = "[";
		for (int j = 1; j < n + 1; j++)
		{
			int sj;
			cin >> sj;
			string a;
			if (j <= k)
			{
				a = make_var(0, to_string(i), j);
			}
			else
			{
				a = make_var(1, to_string(i), j - k + 1);
			}
			if ((sj + 1)%2)
				conjunkt += "-";
			conjunkt += to_string(ltoN[a]) + "], [";
		}
		conjunkt = conjunkt.substr(0, conjunkt.size() - 3);
		cout << conjunkt << "\n";
		if(res != "[]")
			res = res.substr(0, res.size() - 1) + ", " + conjunkt + "]";
		else 
			res = "[" + conjunkt + "]";
	}
	return res;
}

bool check_inequivs(string str)
{
	vector<int> answer_check(cur_fresh_var + 1, 0);
	stringstream st(str.substr(1, str.size() - 2));
	string vari = "";
	int res = 0;
	int ans = 0;
	int flag = 0;
	int j = 0;
	while (getline(st, vari, ','))
	{
		j++;
		int va = stoi(vari);
		answer_check[j] = (1 + va / abs(va)) / 2;
	}
	answer_from_sat = answer_check;
	for (int i = 0; i < inequivs.size(); i++)
	{
		res = 0; ans = -1; flag = 0;
		stringstream ineq(inequivs[i]);
		while (getline(ineq, vari, ' '))
		{
			//cout << vari << " ";
			if (vari == "+")
				continue;
			if (vari == ">=")
			{
				flag = 1;
				continue;
			}
			if (flag)
			{
				ans = stoi(vari);
				continue;
			}
			res += answer_check[ltoN[vari]];
		}
		//cout << "checker:" << i << " " << res << " " << ans << "\n";
		if (res < ans)
			return false;
	}
	

	return true;
}

pair<vector<int>, int> next_vec(vector<int> a, int c)
{
	vector<int> b = a;
	int count = c;
	for (int i = a.size() - 1; i >= 0; i--)
	{
		if (a[i] == 0)
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

string print_matrix(int n, int k)
{
	string res;
	for (int j = 0; j < k; j++)
	{
		for (int i = 0; i < n; i++)
		{
			if (i < k)
			{
				if (i == j)
				{
					res += "1 ";
				}
				else
				{
					res += "0 ";
				}
			}
			else
			{
				string a = "a1_" + to_string(j + 1) + "_" + to_string(i - k + 1);

				res += to_string(answer_from_sat[ltoN[a]]) + " ";
			}
		}
		res += "\n";
	}
	return res;
}

bool check_enumeration(string str, int n, int k, int d)
{
	vector<int> a(k, 0);
	int count = 0;
	for (int i = 0; i < pow(2, k) - 1; i++)
	{
		pair<vector<int>, int> vec = next_vec(a, count);
		a = vec.first;
		/*for(int j = 0; j < k; j++)
			cout << a[j] << " ";
		cout << "\n";
		*/count = vec.second;// step
		//cout << "count:" << count << "\n";
		string variables = "a" + to_string(count) + "_";
		for (int j = 0; j < k; j++)
			if (a[j])
			{
				variables += to_string(j + 1) + ".";
				cout << "1 + ";
			}
			else 
			{
				cout << "0 + ";
			}
		if (variables[variables.size() - 1] == '.')
			variables = variables.substr(0, variables.size() - 1);
		variables += "_";
		cout << "| ";
		//cout << "vars:" << variables << "\n";
		int summ = 0;
		for (int j = 1; j <= n - k; j++)
		{
			cout << answer_from_sat[ltoN[variables + to_string(j)]] << " + ";
			if (answer_from_sat[ltoN[variables + to_string(j)]])
				summ++;
		}
		cout << "=" << summ + count << ">=" << d << "\n";

		if (summ + count < d)
		{
			return false;
		}
	}


	return true;
}



string func4remake2(int n1, int d, int step, string i_main)// vector< int > literals)//mknf(неравенства на шаге step), n1 = n - k, diff = d - step
{
	PB2CNF pb2cnf;
	vector< int64_t > weights(n1, 1);
	vector< int > literals(n1);
	for (int j = 1; j <= n1; j++)
	{
		string a = make_var(step, i_main, j);
		literals[j - 1] = ltoN[a];
	}

	vector< vector< int> > formula;
	int32_t firstFreshVariable = cur_fresh_var;
	firstFreshVariable = pb2cnf.encodeGeq(weights, literals, d - step, formula, firstFreshVariable) + 1;// literals, d - step
	cur_fresh_var = firstFreshVariable;

	string mknf = "[";

	for (int h = 0; h < formula.size(); h++) //() ^ ()
	{
		stringstream disunkt;
		disunkt << "[";
		if (formula[h].size() > 0)
		{
			for (int j = 0; j < formula[h].size() - 1; j++)//(a v b v c)
			{
				if (formula[h][j] < 0)
					disunkt << "-";
				disunkt << abs(formula[h][j]) << ", ";
			};
			if (formula[h][formula[h].size() - 1] < 0)
				disunkt << "-";
			disunkt << abs(formula[h][formula[h].size() - 1]) << "]";
			if (mknf != "[")
				mknf += ", ";
			mknf += disunkt.str();
		}
		//cout << "\n";
	};
	mknf += "]";
	return mknf;
}

//remake complete
string make_inequality(int jmax, int d, int step, string i)// x1 + x2 + x3 >= d
{
	string inequality = "";
	string a_i_j;
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
//remake complete
string generate_inequalities(int n, int k, int d, int step)// x1 + x2 + x3 >= d
{
	std::vector<bool> v(k);
	std::fill(v.begin(), v.begin() + step, true);
	string res = "";
	string ans = "";
	do {
		string i_main = "";
		for (int i = 0; i < k; ++i) {
			if (v[i]) {
				if (i_main != "")
					i_main += ".";
				i_main += to_string(i + 1);
			}
		}
		inequivs.push_back(make_inequality(n - k, d, step, i_main));
		if (switch_cout_mknf)
			cout << inequivs[inequivs.size() - 1] << "\n";
		ans = func4remake2(n - k, d, step, i_main);
		//cout << ans << "\n";
		res = res.substr(0, res.size() - 1) + ",\n" + ans.substr(1, ans.size() - 1);
	} while (std::prev_permutation(v.begin(), v.end()));
	return res;
}

string make_equi(string newLet, string let1, string let2)// x1 + x2 + x3 >= d
{
	string equi = newLet + " equiv " + let1 + " xor " + let2;
	return equi;
}

bool func2(long long a)//y equi x1 xor x2
{
	int y, x1, x2;
	x2 = a % 2;
	a /= 2;
	x1 = a % 2;
	a /= 2;
	y = a % 2;
	// !(y xor (x1 xor x2))
	return !((x1 + x2 + y) % 2);
}

string func5remake(int newLet, int let1, int let2)//equi(введение новой переменной) работает за константу, не требует remake
{
	string equi = "[";
	for (int line = 1; line <= pow(2, 3); line++)
	{
		if (!func2(line))
		{
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
	string res = "";
	string ans = "";
	do {
		string i_main = "";
		string last = "";
		for (int i = 0; i < k; ++i) {
			if (v[i]) {
				i_main += last;
				last = "";
				if (i_main != "")
					last += ".";
				last += to_string(i + 1);
			}
				
		}
		//cout << i_main << "," << last << "\n";
		last = last.substr(1, last.length() - 1);
		for (int j = 1; j <= n - k; j++)
		{
			if (switch_cout_equi)
				cout << make_equi(make_var(step, i_main + "." + last, j), make_var(step - 1, i_main, j), make_var(1, last, j)) << "\n";
			ans = func5remake(ltoN[make_var(step, i_main + "." + last, j)], ltoN[make_var(step - 1, i_main, j)], ltoN[make_var(1, last, j)]);
			res = res.substr(0, res.size() - 1) + ",\n" + ans.substr(1, ans.size() - 1);
		}
	} while (std::prev_permutation(v.begin(), v.end()));
	return res;
}

