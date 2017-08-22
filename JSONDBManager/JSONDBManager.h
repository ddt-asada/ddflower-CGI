	/* 関数名：createJSON
	 * 概要:DBからデータを取得してJSONを作る
	 * 引数:Map<String,Object> json:カレントのJSON
	 *     String:key:JSONのキー
	 *     DBResultTree:dbrt_parent:DBから取得したデータを格納してツリー構造を作るためのクラスのインスタンス
	 * 戻り値:無し
	 * 作成日:2016.10.19
	 * 作成者:R.Shibata
	 */
#pragma once

#include <iostream>
#include <fstream>//テスト用
#include <string>
#include <vector>
#include <sstream>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <boost/uuid/sha1.hpp>
#include <boost/cstdint.hpp>
#include <boost/array.hpp>

#include "mysql_connection.h"

#include "picojson.h"
#include "DB_ResultTree.h" //自作クラス。メンバ変数のみ持つ。
#include "Constants.h"		//自作クラス。様々な定数を持つ。
#include "DbConnect.h"		//自作クラス。DB接続の関数を持つ。

//boostのSHA1を利用するための準備。
typedef boost::array<boost::uint8_t, 20> hash_data_t;

class JSONDBManager : public DbConnect {
public:int processedRecords = 0;
	   //JSONを返還した連想配列を格納するメンバ変数
public:picojson::object json;
	   //HTML上に出力するための作成した文字列
private:std::string outHTMLString = "";
		//作成したcookieを格納する文字列。
		std::string outCookieString = "";

public:

	JSONDBManager() {

	};
	//エラー処理を後で追記。
	void createJSON(picojson::object& json, std::string key, DB_ResultTree* dbrt_parent) {

		std::string keyString;		//JSONのキー値を格納する文字列。
		// DBの結果から構築したツリーを構成するクラスのインスタンスを作成する
		//4つのフィールドを持つクラス。あとで作る。
		DB_ResultTree db_resultTree;
		//ステートメントを作成する
		//DBから結果セットを取得する関数。    //あとで変更。
		db_resultTree.db_result = executeQuery(json, constants.DB_GETQUERY);
		//DB_ResultTreeの親子関係を構築する
		db_resultTree.parent = dbrt_parent;
		//カレントのJSONを保存する
		db_resultTree.json = json;
		//カレントのキーを保存する
		db_resultTree.keyData = key;
		// db_resultTreeから"key"に該当するデータを取得する
		std::string column = getDBColumn(key, &db_resultTree);

		//jsonについて最下層の要素にたどり着くまでループしてデータを書き換える。
		//テスト完了
		for (picojson::object::iterator itr = json.begin(); itr != json.end(); ++itr) {
			//jsonのキー値を取り出す。
			keyString = itr->first;
			//JSONに子供があるとき。
			//あとで定数に置き換える。
			if (itr->second.to_str() == "object")
			{
				//再帰処理
				createJSON(itr->second.get<picojson::object>(), keyString, &db_resultTree);
			}
			//取り出しておいたキーが"text"または"html"または"src"であれば。
			//あとで定数に置き換え。
			else if (column != "" && (keyString == "text" || keyString == "html" || keyString == "src"))
			{
				//上書き
				json[keyString] = (picojson::value)column;
			}
		}
	}

	/* 関数名：executeQuery
	* 概要:クエリを実行してDBから結果セットを取得する
	* 引数:Map<String,Object> json:カレントのJSON連想配列
	*     String:queryKey:実行するクエリのベースとなる文字列
	* 戻り値:ResultSet retRs:DBから取得した結果セットを返す
	* 作成日:2016.10.19
	* 作成者:R.Shibata
	* 更新日:2017.8.15
	* 更新者:K.Asada
	* 更新内容:C++に対応するように引数などjava特有のものを変更。
	*/

