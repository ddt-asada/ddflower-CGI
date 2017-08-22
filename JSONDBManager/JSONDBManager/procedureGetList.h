#pragma once

#include "procedureBase.h"

/* クラス名:procedureGetList
* 概要:JSONDBManagerを利用し、クライアント側から送信されたJSONのクエリを
*     基にDBから取得したデータをテーブルにしてクライアントに返す役割のクラス。
* 作成日:2016.10.20
* 作成者:R.Shibata
*/

class procedureGetList : public procedureBase {
public:
	procedureGetList() {

	};

	/* クラス名:init
	* 概要:クラスの初期化関数。
	* 引数:無し
	* 戻り値:無し
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/
	void init(){
		//親クラスのinit関数をコールする。
		procedureBase::init();
	}

	/* クラス名:job
	* 概要:クラス特有の処理を行う関数。
	* 引数:String $jsonString:JSON文字列
	* 戻り値:無し
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/
	void job(std::string jsonString) {
		//親クラスのjobを実行し、メンバにJSONの連想配列を格納する
		procedureBase::job(jsonString);

		//返却するJSON配列の文字列を格納する変数を用意する
		std::string retArrayString = "";

		//SQLによる例外の対処のため、tryブロックで囲む
		try {
			//レコードのJSONを作成する
			retArrayString = getListJSONPlusKey(json, constants.STR_TABLE_DATA);
		}
		catch (sql::SQLException &e) {
			// エラーメッセージを表示する
			//e.printStackTrace();
			// プログラムを終了する
		//	return;
		}
		//DBとの接続を閉じる
		disconnect();
		//作成したJSON文字列を出力用文字列へ設定する
		setOutHTMLString(retArrayString);
	}

	/* クラス名:run
	* 概要:クラスのinit、job関数をまとめて実行する。
	* 引数:String $jsonString:JSON文字列
	* 戻り値:無し
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/
	void run(std::string jsonString){
		//初期化処理とクラス独自の処理をまとめて実行する
		init(); //初期化関数
		job(jsonString); //クラス特有の処理を行う
	}
};