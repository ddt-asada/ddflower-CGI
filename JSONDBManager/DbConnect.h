#pragma once

#include <stdlib.h>
#include <iostream>

#include "Constants.h"

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class DbConnect : public Constants
{
public:
	sql::Connection *con;
	Constants constants;

	DbConnect() {

	};

	/* �֐���:connect
	* �T�v:DB�Ƃ̐ڑ����s���B
	* ����:����
	* �߂�l:����
	* �쐬��:2016.10.19
	* �쐬��:R.Shibata
	*/
	void connect(){
		sql::Driver *driver;
		sql::Statement *stmt;
/*		sql::ConnectOptionsMap connection_properties;

		sql::SQLString hostName(DSN);
		sql::SQLString userName(DB_USER);
		sql::SQLString password(DB_PASSWORD);
		sql::SQLString charset("utf8");

		connection_properties["hostName"] = hostName;
		connection_properties["userName"] = userName;
		connection_properties["password"] = password;
		connection_properties["OPT_CHARSET_NAME"] = charset;*/

		driver = get_driver_instance();

		//�ڑ����s���B
		con = driver->connect(constants.DSN, constants.DB_USER, constants.DB_PASSWORD);

		//�X�e�[�g�����g���쐬�B
	//	stmt = con->createStatement();

		//UTF8��ݒ�B
		//stmt->executeQuery("SET NAMES utf8");

		//�X�e�[�g�����g���I������B
	//	stmt->close();
	}

	/* �֐���:disconnect
	* �T�v:DB�Ƃ̐ڑ������B
	* ����:����
	* �߂�l:����
	* �쐬��:2016.10.19
	* �쐬��:R.Shibata
	*/
	void disconnect() {
		
		con->close();
		/*
		//�ؒf���s��SQL�G���[���L���b�`����try�u���b�N
		try {
			//DB�Ƃ̐ڑ����悶��
			con->close();
			//SQL�G���[�����������ꍇ
		}
		catch (SQLException e) {
			//�G���[���b�Z�[�W��\������
			e.printStackTrace();
		}*/
	}
};