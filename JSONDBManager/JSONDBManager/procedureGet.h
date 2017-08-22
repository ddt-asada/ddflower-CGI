#pragma once

#include "procedureBase.h"

class procedureGet : public procedureBase {
public:
	procedureGet() {

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

		//JSONを取得する
		//SQLによる例外の対処のためtryブロックで囲む
		try {
			//JSON文字列の作成を行う
			createJSON(json, constants.EMPTY_STRING, NULL);
			//SQL例外のcatchブロック
		}
		catch (sql::SQLException &e) {
			//エラーメッセージを表示する
		//	e.printStackTrace();
			//プログラムを終了する
			return;
		}

		//DBとの接続を閉じる
		disconnect();

		//連想配列をjsonに変換して変数に入れる
		std::string jsonOut = Encode(json);
		//作成したJSON文字列を出力用文字列へ設定する
		setOutHTMLString(jsonOut);
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