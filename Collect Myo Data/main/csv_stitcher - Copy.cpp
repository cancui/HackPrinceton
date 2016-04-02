#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>

using namespace std;

int main() {
	string new_file_name;
	cout << "What will be the name of the new file?" << endl;
	cin >> new_file_name;
	
	string final_name = new_file_name + ".csv";
	
	int first;
	cout << "What is the number of the first file to stitch?" << endl;
	cin >> first;
	
	int last;
	cout << "What is the number of the last file to stitch?" << endl;
	cin >> last;
	
	ofstream fout(final_name);
	
	for (int i = first; i <= last; i++) {
		string in_name = "myo_output" + static_cast<string>(i) + ".csv";
		ifstream fin(in_name);
		string test;
		cout << (fin >> test);
		fin.close();
	}
	
	fout.close();
	
	return 0;
}
