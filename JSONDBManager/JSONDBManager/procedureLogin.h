#pragma once

#include "account.h"
#include "procedureBase.h"

procedureBase Base;

class procedureLogin : public account {
public:
	procedureLogin() {

	};

	/* クラス名:init
	* 概要:クラスの初期化関数。ログイン用のクラスの初期化関数をコールする。
	* 引数:無し
	* 戻り値:無し
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/
	void init(){
		//ok
		//accountクラスのinit関数をコールする。
		account::init();
	}

	/* クラス名:job
	* 概要:クラス特有の処理を行う関数。ログイン処理を行う。
	* 引数:String $jsonString:JSON文字列
	* 戻り値:無し
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/
	void job(std::string jsonString) {
		//ログインを実行する
		login(jsonString);
		//DBとの接続を閉じる
		disconnect();
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