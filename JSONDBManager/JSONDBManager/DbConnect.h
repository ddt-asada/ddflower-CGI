#pragma once

#include <stdlib.h>
#include <iostream>

#include "Constants.h"

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class DbConnect : public Constants
{
public:
	sql::Connection *con;
	Constants constants;

	DbConnect() {

	};

	/* 関数名:connect
	* 概要:DBとの接続を行う。
	* 引数:無し
	* 戻り値:無し
	* 作成日:2016.10.19
	* 作成者:R.Shibata
	*/
	void connect(){
		sql::Driver *driver;
		sql::Statement *stmt;
/*		sql::ConnectOptionsMap connection_properties;

		sql::SQLString hostName(DSN);
		sql::SQLString userName(DB_USER);
		sql::SQLString password(DB_PASSWORD);
		sql::SQLString charset("utf8");

		connection_properties["hostName"] = hostName;
		connection_properties["userName"] = userName;
		connection_properties["password"] = password;
		connection_properties["OPT_CHARSET_NAME"] = charset;*/

		driver = get_driver_instance();

		//接続を行う。
		con = driver->connect(constants.DSN, constants.DB_USER, constants.DB_PASSWORD);

		//ステートメントを作成。
	//	stmt = con->createStatement();

		//UTF8を設定。
		//stmt->executeQuery("SET NAMES utf8");

		//ステートメントを終了する。
	//	stmt->close();
	}

	/* 関数名:disconnect
	* 概要:DBとの接続を閉じる。
	* 引数:無し
	* 戻り値:無し
	* 作成日:2016.10.19
	* 作成者:R.Shibata
	*/
	void disconnect() {
		
		con->close();
		/*
		//切断失敗のSQLエラーをキャッチするtryブロック
		try {
			//DBとの接続を取じる
			con->close();
			//SQLエラーが発生した場合
		}
		catch (SQLException e) {
			//エラーメッセージを表示する
			e.printStackTrace();
		}*/
	}
};