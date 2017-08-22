#pragma once

#include "boost/io/ios_state.hpp"

#include "CgiCookie.h"
#include "JSONDBManager.h"

namespace nana{


using namespace boost::io;

class CgiCookieManager {
public:
		SaveDataMap cmap;
		std::map<std::string, CgiCookie> retCookie;		//返却用のマップ。
		JSONDBManager test;

		//デフォルトコンストラクタ。マップにcookieをセットして使えるようにする。
		CgiCookieManager() {
			char* env = getenv("HTTP_COOKIE");
			string str = (env == NULL ? "" : env);

			//クッキーマップをセットして使えるようにする。 
			loadCookie(cmap, str);
		}

		//コンストラクタ。セットするcookieが投げられていてたらメンバのcookieに追加する。
		CgiCookieManager(std::string name, std::string value, int expir) {
			//メンバのcookieにnameとvalueをセットにして追加する。
			cmap.insert(make_pair(name, value));
			//メンバのcookieに生存時間をセットする。
			cmap.insert(make_pair("expir", std::to_string(expir)));
			//メンバのパスに"/"をセットする。
			cmap.insert(make_pair("path", "/"));
		}

		//キーに対応した値を探して返却する。
		std::string getCookieValue(std::string key) {

			//返却用の文字列。
			std::string cookieValue = "";

			//キーに対応した値を取得する。ない場合はNULLになる。
			cookieValue = cmap[key];

			//値を返却する。

			return cookieValue;
		}

		//セットするcookieが投げられていてたらメンバのcookieに追加する。
		void setCookie(std::string name, std::string value, int expir) {
			//セットするためのクッキーを作成する
			CgiCookie cookie(name, value);
			//生存時間を加算する。
			cookie.addMaxAge(expir);
			//クッキーにアクセス可能なURLパスを指定する
			cookie.setPath("/");

			//セットするcookieを出力する。
			cout << cookie << "\n";
			//responseにクッキーを追加する(生存時間0のクッキーで上書きし削除する)
			//retCookie.insert(make_pair(name, cookie));
		}

		//cookieを削除する。
		void deleteCookie(std::string key) {
			//セットするためのクッキーを作成する
			CgiCookie cookie(key, "");
			//クッキーの生存時間を0指定して削除する
			cookie.setMaxAge(-1);
			//クッキーにアクセス可能なURLパスを指定する
			cookie.setPath("/");
			//responseにクッキーを追加する(生存時間0のクッキーで上書きし削除する)
			//retCookie.insert(make_pair(key, cookie));
			cout << cookie << "\n";
		}

		//sessionがあるかをチェックする。あとでsessionManager作るかも。
		bool checkSession() {
			
		}
};
/*
void CgiCookie::setExpires(const int& year, const int& month, const int& day,
	const int& hour, const int& min, const int& sec)
{
	tm st;
	st.tm_sec = sec;      // 秒(0～61) 
	st.tm_min = min;      // 分(0～59) 
	st.tm_hour = hour;     // 時(0～23) 
	st.tm_mday = day;     // 日(1～31) 
	st.tm_mon = month-1;      // 月(1月を0とする) 
	st.tm_year = year-1900;     // 年(1900年を0とする) 
	m_expires = mktime(&st);
};
*/
	

//01-Jan-1970 00:00:00 GMT形式
const string CgiCookie::createExpires() const
{
	if(maxAge == 0) return "";
	//
	tm ti = *gmtime(&maxAge);
	char str[30];
	strftime(str, 30, "%a, %d-%b-%Y %H:%M:%S GMT", &ti);
	return string(str);
};


ostream& operator << (ostream& os, const CgiCookie& cookie)
{
	ios_flags_saver ifs(os);//現状のストリームの書式などを保存
	typedef pair<const string, CgiSaveData> KeyPair;
	
	//出力する 
	os << "Set-Cookie: ";
	os << cookie.name + '=' + cookie.value + ';';
	const string expires = cookie.createExpires();
	if(!expires.empty()) os << " expires=" << expires << ";";
	if(!cookie.domain.empty()) os << " domain=" << cookie.domain << ";";
	if(!cookie.path.empty()) os << " path=" << cookie.path << ";";
	if(cookie.isSecure) os << " secure";
	
	return os;
};


void loadCookie(SaveDataMap& smap, const string& cookieStr)
{
	istringstream iss(cookieStr);
	loadData(smap, iss);
};


}; //namespace nana{