	//あとで例外処理を追記する。
	//テスト完了。
	//パス関係のみ後程追記予定。
	sql::ResultSet* executeQuery(picojson::object& json, std::string queryKey) {
		//返却する結果セットの変数を作成する
		int tmp = 0;
		std::string query;			//キー名に対応した値を格納する文字列。
		std::string replaceValue;	//置換対象の文字列を格納する文字列。
		std::string passString;		//パスワードの紐づいた文字列を格納する文字列。
		picojson::object obj;		//クエリを置換する際の仮の置き場。
		sql::Statement *stmt;		//ステートメントを格納する。
		sql::ResultSet *res = NULL;		//Connecter/C++で使う。

		//ユーザ情報を保護するためにパスワードが存在しているかを判定する。
		auto itr = json.find(constants.STR_PASSWORD);
		//値が取得できていたら。
		if (itr != json.end()) {
			obj = itr->second.get<picojson::object>();
			//パスワードに当たる文字列をハッシュ化し、jsonの値を置換する（仮置換）
			ChangeJSON(json, constants.STR_PASSWORD, changeToSHA1(obj["value"].get<std::string>()));
			//ここまでで暗号化できてる念のため残してます。
		}

		// queryKeyに対応する値が取得する。
		auto it = json.find(queryKey);
		//queryが正しく取得できていれば。
		if (it != json.end()) {
			query = CatchJSON(json, queryKey);
			//queryが正しく取得できていれば。
			if (query.length() >= 1) {
				//jsonについて最下層の要素にたどり着くまでループしてデータを取り出す
				for (picojson::object::iterator itr = json.begin(); itr != json.end(); ++itr) {
					//valueに子供があるときの処理(linkedHashMapの時)
					if (itr->second.to_str() == "object") {
						//子オブジェクトを取得する
						obj = itr->second.get<picojson::object>();
						//子オブジェクトがvalueを持っていたら
						if (obj["value"].is<std::string>() || obj["value"].is<picojson::array>()) {
							//置換対象の文字列を、取得した子オブジェクトのvalueより作成する
							replaceValue = createReplaceValue(obj);
							//子オブジェクトのkey文字列と一致するqueryの文字列を置換する
							//あとで関数作る。
							std::string findKey = itr->first;
							//置換対象の文字列を取得できていたら。
							if (query.find("'" + findKey + "'") != std::string::npos) {
								//置換対象のキー名を取得。
								query.replace(query.find("'" + findKey + "'") + 1, findKey.length(), replaceValue);
							}
						}
					}
				}
				try {
					//実行できないパターンのクエリを修正する
					query = queryCorrection(query);
					fileout(query);
					//ステートメントを作成する
					stmt = con->createStatement();
					
					//クエリを実行し結果セットを返すクエリであれば。
					if (stmt->execute(query)) {
						res = stmt->getResultSet();
						//行数を取得するため、レコードセットの位置をlastにする
						res->last();
						//処理を行ったレコード数を結果セットより取得してメンバに保存する
						processedRecords = res->getRow();
						//行数を取得するためにlastにしたカーソル位置を先頭に戻す。
						res->beforeFirst();
					}
					else {
						//結果を返さない場合は更新した行数を取得する。
						processedRecords = stmt->getUpdateCount();
					}
				}
				catch (sql::SQLException &e) {
					std::cout << "# ERR: SQLException in " << __FILE__;
					std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
					std::cout << "# ERR: " << e.what();
					std::cout << " (MySQL error code: " << e.getErrorCode();
					std::cout << ", SQLState: " << e.getSQLState() << " )" << endl;
					
				}
			}
		}
		//結果セットを返す
		return res;
	}

	/* 関数名：createReplaceValue
	* 概要:クライアントより受け取った置換対象の値を、置換可能文字列に変換する
	* 引数:Object childObjectValue:クライアントより受け取った置換対象の値、StringとArrayListが存在する
	* 戻り値:String:作成した返却用の文字列
	* 作成日:2016.10.28
	* 作成者:R.Shibata
	*/

