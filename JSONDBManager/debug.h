#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

using namespace std;

void fileout(string str) {
	string filename = "debug";
	ofstream ofs;
	ofs.open(filename, ios::app);
	ofs << str << endl;
	ofs.close();
}