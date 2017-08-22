#pragma once

#include <iostream>

#include "picojson.h"
#include "mysql_connection.h"

/*
* �N���X��:DB_ResultTree
* �T�v  :DB�̌��ʃZ�b�g�̃c���[�̃m�[�h�N���X
* �쐬��:R.Shibata
* �쐬��:2016.10.19
*/

//���ƂŎ蒼���B
class DB_ResultTree {
public:
	DB_ResultTree* parent;		//���̃m�[�h�i�C���X�^���X�j�̐e
	picojson::object json;	//JSON�f�[�^�̘A�z�z��
	std::string keyData;				//�����o��json�L�[
	sql::ResultSet* db_result;			//DB�̌��ʃZ�b�g
};