	//のちほど調整を行う。
	std::string createReplaceValue(picojson::object childObjectValue) {
		//受け取ったオブジェクトにより、返却する文字列を作成するための変数を宣言する
		std::string retReplaceString = "";
		//走査用の文字列の格納場所を生成。
		std::string childObjectstring = "";
		
		int i = 0;
		//データ作成のための文字列配列を用意する
		std::string childObjectArray[10] = { "" };
		//picojsonのメンバ関数を利用できるように一度オブジェクトを変換。
		//配列から文字列を取り出す。
		if (childObjectValue["value"].is<picojson::array>()) {
			//picojsonの配列に変換。
			picojson::array&tmp = childObjectValue["value"].get<picojson::array>();
			//走査用文字列配列に全て代入する
			for (picojson::array::iterator itr = tmp.begin(); itr != tmp.end(); itr++) {
				//文字列を配列に格納していく。
				childObjectArray[i++] = itr->get<std::string>();
			}
			//配列以外であれば
		}
		else {
			//走査用文字列配列に文字列として取得
			childObjectArray[0] = (childObjectValue["value"].get<std::string>());
		}
		//取得、作成した配列を走査する
		for (i = 0; childObjectArray[i] != ""; i++) {
			//走査用の文字列配列から文字列を取得。
			childObjectstring = childObjectArray[i];
			//置換文字列が空白であれば何もしない、値があれば区切り文字を付与する
			retReplaceString += retReplaceString == "" ? "" : "','";
			//配列の文字列を、エスケープ処理を行い置換文字列に付与する
			retReplaceString += escapeSQLValue(childObjectstring);
		}
		//作成した文字列を返却する
		return retReplaceString;
	}

	/* 関数名：getDBColumn
	* 概要:指定したkey(列)の値を結果セットから取得して返す
	* 引数:String key:JSONのオブジェクトのキー
	*     DBResultTree dbrTree:DBから取得した結果をツリー構造にするクラスのインスタンス
	* 戻り値:String column:取得した列の値を返す
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/

	//例外処理を後で追加。
	std::string getDBColumn(std::string key, DB_ResultTree *dbrTree) {
		//返却値を格納する変数を初期化する
		std::string column = "";
		//取得対象が列の何行目かをセットする
		int columnNumber = 0;
		//dbrTreeの親のキーが、これが配列の要素であるという事を示す文字を含んでいれば
		//あとで定数に置き換える。
		if (dbrTree->parent != NULL && dbrTree->parent->keyData.find("__") != -1) {
			//keyの値を分割する
			//あとで定数に置き換える。
			std::string keyString[] = { (dbrTree->parent->keyData, "__") };
			//行数をセットする
			columnNumber = std::stoi(keyString[1]);
		}

		//親が無くなるまでDBレコードツリーを走査する
		while (dbrTree != NULL) {
			//dbtTreeに結果セットが登録されていれば
			if (checkColumn(dbrTree->db_result, key)) {
				//colNumberの位置へレコードセットを移動させる（添え字は1開始のため+1）
				if (dbrTree->db_result->absolute(columnNumber + 1)) {
					//カラムの値を取得する
					column = dbrTree->db_result->getString(key);
				}
				//ループを抜ける
				break;
			}
			else {
				//親をセットする
				dbrTree = dbrTree->parent;
			}
		}
		//columnを返す
		return column;
	}

	/* 関数名：getListJSON
	* 概要:リスト形式のJSONを作成して返す
	* 引数:Map<String, Object> json:JSONのオブジェクト。
	* 戻り値:String strAll:JSONの文字列配列を文字列で返す
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/

	//テスト完了。
	//あとで例外処理追加。
	std::string getListJSON(picojson::object json) {
		//返却する文字列を作成するための変数を3つ宣言、初期化する。
		std::string strAll = "";
		std::string strBlock = "";
		std::string strLine = "";
		try {
			//データベースから当該レコード群を取得する(結果セットを取得する)
			sql::ResultSet* rs = executeQuery(json, constants.DB_GETQUERY);
			//レコードセットの列名を取得する、あとでいじる。
			sql::ResultSetMetaData* rsMeta = rs->getMetaData();
			//レコードセットの列数を取得する、あとでいじる。
			int columnCount = rsMeta->getColumnCount();
			//結果セットの行についてのループ
			while (rs->next()) {
				//レコードの文字列を初期化する
				strLine = "";
				//列についてのループ
				for (int i = 0; i < columnCount; i++) {
					//列名を取得する(別名の取得)
					std::string sColName = rsMeta->getColumnLabel(i + 1);
					//列の値を取得する
					std::string value = rs->getString(i + 1);
					//列の値がNULLだった場合、ブランクに置換する
					value = value == constants.EMPTY_STRING ? constants.EMPTY_STRING : value;
					//文字列の行単位の変数が空でない時、行の文字列をカンマで区切る
					strLine += strLine == constants.EMPTY_STRING ? constants.EMPTY_STRING : ",";

					//取得した値に、JSON用のエスケープ処理を行う
					value = escapeJSONValue(value);

					//1行分のデータを文字列に追加する
					strLine += "\"" + sColName + "\":\"" + value + "\"";
				}
				//行に文字列が入っていたら、カンマで区切る
				strBlock += strBlock == constants.EMPTY_STRING ? constants.EMPTY_STRING : ",";
				//作成した行の文字列をブロックの文字列に追加する
				strBlock += "{" + strLine + "}";

			}
		}
		catch (std::exception) {

			throw;
		}
		//作成した全ブロックを配列の括弧で囲む
		strAll = "[" + strBlock + "]";

		//作成した文字列を返す
		return strAll;
	}

	/* 関数名：outputJSON
	* 概要:DBから取得したレコードでJSONを作る。
	* 引数:String jsonString:クライアントから受け取ったJSON文字
	*    :String key:JSONのトップのノードのキー。
	* 戻り値:無し
	* 作成日:2016.10.24
	* 作成者:R.Shibata
	*/

