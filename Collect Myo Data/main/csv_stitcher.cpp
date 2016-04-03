#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>

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
		string in_name = "myo_filtered_output" + to_string(i) + ".csv";
		ifstream fin(in_name);

		for (int j = 0; j < 101; j++) {
			string incoming_row;

			/*
			for (int k = 0; k < 14; k++) {
				string newest;
				fin >> newest;
				incoming_row += (newest + " ");
				cout << incoming_row << endl; */

			fin >> incoming_row;
			
			fout << incoming_row << endl;
		}
		fin.close();
	}
	
	fout.close();

	string ender;
	cin >> ender;
	
	return 0;
}
