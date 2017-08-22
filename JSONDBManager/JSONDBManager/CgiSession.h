#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>

#include "CgiSaveData.h"


namespace nana{

using namespace std;




/**
CGI�̃Z�b�V�����N���X�B
�^�C���A�E�g����ƃf�[�^�̓N���A����܂��B 
timeout��0�̂Ƃ��A�^�C���A�E�g�Ȃ��B 
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

	///�R���X�g���N�^
	CgiSession(const string& sessionId, const string& dir = "", const int& timeout = 30)
		: m_sessionId(sessionId), m_dir(dir), m_timeout(timeout), m_sessionTime(time(NULL)) {};
	
	///�Z�b�V����ID�̍쐬�Ɛݒ�B������true
	bool createAndSetSessionId();
	
	/**
	����������B�g�p�O�ɕK���Ăяo�����ƁB
	���s����� getId().empty() ��true��Ԃ��B 
	*/
	void load();
	 
	///�^�C���A�E�g�������H
	bool isTimeout() const; 
	
	///�Z�b�V�����t�@�C�����폜����
	bool remove();
	
	 
	///�Z�b�V�����f�[�^��ۑ�����
	void save() const;
	
	///�Z�b�V����ID���擾����
	const string& getId() const
	{
		return m_sessionId;
	};
	
	///�Z�b�V������ۑ������������擾����
	const time_t& getSessionTime() const 
	{
		return m_sessionTime;
	};
	
protected:
	///�Z�b�V�����t�@�C�������b�N����
	bool lock() const;

	///�Z�b�V�����t�@�C�������b�N��������
	bool unlock() const;

	///�Z�b�V�����f�[�^��ۑ�����t�@�C�������쐬����
	const string createSessionFileName() const;

	///���Ɏg�p����Ă���ID���ǂ����H
	const bool isAlreadyExistsId() const;
};


}; //namespace nana{ �I��




