#pragma once

#include "procedureBase.h"

class procedureLogout : public procedureBase {
public:
	procedureLogout() {

	};
	/* クラス名:init
	* 概要:クラスの初期化関数。親クラスの初期化関数をコールする。
	* 引数:無し
	* 戻り値:無し
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/
	void init(){
		//親クラスのinit関数をコールする
		procedureBase::init();
	}

	/* クラス名:job
	* 概要:クラス特有の処理を行う関数。ログアウト処理を行う。
	* 引数:無し
	* 戻り値:無し
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/
	void job() {
		//ログアウト処理を行う
		logout();
		//DBとの接続を閉じる
		disconnect();
	}

	/* クラス名:run
	* 概要:クラスのinit、job関数をまとめて実行する。
	* 引数:無し
	* 戻り値:無し
	* 作成日:2016.10.20
	* 作成者:R.Shibata
	*/
	void run(){
		//初期化処理とクラス独自の処理をまとめて実行する
		init(); //初期化関数
		job(); //クラス特有の処理を行う
	}
};