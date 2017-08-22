#pragma once

#include "boost/io/ios_state.hpp"

#include "CgiCookie.h"
#include "JSONDBManager.h"

namespace nana{


using namespace boost::io;

class CgiCookieManager {
public:
		SaveDataMap cmap;
		std::map<std::string, CgiCookie> retCookie;		//�ԋp�p�̃}�b�v�B
		JSONDBManager test;

		//�f�t�H���g�R���X�g���N�^�B�}�b�v��cookie���Z�b�g���Ďg����悤�ɂ���B
		CgiCookieManager() {
			char* env = getenv("HTTP_COOKIE");
			string str = (env == NULL ? "" : env);

			//�N�b�L�[�}�b�v���Z�b�g���Ďg����悤�ɂ���B 
			loadCookie(cmap, str);
		}

		//�R���X�g���N�^�B�Z�b�g����cookie���������Ă��Ă��烁���o��cookie�ɒǉ�����B
		CgiCookieManager(std::string name, std::string value, int expir) {
			//�����o��cookie��name��value���Z�b�g�ɂ��Ēǉ�����B
			cmap.insert(make_pair(name, value));
			//�����o��cookie�ɐ������Ԃ��Z�b�g����B
			cmap.insert(make_pair("expir", std::to_string(expir)));
			//�����o�̃p�X��"/"���Z�b�g����B
			cmap.insert(make_pair("path", "/"));
		}

		//�L�[�ɑΉ������l��T���ĕԋp����B
		std::string getCookieValue(std::string key) {

			//�ԋp�p�̕�����B
			std::string cookieValue = "";

			//�L�[�ɑΉ������l���擾����B�Ȃ��ꍇ��NULL�ɂȂ�B
			cookieValue = cmap[key];

			//�l��ԋp����B

			return cookieValue;
		}

		//�Z�b�g����cookie���������Ă��Ă��烁���o��cookie�ɒǉ�����B
		void setCookie(std::string name, std::string value, int expir) {
			//�Z�b�g���邽�߂̃N�b�L�[���쐬����
			CgiCookie cookie(name, value);
			//�������Ԃ����Z����B
			cookie.addMaxAge(expir);
			//�N�b�L�[�ɃA�N�Z�X�\��URL�p�X���w�肷��
			cookie.setPath("/");

			//�Z�b�g����cookie���o�͂���B
			cout << cookie << "\n";
			//response�ɃN�b�L�[��ǉ�����(��������0�̃N�b�L�[�ŏ㏑�����폜����)
			//retCookie.insert(make_pair(name, cookie));
		}

		//cookie���폜����B
		void deleteCookie(std::string key) {
			//�Z�b�g���邽�߂̃N�b�L�[���쐬����
			CgiCookie cookie(key, "");
			//�N�b�L�[�̐������Ԃ�0�w�肵�č폜����
			cookie.setMaxAge(-1);
			//�N�b�L�[�ɃA�N�Z�X�\��URL�p�X���w�肷��
			cookie.setPath("/");
			//response�ɃN�b�L�[��ǉ�����(��������0�̃N�b�L�[�ŏ㏑�����폜����)
			//retCookie.insert(make_pair(key, cookie));
			cout << cookie << "\n";
		}

		//session�����邩���`�F�b�N����B���Ƃ�sessionManager��邩���B
		bool checkSession() {
			
		}
};
/*
void CgiCookie::setExpires(const int& year, const int& month, const int& day,
	const int& hour, const int& min, const int& sec)
{
	tm st;
	st.tm_sec = sec;      // �b(0�`61) 
	st.tm_min = min;      // ��(0�`59) 
	st.tm_hour = hour;     // ��(0�`23) 
	st.tm_mday = day;     // ��(1�`31) 
	st.tm_mon = month-1;      // ��(1����0�Ƃ���) 
	st.tm_year = year-1900;     // �N(1900�N��0�Ƃ���) 
	m_expires = mktime(&st);
};
*/
	

//01-Jan-1970 00:00:00 GMT�`��
const string CgiCookie::createExpires() const
{
	if(maxAge == 0) return "";
	//
	tm ti = *gmtime(&maxAge);
	char str[30];
	strftime(str, 30, "%a, %d-%b-%Y %H:%M:%S GMT", &ti);
	return string(str);
};


ostream& operator << (ostream& os, const CgiCookie& cookie)
{
	ios_flags_saver ifs(os);//����̃X�g���[���̏����Ȃǂ�ۑ�
	typedef pair<const string, CgiSaveData> KeyPair;
	
	//�o�͂��� 
	os << "Set-Cookie: ";
	os << cookie.name + '=' + cookie.value + ';';
	const string expires = cookie.createExpires();
	if(!expires.empty()) os << " expires=" << expires << ";";
	if(!cookie.domain.empty()) os << " domain=" << cookie.domain << ";";
	if(!cookie.path.empty()) os << " path=" << cookie.path << ";";
	if(cookie.isSecure) os << " secure";
	
	return os;
};


void loadCookie(SaveDataMap& smap, const string& cookieStr)
{
	istringstream iss(cookieStr);
	loadData(smap, iss);
};


}; //namespace nana{

