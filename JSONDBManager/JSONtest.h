#pragma once

#include <iostream>
#include <fstream>
#include "picojson.h"

	std::string testString() {

		stringstream ss;
		ifstream     f;
		unsigned int i;

		// Read Json file
		f.open("Text.txt", ios::binary);
		if (!f.is_open()) {
			return "1";
		}
		ss << f.rdbuf();
		f.close();

		// Parse Json data
		picojson::value v;
		ss >> v;
		string err = picojson::get_last_error();
		if (!err.empty()) {
			cerr << err << endl;
			return "-1";
		}

		return v.serialize();
	}