	void outputJSON(std::string jsonString, std::string key) {
		//引数のJSON文字列を変換して、JSONの連想配列を取得してクラスのメンバに格納する
		getJSONMap(jsonString);
		//例外に備える
		/*try*/ {
			//データベースに接続する
			connect();
			//JSON文字列の作成を行う
			createJSON(json, key, NULL);
			//データベースから切断する
			//あとで作る。
			disconnect();
		}/*
		catch (ClassNotFoundException | SQLException | NoSuchAlgorithmException e) {
			//エラーメッセージを表示する
			e.printStackTrace();
			//プログラムを止める
			return;
		}*/
	}

	/* 関数名：getJSONMap
	* 概要:JSON文字列から連想配列を生成する。
	* 引数:String jsonString:変換するJSON文字列
	* 戻り値:無し
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/

	//テスト成功。
	void getJSONMap(std::string jsonString) {
		// JSON文字列を連想配列に変換するためにパースする。
		picojson::value v;
		parse(v, jsonString);
		//picojsonのオブジェクトに変換することにより連想配列として扱えるようにする。
		picojson::object map = v.get<picojson::object>();
		// Mapに変換されたJSONをJSONDBManagerクラスのメンバに格納する
		json = map;
	}

	/* 関数名：checkColumn
	* 概要:結果セットに指定した列名を持つ列があるかをチェックする
	* 引数:ResultSet rs:指定した列があるかをチェックする対象の結果セット
	*    :String columnName:チェック対象の列名
	* 戻り値:無し
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/

	//例外処理後で追加
	bool checkColumn(sql::ResultSet* rs, std::string columnName) {
		bool retBoo = false;
		//結果セットがnullでない時の処理
		if (rs != NULL) {
			//列名を取得するため、MetaDataを取得する
			sql::ResultSetMetaData* rsMeta = rs->getMetaData();
			//MetaDataより列数を取得する
			int columnCount = rsMeta->getColumnCount();
			// 最初の結果セットから列を走査する
			for (int i = 0; i < columnCount; i++) {
				//結果セットの列に指定した列名の列が存在する(添え字開始位置が１のため+1)
				if (rsMeta->getColumnLabel(i + 1) == columnName) {
					// 返す変数にtrueを格納する
					retBoo = true;
					//チェック完了となり、ループを終了する
					break;
				}
			}
		}
		//判定を返す
		return retBoo;
	}

	/* 関数名：getListJSONPlusKey
	* 概要:getListJSONで作成した配列を、クライアントから送信されたJSONに格納して文字列で返す。
	* 引数:Object json:JSONのオブジェクト。
	*    :String key:キー名
	* 戻り値:String:オブジェクトで囲んだ配列のJSON文字列を返す
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/

	//テスト完了。
	//例外処理は後で追加。
	std::string getListJSONPlusKey(picojson::object json, std::string key) {
		//tsbleDataを補正するための文字列。
		//picojsonによる文字列のソートを避けるため。
		//getListJSONでテーブル用のJSON配列を作成する
		std::string retArray = ",\"tableData\":" + getListJSON(json) + '}';
		std::string	retStr = "";
		//JSON配列の文字列を配列データに変換し、引数のJSONに追加する
		//キー名に対応する値がJSONの中に存在しているかを確認。
		auto itr = json.find(key);
		if (itr != json.end()) {
			//存在している場合は削除する。
			json.erase(itr);
		}
			retStr = Encode(json);
			retStr.pop_back();

		//追加を行った引数のJSONを文字列に変換する
		//作成した文字列を返す
		return retStr + retArray;
	}

	/* 関数名：queryCorrection
	* 概要:Javaでは実行できないタイプのクエリーを修正して返却する
	*    :本来であればこんな処理は不要のはず
	* 引数:String query:修正対象のクエリー
	* 戻り値:String:修正後のクエリーを返却する
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/
	//いるかわからないけどとりあえず置いておく。
	std::string queryCorrection(std::string query) {
		//返却用クエリーの文字列を宣言し、入力のクエリーをセットする
		std::string retQuery = query;
		//クエリーの中にセミコロンが含まれている場合
		if (retQuery.find(";") != -1) {
			//クエリの最初の;を検索しindexを保持する。;も出力対象のため、数値を+1する
			int index = retQuery.find(";") + 1;
			//クエリー文字列から最初の";"までを切り出す
			retQuery = retQuery.substr(0, index);
		}
		//修正したクエリーを返却する
		return retQuery;
	}

	/* 関数名：escapeJSONValue
	* 概要:引数で受け取った文字列をJSON用のエスケープ処理を行い返却する
	* 引数:String str:エスケープする文字列
	* 戻り値:String:エスケープした文字列
	* 作成日:2016.10.27
	* 作成者:R.Shibata
	*/
	std::string escapeJSONValue(std::string str) {
		//返却用文字列に、引数の文字列をセットする
		std::string retStr = str;
		//￥マークをエスケープ文字に置き換える(replaceAllは正規表現として扱われるため、￥一つが\4つ分となる）
		//文字列を置き換える関数を呼び出す。
		retStr = Replace(retStr, "\\", "\\\\");
		//改行文字をエスケープ文字に置き換える
		retStr = Replace(retStr, "\r\n", "\\n");
		retStr = Replace(retStr, "\r", "\\n");
		retStr = Replace(retStr, "\n", "\\n");
		//ダブルクォートをエスケープ文字に置き換える
		retStr = Replace(retStr, "\"", "\\\"");
		//エスケープした値を返却する
		return retStr;
	}

