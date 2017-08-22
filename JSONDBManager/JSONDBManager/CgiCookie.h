#pragma once


#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <direct.h>

#include "CgiSaveData.h"

namespace nana{

using namespace std;


/**
Data1=Data2%3Derror;Data2=xyz
NAME=値; expires=値; domain=値; path=値; secure
http://www.tohoho-web.com/wwwcook.htm
Cookieがデータを記録できるのは4096バイトまでで、最大で300まで保存することが可能です
*/
class CgiCookie
{
public:
	string domain;
	string path;
	bool isSecure;
	string name;
	string value;
	time_t maxAge;
	
	//デフォルトコンストラクタ。使わない。
	CgiCookie(){
	};
	
	//cookieを作成するコンストラクタ。
	CgiCookie(const string& name, const string& value)
	 : name(name), value(value), isSecure(false), maxAge(0), path("/")
	{
		//現在時刻を設定るする。
		maxAge = time(NULL);
	};

	//生存時間を設定する。。
	void setMaxAge(int expir)
	{
		this->maxAge = expir;
	}

	//生存時間を加算する。。
	void addMaxAge(int expir)
	{
		this->maxAge += expir;
	}

	//パスを設定する。
	void setPath(std::string path) {
		this->path = path;
	}

	///01-Jan-1970 00:00:00 GMT形式
	const string createExpires() const;
};


///クッキークラスの出力オーバーロード
ostream& operator << (ostream& os, const CgiCookie& cookie);

///クッキーデータのクッキーストリングからのロード
void loadCookie(SaveDataMap& smap, const string& cookieStr);


}; //namespace nana{

