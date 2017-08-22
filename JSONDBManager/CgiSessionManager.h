#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <direct.h>
#include <ctime>

#include "CgiSession.h"


namespace nana{


namespace{

const string createSessionId()
{
	srand((unsigned int)time(NULL));
	ostringstream oss;
	oss << hex << uppercase << setfill('0');
	
	//IDが重なることが少なくなるように日時を入れておく
	oss << setw(8) << (long)time(NULL);

	//乱数を連結していく
	for(int i=0; i<12; ++i){
		int c = (int)((float)rand()/RAND_MAX*255);
		oss << setw(2) << c;
	}
	return oss.str();
};

}; //namespace{



	
//セッションIDの作成と設定。成功時true
bool CgiSession::createAndSetSessionId()
{
	for(int i=0; i<3; ++i){
		m_sessionId = createSessionId();
		if(!lock()) continue;
		//既に存在するIDか？をチェック
		if(!isAlreadyExistsId()){
			//ファイルを作成してしまう。
			string file = createSessionFileName();
			ofstream ofs(file.c_str());
			ofs << time(NULL) << endl;
			ofs.close();
			unlock();
			return true;
		}
		//
		unlock();
	}
	m_sessionId = "";
	return false;
};


void CgiSession::load()
{
	time_t sessionTime = 0;
	if(m_sessionId.empty()){
		createAndSetSessionId();
	}else{
		//セッションデータをロード
		string file = createSessionFileName();
		ifstream ifs(file.c_str());

		//セッションデータが存在しない場合
		if(!ifs.is_open()){
			m_sessionId = "";
			return;
		}

		//セッション時間の読み込み
		ifs >> m_sessionTime;
		if(ifs.peek() == '\n') ifs.ignore();
		
		//タイムアウトの場合データクリア 
		if(isTimeout()){
			saveMap.clear();
			return;
		}
		//
		loadData(saveMap, ifs);
		ifs.close();
	}
};
 
//タイムアウトしたか？
bool CgiSession::isTimeout() const 
{
	if(m_timeout == 0) return false;
	int diff = static_cast<int>(time(NULL) - m_sessionTime);
	if(diff > m_timeout*60) return true;
	return false;
}; 

///セッションファイルを削除する
bool CgiSession::remove()
{
	if(!lock()) return false;
	string file = createSessionFileName();
	::remove(file.c_str());
	if(!unlock()) return false;
	
	m_sessionId = "";
	return true;
};
	
	 
///セッションデータを保存する
void CgiSession::save() const
{
	if(m_sessionId.empty()) return;
	if(!lock()){
		cerr << "CgiSession can't save data."  << endl;
		saveData(saveMap, cerr);
		return;
	}

	//セッションデータを保存
	string file = createSessionFileName();
	ofstream ofs(file.c_str());
	ofs << time(NULL) << endl;
	saveData(saveMap, ofs);
	ofs.close();

	if(!unlock()) return;
};

	
///セッションファイルをロックする
bool CgiSession::lock() const
{
	string lockDir = m_dir + string("__") + m_sessionId;
	if(mkdir(lockDir.c_str())) return false;
	return true;
}

///セッションファイルをロック解除する
bool CgiSession::unlock() const
{
	string lockDir = m_dir + string("__") + m_sessionId;
	if(rmdir(lockDir.c_str())) return false;
	return true;
}

///セッションデータを保存するファイル名を作成する
const string CgiSession::createSessionFileName() const
{
	return m_dir + m_sessionId;
};

///既に使用されているIDかどうか？
const bool CgiSession::isAlreadyExistsId() const
{
	string file = createSessionFileName();
	ifstream ifs(file.c_str());
	//1文字読み出して読める場合、既に存在する
	ifs.peek();
	bool bl = (bool)ifs;
	ifs.close();
	return bl;
};


}; //namespace nana{

