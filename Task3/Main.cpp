#include <iostream>;
#include <fstream>;
#include <string>;
#include <vector>;
#include <map>;

using namespace::std;

int reduction(int n, int k, int d, int want_check_matrix);		// create CNF
bool check_inequivs(string str);								// check minimal distances for all inequivs we have //ToDo (can be removed)
bool check_enumeration(string str, int n, int k, int d);		// check minimal distance all combinations
string print_matrix(int n, int k);								// create result matrix //ToDo rename
void output_res_to_file(string file_name, string text);  // printing results to files

int main()
{
	int n, k, d, want_check_matrix;
	string str;
	ifstream in;
	ofstream out;
	while (cin >> n >> k >> d >> want_check_matrix) {
		//part 1 make CNF
		reduction(n, k, d, want_check_matrix);

		cout << "\nIf results are ready, press Enter\n";
		getline(cin, str);
		getline(cin, str);

		//part 2 get results from SAT
		in.open("sat_result.txt");
		if (in.is_open()) {
			getline(in, str);
		}
		in.close();

		//part 3 check results //ToDo move to another file
		if (check_inequivs(str))
		{
			if (check_enumeration(str, n, k, d))
				cout << "It's really work correct!\n";
			else
				cout << "Oh, my bad, it's not work!(\n";
		}
		else
			cout << "something wrong!!!\n";

		//part 4 create code matrix
		string res = print_matrix(n, k);
		cout << res;

		//part 5 save matrix
		output_res_to_file("../saved_answers/(" + to_string(n) + ", " + to_string(k) + ", " + to_string(d) + ")/code.txt", res);
	}
	return 0;
}