#include "procedureGetList.h"
#include "Request.h"
#include "JSONtest.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <direct.h>

#include "boost/io/ios_state.hpp"

#include "CgiSaveDataManager.h"
#include "CgiCookieManager.h"
#include "CgiSessionManager.h"


/*
* ファイル名:jsp/GetJSONArray.jsp
* 概要	:テーブルのタグ作成用のJSON配列を作成して返す。
* 作成者:R.Shibata
* 作成日:2016.10.24
*/


int main() {
	Constants constants;
	//テスト用
	JSONDBManager test;

	//std::string json = testString();
	
	const char* answer = "Content-Type:text/html;charset=utf-8;\n";

	std::cout << answer;
	
	//HTTPよりPOSTメソッドでデータを受け取るクラスのインスタンス化。
	ServerRequest request;
	
	//HTTPよりPOSTメソッドで送られたデータを受け取る。
	std::string json = request.HTTPRequest();
	
	//クライアントへ返却するための文字列を宣言する
	std::string outValue = "";
	//リスト形式のJSONを作るクラスのインスタンスを作成する
	procedureGetList listJsonDbGetter;
	
	//生成したインスタンスの処理関数を実行する
	listJsonDbGetter.run(json);
	//作成した文字列を表示しクライアントへ返却する
	outValue = listJsonDbGetter.getOutHTMLString();

	//作成した文字列を表示しクライアントへ返却する
	std::cout <<'\n' << outValue;
	
	
	return 0;
}