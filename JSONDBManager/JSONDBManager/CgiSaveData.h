#pragma once

#include <cstdlib>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>

namespace nana{

using namespace std;


/**
保存用のデータクラス。
stringのpairの入出力演算子の定義を作りたくなかったので、派生しました。
*/
class CgiSaveData : public string
{
public:
	CgiSaveData()
	 : string()
	{};

	CgiSaveData(const string& str)
	 : string(str)
	{};

	CgiSaveData& operator =(const char* str)
	{
		return (*this = CgiSaveData(str));
	};
};

//
typedef map<string, CgiSaveData> SaveDataMap;
//
ostream& operator <<(ostream& os, const pair<const string, CgiSaveData>& data);
//
istream& operator >>(istream& is, pair<string, CgiSaveData>& data);
//
bool saveData(const SaveDataMap& smap, ostream& file);
//
bool loadData(SaveDataMap& smap, istream& is);


}; //namespace nana 終了