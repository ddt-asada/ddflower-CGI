#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>

#include "CgiSaveData.h"


namespace nana{

using namespace std;




/**
CGIのセッションクラス。
タイムアウトするとデータはクリアされます。 
timeoutが0のとき、タイムアウトなし。 
*/
class CgiSession
{
public:
	SaveDataMap saveMap;

	string m_sessionId;
	string m_dir;
	int m_timeout;
	time_t m_sessionTime;

	CgiSession() {

	};

	///コンストラクタ
	CgiSession(const string& sessionId, const string& dir = "", const int& timeout = 30)
		: m_sessionId(sessionId), m_dir(dir), m_timeout(timeout), m_sessionTime(time(NULL)) {};
	
	///セッションIDの作成と設定。成功時true
	bool createAndSetSessionId();
	
	/**
	初期化する。使用前に必ず呼び出すこと。
	失敗すると getId().empty() がtrueを返す。 
	*/
	void load();
	 
	///タイムアウトしたか？
	bool isTimeout() const; 
	
	///セッションファイルを削除する
	bool remove();
	
	 
	///セッションデータを保存する
	void save() const;
	
	///セッションIDを取得する
	const string& getId() const
	{
		return m_sessionId;
	};
	
	///セッションを保存した日時を取得する
	const time_t& getSessionTime() const 
	{
		return m_sessionTime;
	};
	
protected:
	///セッションファイルをロックする
	bool lock() const;

	///セッションファイルをロック解除する
	bool unlock() const;

	///セッションデータを保存するファイル名を作成する
	const string createSessionFileName() const;

	///既に使用されているIDかどうか？
	const bool isAlreadyExistsId() const;
};


}; //namespace nana{ 終了




