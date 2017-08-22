#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <stdlib.h>

#include "mysql_connection.h"

#include "picojson.h"
#include "DB_ResultTree.h" //自作クラス。メンバ変数のみ持つ。
#include "Constants.h"		//自作クラス。様々な定数を持つ。
#include "DbConnect.h"		//自作クラス。DB接続の関数を持つ。
#include "JSONDBManager.h"
#include "CgiCookieManager.h"
#include "CgiSaveDataManager.h"
#include "CgiSessionManager.h"
#include "CgiSession.h"

class account : public JSONDBManager {
public:
	picojson::object pageAuthorityCheck;
	
	//クッキー操作用のオブジェクト
	//cookieを操作する動作はすべてここに投げる。
	   nana::CgiCookieManager accountCookie;
	   //セッション操作用のオブジェクト
	   //sessionを作る動作はすべてここに投げる。
	   nana::CgiSession accuntSession;

	   //定数クラス。
	   Constants constants;

	   account() {

	   };

	   /* クラス名:init
	   * 概要:初期化処理を行う。初期化としてセッションの開始とDBへの接続を行う。
	   * 引数:無し
	   * 戻り値:無し
	   * 作成日:2016.10.20
	   * 作成者:R.Shibata
	   */
	   void init(){
		   
		   //クッキーマネージャのインスタンスを作成する
		   //セッションが存在しているかをチェックしてなければ作る。
		   if (accountCookie.getCookieValue("JSESSION") == "") {
			   accuntSession.load();
			   accountCookie.setCookie("JSESSION", accuntSession.getId(), constants.COOKIE_EXPIRATION);
		   }
		   //DBへの接続を開始する
		   connect();
	   }


	   /* クラス名:login
	   * 概要:ログイン処理を行う。
	   * 引数:String jsonString: JSON文字列。ログイン情報が入っている必要がある。
	   * 戻り値:無し
	   * 作成日:2016.10.20
	   * 作成者:R.Shibata
	   */
	   void login(std::string jsonString) {
		   //クライアントから送信されたJSON文字列を取得する
		   getJSONMap(jsonString);

		   //SQLによる例外対処のためrtyブロックで囲む
		   //jsonを出力する
		   try {
			   //jsonを出力する
			   createJSON(json, constants.EMPTY_STRING, NULL);
			   //SQL例外のchatchブロック
		   }
		   catch (sql:: SQLException &e) {
			   // エラーメッセージを表示する
			 //  e.printStackTrace();//systemエラーメッセージを出力する
		   }

		   //連想配列をjsonに変換して変数に入れる
		   std::string jsonOut = Encode(json);
/*		   //セッションIDを発行する
			accountSession.load();

		   //発行したsessionIDを返却用のcookieにセットする。
		   //
		   accountCookie.setCookie("JSESSIONID", accuntSession.getId(), constants.COOKIE_EXPIRATION);*/

		   //JSONから会員番号を取り出す
		   std::string userId = getJsonValue(json, constants.STR_ID, constants.KEY_TEXT);
		   //JSONから権限を取り出す
		   std::string authority = getJsonValue(json, constants.AUTHORITY, constants.KEY_TEXT);

		   //cookieにユーザIDをセットする
		   accountCookie.setCookie(constants.USER_ID, userId, constants.COOKIE_EXPIRATION);

		   //cookieにユーザの権限をセットする
		   accountCookie.setCookie(constants.AUTHORITY, authority, constants.COOKIE_EXPIRATION);

		   //強制的にteacherでログイン。
		   //std::string jsonOut = "{ \"db_getQuery\":\"SELECT id, login_id, authority FROM user_inf WHERE login_id='userName' AND password='password';\",\"userName\" : {\"value\":\"teacher\"},\"password\" : {\"value\":\"4a82cb6db537ef6c5b53d144854e146de79502e8\"},\"id\" : {\"text\":\"3\"},\"authority\" : {\"text\":\"10\"} }";

		   //作成したJSON文字列を出力用文字列へセットする
		   setOutHTMLString(jsonOut);
	   }

	   /* クラス名:logout
	   * 概要:ログアウト処理を行う。
	   * 引数:無し
	   * 戻り値:無し
	   * 作成日:2016.10.20
	   * 作成者:R.Shibata
	   */
	   void logout() {
		   
		   //セッションクッキーが存在するなら
		   if (accountCookie.getCookieValue(constants.JSP_SESSION_COOKIE_NAME) != constants.EMPTY_STRING) {
			   //sessionファイルを削除する命令を入れる予定。
			   //セッションクッキーを破棄する
			   accountCookie.deleteCookie(constants.JSP_SESSION_COOKIE_NAME);
		   }

		   //ユーザID、権限のcookieを削除する
		   accountCookie.deleteCookie(constants.JSP_SESSION_COOKIE_NAME);
		   accountCookie.deleteCookie(constants.USER_ID);
		   accountCookie.deleteCookie(constants.AUTHORITY);
	   }

	   /* クラス名:loginCheck
	   * 概要:ログインチェックを行う。
	   *    :チェック失敗時、LoginCheckExceptionをthrowする
	   * 引数:無し
	   * 戻り値:boolean:ログインしているか否かの真理値を返す
	   * 作成日:2016.10.20
	   * 作成者:R.Shibata
	   */
	   bool loginCheck(){
		   bool retBoo = false; //返却値を格納する変数を宣言する

								   //セッション変数のユーザIDを参照し、値が存在するかどうかをチェックする
								   //また、セッションとcookieに保存されているユーザIDが一致するかをたしかめる
	   if (accountCookie.getCookieValue("JSESSION") != "" && accountCookie.getCookieValue(constants.USER_ID) != ""
		   /*&& session.getSessionValue(Constants.USER_ID).equals(cookie.getCookieValue(Constants.USER_ID))*/) {
		   //セッションの有効時間を延長する
		   //ここたぶんいらない。
		   //session.extension();
		   //返却値の変数にtrueを格納する
		   retBoo = true;
		   //ページに対する権限チェックを行う(権限所持ユーザー)
		   pageCheck(accountCookie.getCookieValue(constants.AUTHORITY));
	   }
	   else {
		   //ページに対する権限チェックを行う（ゲストユーザー）
		   pageCheck("1");
	   }
	   //真理値を返す
	   return retBoo;
	   }

	   /* クラス名:pageCheck
	   * 概要:ページの権限チェックを行う
	   *    :チェック失敗時、LoginCheckExceptionをthrowする
	   * 引数:String strAuthority:ユーザーが所持する権限を示す文字列(内容は数値）
	   * 戻り値:無し
	   * 作成日:2016.10.20
	   * 作成者:R.Shibata
	   */
	   void pageCheck(std::string strAuthority){
		   //権限比較用の変数を宣言し、取得したユーザ権限をセットする
		   int authority = stoi(strAuthority);

			//cookieにセットされている対象ページの権限を取得する(16進数)
			//入力は0xFF等の文字列のため、parseIntは使用できない。decodeの返り値はInteger型のため、intValueでintに変換する
		   int pageAuth = std::stoi(accountCookie.getCookieValue(constants.PAGE_AUTH), nullptr, 16);

			//当該ユーザで開けるページなのか検証する
			if ((authority & pageAuth) == 0) {
				std::string outValue = "0";
				if (accountCookie.getCookieValue(constants.USER_ID) != "" && accountCookie.getCookieValue("JSESSIONID") == "") {
					//タイムアウトとして、１をセットする。
					outValue = "1";
				}

				std::cout <<"\n"<< constants.ERROR_JSON_FRONT + outValue + constants.ERROR_JSON_BACK;
				exit(0);
			}
	   }
};