#pragma once


#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <direct.h>

#include "CgiSaveData.h"

namespace nana{

using namespace std;


/**
Data1=Data2%3Derror;Data2=xyz
NAME=�l; expires=�l; domain=�l; path=�l; secure
http://www.tohoho-web.com/wwwcook.htm
Cookie���f�[�^���L�^�ł���̂�4096�o�C�g�܂łŁA�ő��300�܂ŕۑ����邱�Ƃ��\�ł�
*/
class CgiCookie
{
public:
	string domain;
	string path;
	bool isSecure;
	string name;
	string value;
	time_t maxAge;
	
	//�f�t�H���g�R���X�g���N�^�B�g��Ȃ��B
	CgiCookie(){
	};
	
	//cookie���쐬����R���X�g���N�^�B
	CgiCookie(const string& name, const string& value)
	 : name(name), value(value), isSecure(false), maxAge(0), path("/")
	{
		//���ݎ�����ݒ�邷��B
		maxAge = time(NULL);
	};

	//�������Ԃ�ݒ肷��B�B
	void setMaxAge(int expir)
	{
		this->maxAge = expir;
	}

	//�������Ԃ����Z����B�B
	void addMaxAge(int expir)
	{
		this->maxAge += expir;
	}

	//�p�X��ݒ肷��B
	void setPath(std::string path) {
		this->path = path;
	}

	///01-Jan-1970 00:00:00 GMT�`��
	const string createExpires() const;
};


///�N�b�L�[�N���X�̏o�̓I�[�o�[���[�h
ostream& operator << (ostream& os, const CgiCookie& cookie);

///�N�b�L�[�f�[�^�̃N�b�L�[�X�g�����O����̃��[�h
void loadCookie(SaveDataMap& smap, const string& cookieStr);


}; //namespace nana{