	/* 関数名：escapeSQLValue
	* 概要:引数で受け取った文字列をSQL用のエスケープ処理を行い返却する
	* 引数:String str:エスケープする文字列
	* 戻り値:String:エスケープした文字列
	* 作成日:2016.10.27
	* 作成者:R.Shibata
	*/
	std::string escapeSQLValue(std::string str) {
		//返却用文字列に、引数の文字列をセットする
		std::string retStr = str;
		//SQL実行時削除されるため￥マークをエスケープする
		retStr = Replace(retStr, "\\", "\\\\\\");
		//SQL実行できなくなるため、シングルクォートをエスケープする
		retStr = Replace(retStr, "'", "\\'");
		return retStr;
	}

	/* 関数名：getMapValue
	* 概要:指定したmapの一つ下の階層のデータを取得する
	* 引数:Map<String, Object> map:値を取得したいmapオブジェクト
	*    :String jsonKey:mapから取得したい値のキー
	*    :String vauleKey:mapのkeyに含まれる値を示すキー
	* 戻り値:String:取得した文字列
	* 作成日:2016.10.27
	* 作成者:R.Shibata
	*/
	std::string getJsonValue(picojson::object map, std::string jsonKey, std::string valueKey) {
		//引数のmapから下位のオブジェクトを取得する
		picojson::object tempObject = map[jsonKey].get< picojson::object>();
		//下位オブジェクトよりデータを取得し、その値を返却する
		return tempObject[valueKey].get<std::string>();
	}

