#pragma once

#include "boost/io/ios_state.hpp"

#include "CgiSaveData.h"




namespace nana{
	
using namespace std;
using namespace boost;
using namespace boost::io;


namespace{
	
///文字を数字に変換する 
inline const unsigned int toUInt(const char& c)
{
	return (unsigned int)*(unsigned char*)&c;
};


///文字列の空白を除去する。 
inline const string deleteSpace(string ret)
{
	string::iterator i = remove(ret.begin(), ret.end(), ' ');
	ret.erase(i, ret.end());
	return ret;
};

///16進数文字列を文字列に戻す 
const string toString(const string& str)
{
	string ret;
	char tmp[3];
	char* endstr;
	istringstream is(str);
	
	//2文字ずつ読んでいく。get()は最後にNULLが入る。
	while(is.get(tmp, 3)){
		unsigned char c = static_cast<unsigned char>(strtol(tmp, &endstr, 16));
		ret += *(char*)&c;
	}
	 
	return ret;
}

}; //namespace無名終了 



/**
出力のために演算子定義 
pairのキー名には英数字のみ使用可能。空白文字や全角文字などを使用しないこと。 
*/
ostream& operator <<(ostream& os, const pair<const string, CgiSaveData>& data)
{
	ios_flags_saver ifs(os);//現状のストリームの書式などを保存
	
	//キー名の出力 
	os << hex << uppercase << setfill('0');
	os << data.first << "=";
	
	//エスケープしながら出力する 
	string::const_iterator i;
	for(i=data.second.begin(); i!=data.second.end(); ++i){
		os << setw(2) << *i;
	}
	os << ";";
	
	return os;
};



/**
入力のために演算子定義 。pair１つを取り出す 
*/
istream& operator >>(istream& is, pair<string, CgiSaveData>& data)
{
	stringbuf buf;
	
	//キーを取得 
	is.get(buf, '=');
	data.first = deleteSpace(buf.str());
	if(is.eof()) return is;
	//=を読み飛ばす 
	is.get();
	
	//値を取得 
	buf.str("");
	is.get(buf, ';');
	data.second = deleteSpace(buf.str());
	//;を読み飛ばす
	if(!is.eof()) is.get();
	
	return is;
};


typedef pair<string, CgiSaveData> SaveDataPair;


/**
データをファイルに保存する。 
@return ofstream.fail()
*/
bool saveData(const SaveDataMap& smap, ostream& os)
{
	ostream_iterator<SaveDataPair> itrOut(os);
	
	//イテレータでコピーしていく 
	copy(smap.begin(), smap.end(), itrOut);
	
	return !os;
};
 

/**
データをファイルから読み込む。
@return ifstream.fail()
*/
bool loadData(SaveDataMap& smap, istream& is)
{
	typedef istream_iterator<SaveDataPair> SDIterator;
	SDIterator itrIn(is), iend;
	
	//イテレータで挿入していく 
	for(; itrIn !=iend; ++itrIn) smap.insert(*itrIn);
	
	return !is;
};

};//namespace nana終了 


