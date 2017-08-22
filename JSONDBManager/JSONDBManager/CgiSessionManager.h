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
	
	//ID���d�Ȃ邱�Ƃ����Ȃ��Ȃ�悤�ɓ��������Ă���
	oss << setw(8) << (long)time(NULL);

	//������A�����Ă���
	for(int i=0; i<12; ++i){
		int c = (int)((float)rand()/RAND_MAX*255);
		oss << setw(2) << c;
	}
	return oss.str();
};

}; //namespace{



	
//�Z�b�V����ID�̍쐬�Ɛݒ�B������true
bool CgiSession::createAndSetSessionId()
{
	for(int i=0; i<3; ++i){
		m_sessionId = createSessionId();
		if(!lock()) continue;
		//���ɑ��݂���ID���H���`�F�b�N
		if(!isAlreadyExistsId()){
			//�t�@�C�����쐬���Ă��܂��B
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
		//�Z�b�V�����f�[�^�����[�h
		string file = createSessionFileName();
		ifstream ifs(file.c_str());

		//�Z�b�V�����f�[�^�����݂��Ȃ��ꍇ
		if(!ifs.is_open()){
			m_sessionId = "";
			return;
		}

		//�Z�b�V�������Ԃ̓ǂݍ���
		ifs >> m_sessionTime;
		if(ifs.peek() == '\n') ifs.ignore();
		
		//�^�C���A�E�g�̏ꍇ�f�[�^�N���A 
		if(isTimeout()){
			saveMap.clear();
			return;
		}
		//
		loadData(saveMap, ifs);
		ifs.close();
	}
};
 
//�^�C���A�E�g�������H
bool CgiSession::isTimeout() const 
{
	if(m_timeout == 0) return false;
	int diff = static_cast<int>(time(NULL) - m_sessionTime);
	if(diff > m_timeout*60) return true;
	return false;
}; 

///�Z�b�V�����t�@�C�����폜����
bool CgiSession::remove()
{
	if(!lock()) return false;
	string file = createSessionFileName();
	::remove(file.c_str());
	if(!unlock()) return false;
	
	m_sessionId = "";
	return true;
};
	
	 
///�Z�b�V�����f�[�^��ۑ�����
void CgiSession::save() const
{
	if(m_sessionId.empty()) return;
	if(!lock()){
		cerr << "CgiSession can't save data."  << endl;
		saveData(saveMap, cerr);
		return;
	}

	//�Z�b�V�����f�[�^��ۑ�
	string file = createSessionFileName();
	ofstream ofs(file.c_str());
	ofs << time(NULL) << endl;
	saveData(saveMap, ofs);
	ofs.close();

	if(!unlock()) return;
};

	
///�Z�b�V�����t�@�C�������b�N����
bool CgiSession::lock() const
{
	string lockDir = m_dir + string("__") + m_sessionId;
	if(mkdir(lockDir.c_str())) return false;
	return true;
}

///�Z�b�V�����t�@�C�������b�N��������
bool CgiSession::unlock() const
{
	string lockDir = m_dir + string("__") + m_sessionId;
	if(rmdir(lockDir.c_str())) return false;
	return true;
}

///�Z�b�V�����f�[�^��ۑ�����t�@�C�������쐬����
const string CgiSession::createSessionFileName() const
{
	return m_dir + m_sessionId;
};

///���Ɏg�p����Ă���ID���ǂ����H
const bool CgiSession::isAlreadyExistsId() const
{
	string file = createSessionFileName();
	ifstream ifs(file.c_str());
	//1�����ǂݏo���ēǂ߂�ꍇ�A���ɑ��݂���
	ifs.peek();
	bool bl = (bool)ifs;
	ifs.close();
	return bl;
};


}; //namespace nana{

