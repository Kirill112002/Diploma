#include <iostream>;
#include <fstream>;
#include <string>;

using namespace::std;

int reduction(int n, int k, int d, int want_check_matrix);		// create CNF
bool check_inequivs(string str);								// check minimal distances for all inequivs we have //ToDo (can be removed)
bool check_enumeration(string str, int n, int k, int d);		// check minimal distance all combinations
string print_matrix(int n, int k);								// create result matrix //ToDo rename
void output_res_to_file(string file_name, string text);			// printing results to files
int get_vars_count();

bool check_input(int n, int k, int d)
{
	if (n < k || n < d || n == 0 || k == 0)
	{
		cout << "incorrect input\n";
		return false;
	}
	if (n == k)
	{
		if (d <= 1)
			cout << "it's Identity matrix of size n\n";
		else
			cout << "it's Identity matrix of size n. Current d  cannot be bigger than 1\n";
		return false;
	}
	return true;
}
void prepare_file()
{
	ofstream out;
	out.open("sat_result.txt");
	if (out.is_open()) {
		out << "WAITING SAT";
	}
	out.close();
}
int main()
{
	int n, k, d, mode;// mode 0 -- без модификаторов, mode 1 -- продолжение прошлого запроса(d увеличилось), mode 2 -- проверка существующего решения(toDo)
	string str;
	ifstream in;
	ofstream out;
	while (cin >> n >> k >> d)
	{
		//} >> mode) {
		if (check_input(n, k, d))
		{
			//prepare_file();
			//part 1 make CNF
			reduction(n, k, d, 0);// , mode);
			cout << "\nIf results are ready, press Enter\n";
			getline(cin, str);

			read_sat_res:
			getline(cin, str);

			//part 2 get results from SAT
			in.open("sat_result.txt");
			if (in.is_open()) {
				getline(in, str);
			}
			in.close();
			if (str == "WAITING SAT")
			{
				cout << "results aren't ready\n";
				goto read_sat_res;
			}
			bool flag = false;
			//part 3 check results //ToDo move to another file
			if (str != "UNSAT" && check_inequivs(str))
			{
				if (check_enumeration(str, n, k, d))
				{
					cout << "It's really work correct!\n";
					flag = true;
				}
				else
					cout << "Oh, my bad, it's not work!(\n";

			}
			else
				cout << "something wrong!!!\n";

			//remove after test
			string res = print_matrix(n, k);
			cout << res;
			
			if (flag)
			{
				//part 4 create code matrix
				string res = print_matrix(n, k);
				cout << res;

				//part 5 save matrix
				output_res_to_file("../saved_answers/(" + to_string(n) + ", " + to_string(k) + ", " + to_string(d) + ")/code.txt", res);
				cout << get_vars_count();
			}
		}
	}
	return 0;
}