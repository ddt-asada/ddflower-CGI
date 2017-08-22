#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <stdlib.h>

#include "mysql_connection.h"

#include "picojson.h"
#include "DB_ResultTree.h" //����N���X�B�����o�ϐ��̂ݎ��B
#include "Constants.h"		//����N���X�B�l�X�Ȓ萔�����B
#include "DbConnect.h"		//����N���X�BDB�ڑ��̊֐������B
#include "JSONDBManager.h"
#include "CgiCookieManager.h"
#include "CgiSaveDataManager.h"
#include "CgiSessionManager.h"
#include "CgiSession.h"

class account : public JSONDBManager {
public:
	picojson::object pageAuthorityCheck;
	
	//�N�b�L�[����p�̃I�u�W�F�N�g
	//cookie�𑀍삷�铮��͂��ׂĂ����ɓ�����B
	   nana::CgiCookieManager accountCookie;
	   //�Z�b�V��������p�̃I�u�W�F�N�g
	   //session����铮��͂��ׂĂ����ɓ�����B
	   nana::CgiSession accuntSession;

	   //�萔�N���X�B
	   Constants constants;

	   account() {

	   };

	   /* �N���X��:init
	   * �T�v:�������������s���B�������Ƃ��ăZ�b�V�����̊J�n��DB�ւ̐ڑ����s���B
	   * ����:����
	   * �߂�l:����
	   * �쐬��:2016.10.20
	   * �쐬��:R.Shibata
	   */
	   void init(){
		   
		   //�N�b�L�[�}�l�[�W���̃C���X�^���X���쐬����
		   //�Z�b�V���������݂��Ă��邩���`�F�b�N���ĂȂ���΍��B
		   if (accountCookie.getCookieValue("JSESSION") == "") {
			   accuntSession.load();
			   accountCookie.setCookie("JSESSION", accuntSession.getId(), constants.COOKIE_EXPIRATION);
		   }
		   //DB�ւ̐ڑ����J�n����
		   connect();
	   }


	   /* �N���X��:login
	   * �T�v:���O�C���������s���B
	   * ����:String jsonString: JSON������B���O�C����񂪓����Ă���K�v������B
	   * �߂�l:����
	   * �쐬��:2016.10.20
	   * �쐬��:R.Shibata
	   */
	   void login(std::string jsonString) {
		   //�N���C�A���g���瑗�M���ꂽJSON��������擾����
		   getJSONMap(jsonString);

		   //SQL�ɂ���O�Ώ��̂���rty�u���b�N�ň͂�
		   //json���o�͂���
		   try {
			   //json���o�͂���
			   createJSON(json, constants.EMPTY_STRING, NULL);
			   //SQL��O��chatch�u���b�N
		   }
		   catch (sql:: SQLException &e) {
			   // �G���[���b�Z�[�W��\������
			 //  e.printStackTrace();//system�G���[���b�Z�[�W���o�͂���
		   }

		   //�A�z�z���json�ɕϊ����ĕϐ��ɓ����
		   std::string jsonOut = Encode(json);
/*		   //�Z�b�V����ID�𔭍s����
			accountSession.load();

		   //���s����sessionID��ԋp�p��cookie�ɃZ�b�g����B
		   //
		   accountCookie.setCookie("JSESSIONID", accuntSession.getId(), constants.COOKIE_EXPIRATION);*/

		   //JSON�������ԍ������o��
		   std::string userId = getJsonValue(json, constants.STR_ID, constants.KEY_TEXT);
		   //JSON���猠�������o��
		   std::string authority = getJsonValue(json, constants.AUTHORITY, constants.KEY_TEXT);

		   //cookie�Ƀ��[�UID���Z�b�g����
		   accountCookie.setCookie(constants.USER_ID, userId, constants.COOKIE_EXPIRATION);

		   //cookie�Ƀ��[�U�̌������Z�b�g����
		   accountCookie.setCookie(constants.AUTHORITY, authority, constants.COOKIE_EXPIRATION);

		   //�����I��teacher�Ń��O�C���B
		   //std::string jsonOut = "{ \"db_getQuery\":\"SELECT id, login_id, authority FROM user_inf WHERE login_id='userName' AND password='password';\",\"userName\" : {\"value\":\"teacher\"},\"password\" : {\"value\":\"4a82cb6db537ef6c5b53d144854e146de79502e8\"},\"id\" : {\"text\":\"3\"},\"authority\" : {\"text\":\"10\"} }";

		   //�쐬����JSON��������o�͗p������փZ�b�g����
		   setOutHTMLString(jsonOut);
	   }

