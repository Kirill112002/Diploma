#include <iostream>;
#include <fstream>;
#include <string>;
#include <vector>;
#include <map>;

using namespace::std;

int reduction(int n, int k, int d, int want_check_matrix);
void allLtoN();
string print_matrix(int n, int k);
bool check_inequivs(string str);
bool check_enumeration(string str, int n, int k, int d);
//string generate_inequalities(int n, int k, int d, int step);// , map<string, int> ltoN);

int main()
{
	int n, k, d, want_check_matrix;
	while (cin >> n >> k >> d >> want_check_matrix) {
		
		//generate_inequalities(7, 4, 3, 3);
		
		reduction(n, k, d, want_check_matrix);

		//allLtoN();
		cout << "\nif results ready press enter:\n";
		string str;
		getline(cin, str);
		getline(cin, str);
		ifstream in;  // поток для записи
		in.open("sat_result.txt");  // открываем файл для записи
		if (in.is_open()) {
			getline(in, str);
		}
		in.close();
		if (check_inequivs(str))
		{
			if (check_enumeration(str, n, k, d))
				cout << "It's really work correct!";
			else
				cout << "Oh, my bad, it's not work!(";
		}
		else
			cout << "something wrong!!!";
		cout << "\n";
		string res = print_matrix(n, k);
		cout << res;
		ofstream out;  // поток для записи
		string file_dir = "../saved_answers/(" + to_string(n) + ", " + to_string(k) + ", " + to_string(d) + ")";
		out.open(file_dir + "/code.txt");  // открываем файл для записи
		if (out.is_open()) {
			out << res;
		}
		out.close();
	}
	return 0;
}