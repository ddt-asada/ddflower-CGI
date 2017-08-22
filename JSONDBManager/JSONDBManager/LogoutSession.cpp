#include "procedureLogout.h"
#include "Request.h"

/*
* ファイル名:jsp/LogoutSession.jsp
* 概要	:ログアウト処理のため、セッションを破棄する。
* 作成者:R.Shibata
* 作成日:2016.10.24
*/

//未完成。
int main() {
	//ServerRequest request;

	const char* answer = "Content-Type:text/html;charset=utf-8\n";

	std::cout << answer;

	//std::string json = request.HTTPRequest();

	JSONDBManager dbug;
	//クライアントへ返却するための文字列を宣言する
	std::string outValue = "";
	//リスト形式のJSONを作るクラスのインスタンスを作成する
	procedureLogout logout;
	//ログイン失敗エラーをキャッチするため、tryブロックで囲む
	/*try {*/
		//生成したインスタンスの処理関数を実行する
		logout.run();
		//ログインチェックエラーが発生した場合
/*	}
	catch (Exception e) {
		//エラーメッセージを表示する
		e.printStackTrace();
	}*/
	//作成した文字列を表示しクライアントへ返却する
	std::cout << '\n' <<outValue <<'\n';
	return 0;
}