	/* 関数名：getOutHTMLString
	* 概要:クライアントに出力する用の文字列であるoutHTMLStringを返却する
	* 引数:無し
	* 戻り値:String:outHTMLString
	* 作成日:2016.10.25
	* 作成者:R.Shibata
	*/

	//テスト完了。
	std::string getOutHTMLString() {
		//outHTMLStringを返却する
		return outHTMLString;
	}

	/* 関数名：setOutHTMLString
	* 概要:クライアントに出力する用の文字列であるoutHTMLStringを設定する。
	* 引数:String outHTMLString:設定する文字列
	* 戻り値:無し
	* 作成日:2016.10.25
	* 作成者:R.Shibata
	*/

	//テスト完了。
	void setOutHTMLString(std::string setoutHTMLString) {
		//入力の値を設定する
		outHTMLString = setoutHTMLString;
	}

	//作成したcookieを取得する。
	std::string getCookieString() {
		//outHTMLStringを返却する
		return outCookieString;
	}

	//作成したcookieを連結するクラス。
	void setOutCookieString(std::string setoutCookieString) {
		//入力の値を設定する
		outCookieString += setoutCookieString;
	}

	//javaのreplaceAllに代わる文字列を置き換える関数。
	std::string Replace(std::string rep, std::string target, std::string change) {
		for (int i = 0; i < rep.length();) {
			//置き換え対象の文字列を探す。
			i = rep.find(target, i);
			//置き換え対象の文字が見つかったら。
			if (i <= rep.length()) {
				//文字列を置き換える。
				rep.replace(i, target.length(), change);
				i += change.length();
			}
		}

		//置き換え後の文字列を返す。
		return rep;
	}

	//JSON内を走査しパスワードを返す関数。
	bool SearchJSON(picojson::object& json, std::string key) {
		bool judge = false;		//判定結果を格納する。

								//jsonについて最下層の要素にたどり着くまでループしてデータを取り出す。
		for (picojson::object::iterator itr = json.begin(); itr != json.end(); ++itr) {
			//すでにキー名を見つけて書き換えていた場合。
			if (judge) {
				//ループを抜ける。
				break;
			}
			else if (itr->second.to_str() == "object")
			{
				//再帰処理
				judge = SearchJSON(itr->second.get<picojson::object>(), key);
			}
			//キー名に対応した値を見つけたとき。
			else if (itr->first == "src")
			{
				//書き換えが終わったことを格納する。
				if (json["src"].get<std::string>() == key) {
					judge = true;
					//ループを抜ける。
					break;
				}
			}
		}

		//判定結果を返す。
		return judge;
	}

	//JSON内を走査しパスワードを返す関数。
	bool ChangeJSON(picojson::object& json, std::string key, std::string change) {
		bool judge = false;		//判定結果を格納する。

								//jsonについて最下層の要素にたどり着くまでループしてデータを取り出す。
		for (picojson::object::iterator itr = json.begin(); itr != json.end(); ++itr) {
			//すでにキー名を見つけて書き換えていた場合。
			if (judge) {
				//ループを抜ける。
				break;
			}
			//キー名に対応した値を見つけたとき。
			else if (itr->first == key)
			{
				//1階層下にパスワードのvalueがある場合書き換える。
				itr->second.get<picojson::object>()["value"] = (picojson::value)change;
				//書き換えが終わったことを格納する。
				judge = true;
				//ループを抜ける。
				break;
			}
			//子が存在していれば。
			else if (itr->second.to_str() == "object")
			{
				//再帰処理
				judge = ChangeJSON(itr->second.get<picojson::object>(), key, change);
			}
		}

		//判定結果を返す。
		return judge;
	}

