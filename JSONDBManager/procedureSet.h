#pragma once

#include "procedureBase.h"

class procedureSet : public procedureBase {
public:
	procedureSet()
	{

	};
	/* クラス名:init
	* 概要:クラスの初期化関数。ログイン用のクラスの初期化関数をコールする。
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
	* 概要:クラス特有の処理を行う関数。ログイン処理を行う。
	* 引数:String $jsonString:JSON文字列
	* 戻り値:無し
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/
	void job(std::string jsonString) {
		//親クラスのjobを実行し、メンバにJSONの連想配列を格納する
		procedureBase::job(jsonString);
		//JSONをDBに反映させる
		//SQLによる例外の対処のため、tryブロックで囲む
		//INSERT、またはUPDATE命令を実行する
		executeQuery(json, constants.DB_SETQUERY);
		//SQL例外のcatchブロック
		//最後に行う処理
		disconnect();
		//クライアントへ返すメッセージを作成する
		std::string returnMessage = "{\"message\":\"" + std::to_string(processedRecords) + "\"}";
		//作成したJson文字列を出力用文字列へ設定する
		setOutHTMLString(returnMessage);
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