	   /* �N���X��:logout
	   * �T�v:���O�A�E�g�������s���B
	   * ����:����
	   * �߂�l:����
	   * �쐬��:2016.10.20
	   * �쐬��:R.Shibata
	   */
	   void logout() {
		   
		   //�Z�b�V�����N�b�L�[�����݂���Ȃ�
		   if (accountCookie.getCookieValue(constants.JSP_SESSION_COOKIE_NAME) != constants.EMPTY_STRING) {
			   //session�t�@�C�����폜���閽�߂�����\��B
			   //�Z�b�V�����N�b�L�[��j������
			   accountCookie.deleteCookie(constants.JSP_SESSION_COOKIE_NAME);
		   }

		   //���[�UID�A������cookie���폜����
		   accountCookie.deleteCookie(constants.JSP_SESSION_COOKIE_NAME);
		   accountCookie.deleteCookie(constants.USER_ID);
		   accountCookie.deleteCookie(constants.AUTHORITY);
	   }

	   /* �N���X��:loginCheck
	   * �T�v:���O�C���`�F�b�N���s���B
	   *    :�`�F�b�N���s���ALoginCheckException��throw����
	   * ����:����
	   * �߂�l:boolean:���O�C�����Ă��邩�ۂ��̐^���l��Ԃ�
	   * �쐬��:2016.10.20
	   * �쐬��:R.Shibata
	   */
	   bool loginCheck(){
		   bool retBoo = false; //�ԋp�l���i�[����ϐ���錾����

								   //�Z�b�V�����ϐ��̃��[�UID���Q�Ƃ��A�l�����݂��邩�ǂ������`�F�b�N����
								   //�܂��A�Z�b�V������cookie�ɕۑ�����Ă��郆�[�UID����v���邩���������߂�
	   if (accountCookie.getCookieValue("JSESSION") != "" && accountCookie.getCookieValue(constants.USER_ID) != ""
		   /*&& session.getSessionValue(Constants.USER_ID).equals(cookie.getCookieValue(Constants.USER_ID))*/) {
		   //�Z�b�V�����̗L�����Ԃ���������
		   //�������Ԃ񂢂�Ȃ��B
		   //session.extension();
		   //�ԋp�l�̕ϐ���true���i�[����
		   retBoo = true;
		   //�y�[�W�ɑ΂��錠���`�F�b�N���s��(�����������[�U�[)
		   pageCheck(accountCookie.getCookieValue(constants.AUTHORITY));
	   }
	   else {
		   //�y�[�W�ɑ΂��錠���`�F�b�N���s���i�Q�X�g���[�U�[�j
		   pageCheck("1");
	   }
	   //�^���l��Ԃ�
	   return retBoo;
	   }

	   /* �N���X��:pageCheck
	   * �T�v:�y�[�W�̌����`�F�b�N���s��
	   *    :�`�F�b�N���s���ALoginCheckException��throw����
	   * ����:String strAuthority:���[�U�[���������錠��������������(���e�͐��l�j
	   * �߂�l:����
	   * �쐬��:2016.10.20
	   * �쐬��:R.Shibata
	   */
	   void pageCheck(std::string strAuthority){
		   //������r�p�̕ϐ���錾���A�擾�������[�U�������Z�b�g����
		   int authority = stoi(strAuthority);

			//cookie�ɃZ�b�g����Ă���Ώۃy�[�W�̌������擾����(16�i��)
			//���͂�0xFF���̕�����̂��߁AparseInt�͎g�p�ł��Ȃ��Bdecode�̕Ԃ�l��Integer�^�̂��߁AintValue��int�ɕϊ�����
		   int pageAuth = std::stoi(accountCookie.getCookieValue(constants.PAGE_AUTH), nullptr, 16);

			//���Y���[�U�ŊJ����y�[�W�Ȃ̂����؂���
			if ((authority & pageAuth) == 0) {
				std::string outValue = "0";
				if (accountCookie.getCookieValue(constants.USER_ID) != "" && accountCookie.getCookieValue("JSESSIONID") == "") {
					//�^�C���A�E�g�Ƃ��āA�P���Z�b�g����B
					outValue = "1";
				}

				std::cout <<"\n"<< constants.ERROR_JSON_FRONT + outValue + constants.ERROR_JSON_BACK;
				exit(0);
			}
	   }
};