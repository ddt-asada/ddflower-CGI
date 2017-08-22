#include "procedureSet.h"
#include "Request.h"

/*
* ファイル名:jsp/GetJSONString.jsp
* 概要	:テーブルのタグ作成用のJSON配列を作成して返す。
* 作成者:R.Shibata
* 作成日:2016.10.24
*/

int main() {

	//リクエストを受け取るためのクラスをインスタンス化。
	ServerRequest request;
	//クライアントへ返却するための文字列を宣言する
	std::string outValue = "";
	//クライアントから取得したjsonデータを取得する
	std::string json = request.HTTPRequest();
	//リスト形式のJSONを作るクラスのインスタンスを作成する
	procedureSet procedureset;
	//ログイン失敗エラーをキャッチするため、tryブロックで囲む
	/*try {*/
		//生成したインスタンスの処理関数を実行する
		procedureset.run(json/*, request, response*/);
		//作成した文字列を表示するための文字列へ格納する
		outValue = procedureset.getOutHTMLString();
		//ログインチェックエラーが発生した場合
	/*}
	catch (LoginCheckException e) {
		//エラーメッセージを作成し、表示するための文字列へ格納する
		outValue = Constants.ERROR_JSON_FRONT + e.checkLoginState() + Constants.ERROR_JSON_BACK;
		//その他エラーが発生した場合
	}
	catch (Exception e) {
		//エラーメッセージを表示する
		e.printStackTrace();
	}
	*///作成した文字列を表示しクライアントへ返却する
	cout << outValue;

	return 0;
}