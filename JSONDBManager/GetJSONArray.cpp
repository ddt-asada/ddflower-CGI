#include "procedureGetList.h"
#include "Request.h"
#include "JSONtest.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <direct.h>

#include "boost/io/ios_state.hpp"

#include "CgiSaveDataManager.h"
#include "CgiCookieManager.h"
#include "CgiSessionManager.h"


/*
* �t�@�C����:jsp/GetJSONArray.jsp
* �T�v	:�e�[�u���̃^�O�쐬�p��JSON�z����쐬���ĕԂ��B
* �쐬��:R.Shibata
* �쐬��:2016.10.24
*/


int main() {
	Constants constants;
	//�e�X�g�p
	JSONDBManager test;

	//std::string json = testString();
	
	const char* answer = "Content-Type:text/html;charset=utf-8;\n";

	std::cout << answer;
	
	//HTTP���POST���\�b�h�Ńf�[�^���󂯎��N���X�̃C���X�^���X���B
	ServerRequest request;
	
	//HTTP���POST���\�b�h�ő���ꂽ�f�[�^���󂯎��B
	std::string json = request.HTTPRequest();
	
	//�N���C�A���g�֕ԋp���邽�߂̕������錾����
	std::string outValue = "";
	//���X�g�`����JSON�����N���X�̃C���X�^���X���쐬����
	procedureGetList listJsonDbGetter;
	
	//���������C���X�^���X�̏����֐������s����
	listJsonDbGetter.run(json);
	//�쐬�����������\�����N���C�A���g�֕ԋp����
	outValue = listJsonDbGetter.getOutHTMLString();

	//�쐬�����������\�����N���C�A���g�֕ԋp����
	std::cout <<'\n' << outValue;
	
	
	return 0;
}