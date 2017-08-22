#include "DbConnect.h"
#include "procedureGet.h"
#include "procedureLogin.h"
#include "Request.h"
#include "JSONtest.h"

/*
* ファイル名:jsp/GetJSONString.jsp
* 概要	:クライアントから渡されたJSON文字列にDBから取得した値を追加して返す。
* 		また、ログイン用のJSONを渡されたらログイン用のクラスを生成してログイン処理を行う。
* 作成者:R.Shibata
* 作成日:2016.10.24
*/

int main() {
	Constants constants;
	JSONDBManager test;
	
	const char* answer = "Content-Type:text/html;charset=utf-8\n";

	std::cout << answer;

	//HTTPよりPOSTメソッドでデータを受け取るクラスのインスタンス化。
	ServerRequest request;

	//HTTPよりPOSTメソッドで送られたデータを受け取る。
	std::string json = request.HTTPRequest();
	
	//クライアントへ返却するための文字列を宣言する
	std::string outValue = "";
	//テスト用の仮の文字列。
	//std::string json = "{\"db_getQuery\":\"SELECT id AS numberArea, user_name, get_point, authority FROM user_inf WHERE id = 'user_key'; \",\"user_key\":{\"value\":\"3\"},\"authority\":{\"text\":\"10\"},\"memberStatus\":{\"topLogoArea\":{\"topLink\":{\"href\":\"#top.php\",\"topLogoImage\":{\"src\":\"image / top(logo).png\"}}},\"memberName\":{\"user_name\":{\"text\":\"\"},\"nameTitleArea\":{\"text\":\"様\"}},\"memberPoint\":{\"get_point\":{\"text\":\"\"},\"ptArea\":{\"class\":\"ptArea unitSpace\",\"text\":\"pt\"}},\"logoutArea\":{\"class\":\"logoutArea inlineBlock\",\"logoutLink\":{\"href\":\"\",\"logoutImage\":{\"src\":\"image / icon(logout22 - 50).png\"}}}}}";
	//jsonにユーザ名、パスワード、IDの文字列が含まれていれば
	if (json.find(constants.USER_NAME) != -1 && json.find(constants.STR_PASSWORD) != -1
		&& json.find(constants.STR_ID) != -1) {
		//ログイン用の処理を行うクラスのインスタンスを生成する
		procedureLogin procedurelogin;
		//ログイン失敗エラーをキャッチするため、tryブロックで囲む
		/*try {*/
		//生成したインスタンスの処理関数を実行する
		procedurelogin.run(json);
		//作成した文字列を表示するための文字列へ格納する
		outValue = procedurelogin.getOutHTMLString();

	//ログインチェックエラーが発生した場合
	/*	}
		catch (Exception e) {
		//エラーメッセージを表示する
		e.printStackTrace();
		}*/
	}
	else {
		//ログイン用の処理を行うクラスのインスタンスを生成する
		procedureGet procedureget;
		//ログイン失敗エラーをキャッチするため、tryブロックで囲む
		//try {
		//生成したインスタンスの処理関数を実行する
		procedureget.run(json);
		//作成した文字列を表示するための文字列へ格納する
		outValue = procedureget.getOutHTMLString();
		//ログインチェックエラーが発生した場合
	//		}
/*		catch (std::exception &e) {
		//エラーメッセージを表示する
			outValue = constants.ERROR_JSON_FRONT + "0" + constants.ERROR_JSON_BACK;
		}*/
	}
	//作成した文字列を表示しクライアントへ返却する
	std::cout << '\n' <<outValue;

	return 0;
}