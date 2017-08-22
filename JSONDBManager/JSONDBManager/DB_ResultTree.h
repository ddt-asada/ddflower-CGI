#pragma once

#include <iostream>

#include "picojson.h"
#include "mysql_connection.h"

/*
* クラス名:DB_ResultTree
* 概要  :DBの結果セットのツリーのノードクラス
* 作成者:R.Shibata
* 作成日:2016.10.19
*/

//あとで手直し。
class DB_ResultTree {
public:
	DB_ResultTree* parent;		//このノード（インスタンス）の親
	picojson::object json;	//JSONデータの連想配列
	std::string keyData;				//メンバのjsonキー
	sql::ResultSet* db_result;			//DBの結果セット
};