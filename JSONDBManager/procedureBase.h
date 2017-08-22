#pragma once

#include "account.h"

class procedureBase : public account {
public:
	procedureBase() {

	};

	void init(){
		//親クラスのinit関数をコールする
		account::init();
		//ログインチェックを行う
		account::loginCheck();
	}

	void job(std::string jsonString) {
		//JSON文字列から連想配列を取得し、自身のメンバに保存する
		getJSONMap(jsonString);
	}

	void run(std::string jsonString) {
		//初期化処理とクラス独自の処理をまとめて実行する
		init();//初期化関数
		job(jsonString); //クラス特有の処理を行う
	}
};