	//テスト完了。
	//JSON内を走査し、キー名に対応した値を返す関数。
	std::string CatchJSON(picojson::object& json, std::string key) {
		std::string result = "roop";	//取り出した値を格納する文字列。初期値として使わないであろう文字を入れておく。

		//jsonについて最下層の要素にたどり着くまでループしてデータを取り出す。
		for (picojson::object::iterator itr = json.begin(); itr != json.end(); ++itr) {
			//すでに値を取り出していた場合。
			if (result != "roop") {
				//ループを抜ける。
				break;
			}
			//キー名に対応した値を見つけたとき。
			else if (itr->first == key)
			{
				//子要素が残っている場合。
				//この部分は変える可能性高い。
				if (itr->second.to_str() == "object") {
					//いったんパースした状態に変換する。
					picojson::value v(itr->second);
					//JSONのネスト構造に成型して返す。
					result = v.serialize();
					//ループを抜ける。
					break;
				}
				else if (json[key].is<std::string>()) {
					//値を取り出す。
					result = json[key].get<std::string>();
					break;
				}
			}
			//取り出しておいたキーが"text"または"html"または"src"であれば。
			//あとで定数に置き換え。
			else if (itr->second.to_str() == "object")
			{
				//再帰処理
				result = CatchJSON(itr->second.get<picojson::object>(), key);
			}
		}

		//取り出した値を返す。
		return result;
	}

	//テスト完了。
	//javaのJSON.encodeに当たる関数。
	std::string Encode(picojson::object& json) {
		//JSONから文字列に変換して返す。
		return picojson::value(json).serialize();
	}

	//テスト完了。
	//javaのJSON.decodeに当たる関数。
	picojson::value Decode(std::string json) {
		picojson::value v;
		//文字列をパースする。
		parse(v, json);
		//文字列をJSONに変換。
		return v;
	}

	//javaの文字列分割関数splitに当たるもの。自作。
	std::vector<std::string> split(const std::string &str, char sep)
	{
		std::vector<std::string> v;
		std::stringstream ss(str);
		std::string buffer;
		while (std::getline(ss, buffer, sep)) {
			v.push_back(buffer);
		}
		return v;
	}

	//受け取った文字列をSHA1 に変換して返却する関数。
	typedef boost::array<boost::uint8_t, 20> hash_data_t;

	std::string changeToSHA1(std::string change) {
		std::stringstream ss;
		std::cout << std::hex;
		std::string tmp;
		const char* changechar = change.c_str();

		hash_data_t hash = get_sha1_hash(changechar, change.length());
		hash_data_t::const_iterator itr = hash.begin();
		const hash_data_t::const_iterator end_itr = hash.end();
		for (; itr != end_itr; ++itr)
		{
			ss << std::hex << ((*itr & 0xf0) >> 4)
				<< (*itr & 0x0f);

			tmp = ss.str();
		}

		return tmp;
	}

	//SHA1変換の内部処理。

	hash_data_t get_sha1_hash(const void *data, const std::size_t byte_count)
	{
		boost::uuids::detail::sha1 sha1;
		sha1.process_bytes(data, byte_count);
		unsigned int digest[5];
		sha1.get_digest(digest);
		const boost::uint8_t *p_digest = reinterpret_cast<const boost::uint8_t *>(digest);
		hash_data_t hash_data;
		for (int i = 0; i < 5; ++i)
		{
			hash_data[i * 4] = p_digest[i * 4 + 3];
			hash_data[i * 4 + 1] = p_digest[i * 4 + 2];
			hash_data[i * 4 + 2] = p_digest[i * 4 + 1];
			hash_data[i * 4 + 3] = p_digest[i * 4];
		}
		return hash_data;
	}

	/*デバッグ用*/
	void fileout(string str) {
		string filename = "debug";
		ofstream ofs;
		ofs.open(filename, ios::app);
		ofs << str << "\n\n";
		ofs.close();
	}
};
/*
//テスト用のmain
int main() {
	std::ifstream ifs("Text.txt");	//JSONが格納されたファイルを開く、実際の動作の時はクライアントよりJSONを受け取る。
	std::string err;				//パースした際のエラーを格納する文字列。
	std::string json;				//開いたファイルの中身を格納する文字列。パースに利用する。
	picojson::value val;			//パース後のJSONを格納する変数。。
	picojson::object query;				//key名に対応する値を格納する文字列
	JSONDBManager DB;

	while (!ifs.eof()) {
		std::string work = "";
		ifs >> work;
		json += work;
	}
	ifs.close();

	err = parse(val, json);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	//JSON配列をオブジェクトに変換する。
	picojson::object& obj = val.get<picojson::object>();
	DB.createJSON(obj, "test", NULL);

	std::cout << ((picojson::value)obj).serialize(true) << '\n';

	return 0;
}
*/