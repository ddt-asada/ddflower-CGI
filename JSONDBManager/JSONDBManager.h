	/* �֐����FcreateJSON
	 * �T�v:DB����f�[�^���擾����JSON�����
	 * ����:Map<String,Object> json:�J�����g��JSON
	 *     String:key:JSON�̃L�[
	 *     DBResultTree:dbrt_parent:DB����擾�����f�[�^���i�[���ăc���[�\������邽�߂̃N���X�̃C���X�^���X
	 * �߂�l:����
	 * �쐬��:2016.10.19
	 * �쐬��:R.Shibata
	 */
#pragma once

#include <iostream>
#include <fstream>//�e�X�g�p
#include <string>
#include <vector>
#include <sstream>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <boost/uuid/sha1.hpp>
#include <boost/cstdint.hpp>
#include <boost/array.hpp>

#include "mysql_connection.h"

#include "picojson.h"
#include "DB_ResultTree.h" //����N���X�B�����o�ϐ��̂ݎ��B
#include "Constants.h"		//����N���X�B�l�X�Ȓ萔�����B
#include "DbConnect.h"		//����N���X�BDB�ڑ��̊֐������B

//boost��SHA1�𗘗p���邽�߂̏����B
typedef boost::array<boost::uint8_t, 20> hash_data_t;

class JSONDBManager : public DbConnect {
public:int processedRecords = 0;
	   //JSON��Ԋ҂����A�z�z����i�[���郁���o�ϐ�
public:picojson::object json;
	   //HTML��ɏo�͂��邽�߂̍쐬����������
private:std::string outHTMLString = "";
		//�쐬����cookie���i�[���镶����B
		std::string outCookieString = "";

public:

	JSONDBManager() {

	};
	//�G���[��������ŒǋL�B
	void createJSON(picojson::object& json, std::string key, DB_ResultTree* dbrt_parent) {

		std::string keyString;		//JSON�̃L�[�l���i�[���镶����B
		// DB�̌��ʂ���\�z�����c���[���\������N���X�̃C���X�^���X���쐬����
		//4�̃t�B�[���h�����N���X�B���Ƃō��B
		DB_ResultTree db_resultTree;
		//�X�e�[�g�����g���쐬����
		//DB���猋�ʃZ�b�g���擾����֐��B    //���ƂŕύX�B
		db_resultTree.db_result = executeQuery(json, constants.DB_GETQUERY);
		//DB_ResultTree�̐e�q�֌W���\�z����
		db_resultTree.parent = dbrt_parent;
		//�J�����g��JSON��ۑ�����
		db_resultTree.json = json;
		//�J�����g�̃L�[��ۑ�����
		db_resultTree.keyData = key;
		// db_resultTree����"key"�ɊY������f�[�^���擾����
		std::string column = getDBColumn(key, &db_resultTree);

		//json�ɂ��čŉ��w�̗v�f�ɂ��ǂ蒅���܂Ń��[�v���ăf�[�^������������B
		//�e�X�g����
		for (picojson::object::iterator itr = json.begin(); itr != json.end(); ++itr) {
			//json�̃L�[�l�����o���B
			keyString = itr->first;
			//JSON�Ɏq��������Ƃ��B
			//���ƂŒ萔�ɒu��������B
			if (itr->second.to_str() == "object")
			{
				//�ċA����
				createJSON(itr->second.get<picojson::object>(), keyString, &db_resultTree);
			}
			//���o���Ă������L�[��"text"�܂���"html"�܂���"src"�ł���΁B
			//���ƂŒ萔�ɒu�������B
			else if (column != "" && (keyString == "text" || keyString == "html" || keyString == "src"))
			{
				//�㏑��
				json[keyString] = (picojson::value)column;
			}
		}
	}

	/* �֐����FexecuteQuery
	* �T�v:�N�G�������s����DB���猋�ʃZ�b�g���擾����
	* ����:Map<String,Object> json:�J�����g��JSON�A�z�z��
	*     String:queryKey:���s����N�G���̃x�[�X�ƂȂ镶����
	* �߂�l:ResultSet retRs:DB����擾�������ʃZ�b�g��Ԃ�
	* �쐬��:2016.10.19
	* �쐬��:R.Shibata
	* �X�V��:2017.8.15
	* �X�V��:K.Asada
	* �X�V���e:C++�ɑΉ�����悤�Ɉ����Ȃ�java���L�̂��̂�ύX�B
	*/

	//���Ƃŗ�O������ǋL����B
	//�e�X�g�����B
	//�p�X�֌W�̂݌���ǋL�\��B
	sql::ResultSet* executeQuery(picojson::object& json, std::string queryKey) {
		//�ԋp���錋�ʃZ�b�g�̕ϐ����쐬����
		int tmp = 0;
		std::string query;			//�L�[���ɑΉ������l���i�[���镶����B
		std::string replaceValue;	//�u���Ώۂ̕�������i�[���镶����B
		std::string passString;		//�p�X���[�h�̕R�Â�����������i�[���镶����B
		picojson::object obj;		//�N�G����u������ۂ̉��̒u����B
		sql::Statement *stmt;		//�X�e�[�g�����g���i�[����B
		sql::ResultSet *res = NULL;		//Connecter/C++�Ŏg���B

		//���[�U����ی삷�邽�߂Ƀp�X���[�h�����݂��Ă��邩�𔻒肷��B
		auto itr = json.find(constants.STR_PASSWORD);
		//�l���擾�ł��Ă�����B
		if (itr != json.end()) {
			obj = itr->second.get<picojson::object>();
			//�p�X���[�h�ɓ����镶������n�b�V�������Ajson�̒l��u������i���u���j
			ChangeJSON(json, constants.STR_PASSWORD, changeToSHA1(obj["value"].get<std::string>()));
			//�����܂łňÍ����ł��Ă�O�̂��ߎc���Ă܂��B
		}

		// queryKey�ɑΉ�����l���擾����B
		auto it = json.find(queryKey);
		//query���������擾�ł��Ă���΁B
		if (it != json.end()) {
			query = CatchJSON(json, queryKey);
			//query���������擾�ł��Ă���΁B
			if (query.length() >= 1) {
				//json�ɂ��čŉ��w�̗v�f�ɂ��ǂ蒅���܂Ń��[�v���ăf�[�^�����o��
				for (picojson::object::iterator itr = json.begin(); itr != json.end(); ++itr) {
					//value�Ɏq��������Ƃ��̏���(linkedHashMap�̎�)
					if (itr->second.to_str() == "object") {
						//�q�I�u�W�F�N�g���擾����
						obj = itr->second.get<picojson::object>();
						//�q�I�u�W�F�N�g��value�������Ă�����
						if (obj["value"].is<std::string>() || obj["value"].is<picojson::array>()) {
							//�u���Ώۂ̕�������A�擾�����q�I�u�W�F�N�g��value���쐬����
							replaceValue = createReplaceValue(obj);
							//�q�I�u�W�F�N�g��key������ƈ�v����query�̕������u������
							//���ƂŊ֐����B
							std::string findKey = itr->first;
							//�u���Ώۂ̕�������擾�ł��Ă�����B
							if (query.find("'" + findKey + "'") != std::string::npos) {
								//�u���Ώۂ̃L�[�����擾�B
								query.replace(query.find("'" + findKey + "'") + 1, findKey.length(), replaceValue);
							}
						}
					}
				}
				try {
					//���s�ł��Ȃ��p�^�[���̃N�G�����C������
					query = queryCorrection(query);
					fileout(query);
					//�X�e�[�g�����g���쐬����
					stmt = con->createStatement();
					
					//�N�G�������s�����ʃZ�b�g��Ԃ��N�G���ł���΁B
					if (stmt->execute(query)) {
						res = stmt->getResultSet();
						//�s�����擾���邽�߁A���R�[�h�Z�b�g�̈ʒu��last�ɂ���
						res->last();
						//�������s�������R�[�h�������ʃZ�b�g���擾���ă����o�ɕۑ�����
						processedRecords = res->getRow();
						//�s�����擾���邽�߂�last�ɂ����J�[�\���ʒu��擪�ɖ߂��B
						res->beforeFirst();
					}
					else {
						//���ʂ�Ԃ��Ȃ��ꍇ�͍X�V�����s�����擾����B
						processedRecords = stmt->getUpdateCount();
					}
				}
				catch (sql::SQLException &e) {
					std::cout << "# ERR: SQLException in " << __FILE__;
					std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
					std::cout << "# ERR: " << e.what();
					std::cout << " (MySQL error code: " << e.getErrorCode();
					std::cout << ", SQLState: " << e.getSQLState() << " )" << endl;
					
				}
			}
		}
		//���ʃZ�b�g��Ԃ�
		return res;
	}

	/* �֐����FcreateReplaceValue
	* �T�v:�N���C�A���g���󂯎�����u���Ώۂ̒l���A�u���\������ɕϊ�����
	* ����:Object childObjectValue:�N���C�A���g���󂯎�����u���Ώۂ̒l�AString��ArrayList�����݂���
	* �߂�l:String:�쐬�����ԋp�p�̕�����
	* �쐬��:2016.10.28
	* �쐬��:R.Shibata
	*/

	//�̂��قǒ������s���B
	std::string createReplaceValue(picojson::object childObjectValue) {
		//�󂯎�����I�u�W�F�N�g�ɂ��A�ԋp���镶������쐬���邽�߂̕ϐ���錾����
		std::string retReplaceString = "";
		//�����p�̕�����̊i�[�ꏊ�𐶐��B
		std::string childObjectstring = "";
		
		int i = 0;
		//�f�[�^�쐬�̂��߂̕�����z���p�ӂ���
		std::string childObjectArray[10] = { "" };
		//picojson�̃����o�֐��𗘗p�ł���悤�Ɉ�x�I�u�W�F�N�g��ϊ��B
		//�z�񂩂當��������o���B
		if (childObjectValue["value"].is<picojson::array>()) {
			//picojson�̔z��ɕϊ��B
			picojson::array&tmp = childObjectValue["value"].get<picojson::array>();
			//�����p������z��ɑS�đ������
			for (picojson::array::iterator itr = tmp.begin(); itr != tmp.end(); itr++) {
				//�������z��Ɋi�[���Ă����B
				childObjectArray[i++] = itr->get<std::string>();
			}
			//�z��ȊO�ł����
		}
		else {
			//�����p������z��ɕ�����Ƃ��Ď擾
			childObjectArray[0] = (childObjectValue["value"].get<std::string>());
		}
		//�擾�A�쐬�����z��𑖍�����
		for (i = 0; childObjectArray[i] != ""; i++) {
			//�����p�̕�����z�񂩂當������擾�B
			childObjectstring = childObjectArray[i];
			//�u�������񂪋󔒂ł���Ή������Ȃ��A�l������΋�؂蕶����t�^����
			retReplaceString += retReplaceString == "" ? "" : "','";
			//�z��̕�������A�G�X�P�[�v�������s���u��������ɕt�^����
			retReplaceString += escapeSQLValue(childObjectstring);
		}
		//�쐬�����������ԋp����
		return retReplaceString;
	}

	/* �֐����FgetDBColumn
	* �T�v:�w�肵��key(��)�̒l�����ʃZ�b�g����擾���ĕԂ�
	* ����:String key:JSON�̃I�u�W�F�N�g�̃L�[
	*     DBResultTree dbrTree:DB����擾�������ʂ��c���[�\���ɂ���N���X�̃C���X�^���X
	* �߂�l:String column:�擾������̒l��Ԃ�
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/

	//��O��������Œǉ��B
	std::string getDBColumn(std::string key, DB_ResultTree *dbrTree) {
		//�ԋp�l���i�[����ϐ�������������
		std::string column = "";
		//�擾�Ώۂ���̉��s�ڂ����Z�b�g����
		int columnNumber = 0;
		//dbrTree�̐e�̃L�[���A���ꂪ�z��̗v�f�ł���Ƃ������������������܂�ł����
		//���ƂŒ萔�ɒu��������B
		if (dbrTree->parent != NULL && dbrTree->parent->keyData.find("__") != -1) {
			//key�̒l�𕪊�����
			//���ƂŒ萔�ɒu��������B
			std::string keyString[] = { (dbrTree->parent->keyData, "__") };
			//�s�����Z�b�g����
			columnNumber = std::stoi(keyString[1]);
		}

		//�e�������Ȃ�܂�DB���R�[�h�c���[�𑖍�����
		while (dbrTree != NULL) {
			//dbtTree�Ɍ��ʃZ�b�g���o�^����Ă����
			if (checkColumn(dbrTree->db_result, key)) {
				//colNumber�̈ʒu�փ��R�[�h�Z�b�g���ړ�������i�Y������1�J�n�̂���+1�j
				if (dbrTree->db_result->absolute(columnNumber + 1)) {
					//�J�����̒l���擾����
					column = dbrTree->db_result->getString(key);
				}
				//���[�v�𔲂���
				break;
			}
			else {
				//�e���Z�b�g����
				dbrTree = dbrTree->parent;
			}
		}
		//column��Ԃ�
		return column;
	}

	/* �֐����FgetListJSON
	* �T�v:���X�g�`����JSON���쐬���ĕԂ�
	* ����:Map<String, Object> json:JSON�̃I�u�W�F�N�g�B
	* �߂�l:String strAll:JSON�̕�����z��𕶎���ŕԂ�
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/

	//�e�X�g�����B
	//���Ƃŗ�O�����ǉ��B
	std::string getListJSON(picojson::object json) {
		//�ԋp���镶������쐬���邽�߂̕ϐ���3�錾�A����������B
		std::string strAll = "";
		std::string strBlock = "";
		std::string strLine = "";
		try {
			//�f�[�^�x�[�X���瓖�Y���R�[�h�Q���擾����(���ʃZ�b�g���擾����)
			sql::ResultSet* rs = executeQuery(json, constants.DB_GETQUERY);
			//���R�[�h�Z�b�g�̗񖼂��擾����A���Ƃł�����B
			sql::ResultSetMetaData* rsMeta = rs->getMetaData();
			//���R�[�h�Z�b�g�̗񐔂��擾����A���Ƃł�����B
			int columnCount = rsMeta->getColumnCount();
			//���ʃZ�b�g�̍s�ɂ��Ẵ��[�v
			while (rs->next()) {
				//���R�[�h�̕����������������
				strLine = "";
				//��ɂ��Ẵ��[�v
				for (int i = 0; i < columnCount; i++) {
					//�񖼂��擾����(�ʖ��̎擾)
					std::string sColName = rsMeta->getColumnLabel(i + 1);
					//��̒l���擾����
					std::string value = rs->getString(i + 1);
					//��̒l��NULL�������ꍇ�A�u�����N�ɒu������
					value = value == constants.EMPTY_STRING ? constants.EMPTY_STRING : value;
					//������̍s�P�ʂ̕ϐ�����łȂ����A�s�̕�������J���}�ŋ�؂�
					strLine += strLine == constants.EMPTY_STRING ? constants.EMPTY_STRING : ",";

					//�擾�����l�ɁAJSON�p�̃G�X�P�[�v�������s��
					value = escapeJSONValue(value);

					//1�s���̃f�[�^�𕶎���ɒǉ�����
					strLine += "\"" + sColName + "\":\"" + value + "\"";
				}
				//�s�ɕ����񂪓����Ă�����A�J���}�ŋ�؂�
				strBlock += strBlock == constants.EMPTY_STRING ? constants.EMPTY_STRING : ",";
				//�쐬�����s�̕�������u���b�N�̕�����ɒǉ�����
				strBlock += "{" + strLine + "}";

			}
		}
		catch (std::exception) {

			throw;
		}
		//�쐬�����S�u���b�N��z��̊��ʂň͂�
		strAll = "[" + strBlock + "]";

		//�쐬�����������Ԃ�
		return strAll;
	}

	/* �֐����FoutputJSON
	* �T�v:DB����擾�������R�[�h��JSON�����B
	* ����:String jsonString:�N���C�A���g����󂯎����JSON����
	*    :String key:JSON�̃g�b�v�̃m�[�h�̃L�[�B
	* �߂�l:����
	* �쐬��:2016.10.24
	* �쐬��:R.Shibata
	*/

	void outputJSON(std::string jsonString, std::string key) {
		//������JSON�������ϊ����āAJSON�̘A�z�z����擾���ăN���X�̃����o�Ɋi�[����
		getJSONMap(jsonString);
		//��O�ɔ�����
		/*try*/ {
			//�f�[�^�x�[�X�ɐڑ�����
			connect();
			//JSON������̍쐬���s��
			createJSON(json, key, NULL);
			//�f�[�^�x�[�X����ؒf����
			//���Ƃō��B
			disconnect();
		}/*
		catch (ClassNotFoundException | SQLException | NoSuchAlgorithmException e) {
			//�G���[���b�Z�[�W��\������
			e.printStackTrace();
			//�v���O�������~�߂�
			return;
		}*/
	}

	/* �֐����FgetJSONMap
	* �T�v:JSON�����񂩂�A�z�z��𐶐�����B
	* ����:String jsonString:�ϊ�����JSON������
	* �߂�l:����
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/

	//�e�X�g�����B
	void getJSONMap(std::string jsonString) {
		// JSON�������A�z�z��ɕϊ����邽�߂Ƀp�[�X����B
		picojson::value v;
		parse(v, jsonString);
		//picojson�̃I�u�W�F�N�g�ɕϊ����邱�Ƃɂ��A�z�z��Ƃ��Ĉ�����悤�ɂ���B
		picojson::object map = v.get<picojson::object>();
		// Map�ɕϊ����ꂽJSON��JSONDBManager�N���X�̃����o�Ɋi�[����
		json = map;
	}

	/* �֐����FcheckColumn
	* �T�v:���ʃZ�b�g�Ɏw�肵���񖼂����񂪂��邩���`�F�b�N����
	* ����:ResultSet rs:�w�肵���񂪂��邩���`�F�b�N����Ώۂ̌��ʃZ�b�g
	*    :String columnName:�`�F�b�N�Ώۂ̗�
	* �߂�l:����
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/

	//��O������Œǉ�
	bool checkColumn(sql::ResultSet* rs, std::string columnName) {
		bool retBoo = false;
		//���ʃZ�b�g��null�łȂ����̏���
		if (rs != NULL) {
			//�񖼂��擾���邽�߁AMetaData���擾����
			sql::ResultSetMetaData* rsMeta = rs->getMetaData();
			//MetaData���񐔂��擾����
			int columnCount = rsMeta->getColumnCount();
			// �ŏ��̌��ʃZ�b�g�����𑖍�����
			for (int i = 0; i < columnCount; i++) {
				//���ʃZ�b�g�̗�Ɏw�肵���񖼂̗񂪑��݂���(�Y�����J�n�ʒu���P�̂���+1)
				if (rsMeta->getColumnLabel(i + 1) == columnName) {
					// �Ԃ��ϐ���true���i�[����
					retBoo = true;
					//�`�F�b�N�����ƂȂ�A���[�v���I������
					break;
				}
			}
		}
		//�����Ԃ�
		return retBoo;
	}

	/* �֐����FgetListJSONPlusKey
	* �T�v:getListJSON�ō쐬�����z����A�N���C�A���g���瑗�M���ꂽJSON�Ɋi�[���ĕ�����ŕԂ��B
	* ����:Object json:JSON�̃I�u�W�F�N�g�B
	*    :String key:�L�[��
	* �߂�l:String:�I�u�W�F�N�g�ň͂񂾔z���JSON�������Ԃ�
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/

	//�e�X�g�����B
	//��O�����͌�Œǉ��B
	std::string getListJSONPlusKey(picojson::object json, std::string key) {
		//tsbleData��␳���邽�߂̕�����B
		//picojson�ɂ�镶����̃\�[�g������邽�߁B
		//getListJSON�Ńe�[�u���p��JSON�z����쐬����
		std::string retArray = ",\"tableData\":" + getListJSON(json) + '}';
		std::string	retStr = "";
		//JSON�z��̕������z��f�[�^�ɕϊ����A������JSON�ɒǉ�����
		//�L�[���ɑΉ�����l��JSON�̒��ɑ��݂��Ă��邩���m�F�B
		auto itr = json.find(key);
		if (itr != json.end()) {
			//���݂��Ă���ꍇ�͍폜����B
			json.erase(itr);
		}
			retStr = Encode(json);
			retStr.pop_back();

		//�ǉ����s����������JSON�𕶎���ɕϊ�����
		//�쐬�����������Ԃ�
		return retStr + retArray;
	}

	/* �֐����FqueryCorrection
	* �T�v:Java�ł͎��s�ł��Ȃ��^�C�v�̃N�G���[���C�����ĕԋp����
	*    :�{���ł���΂���ȏ����͕s�v�̂͂�
	* ����:String query:�C���Ώۂ̃N�G���[
	* �߂�l:String:�C����̃N�G���[��ԋp����
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/
	//���邩�킩��Ȃ����ǂƂ肠�����u���Ă����B
	std::string queryCorrection(std::string query) {
		//�ԋp�p�N�G���[�̕������錾���A���͂̃N�G���[���Z�b�g����
		std::string retQuery = query;
		//�N�G���[�̒��ɃZ�~�R�������܂܂�Ă���ꍇ
		if (retQuery.find(";") != -1) {
			//�N�G���̍ŏ���;��������index��ێ�����B;���o�͑Ώۂ̂��߁A���l��+1����
			int index = retQuery.find(";") + 1;
			//�N�G���[�����񂩂�ŏ���";"�܂ł�؂�o��
			retQuery = retQuery.substr(0, index);
		}
		//�C�������N�G���[��ԋp����
		return retQuery;
	}

	/* �֐����FescapeJSONValue
	* �T�v:�����Ŏ󂯎�����������JSON�p�̃G�X�P�[�v�������s���ԋp����
	* ����:String str:�G�X�P�[�v���镶����
	* �߂�l:String:�G�X�P�[�v����������
	* �쐬��:2016.10.27
	* �쐬��:R.Shibata
	*/
	std::string escapeJSONValue(std::string str) {
		//�ԋp�p������ɁA�����̕�������Z�b�g����
		std::string retStr = str;
		//���}�[�N���G�X�P�[�v�����ɒu��������(replaceAll�͐��K�\���Ƃ��Ĉ����邽�߁A�����\4���ƂȂ�j
		//�������u��������֐����Ăяo���B
		retStr = Replace(retStr, "\\", "\\\\");
		//���s�������G�X�P�[�v�����ɒu��������
		retStr = Replace(retStr, "\r\n", "\\n");
		retStr = Replace(retStr, "\r", "\\n");
		retStr = Replace(retStr, "\n", "\\n");
		//�_�u���N�H�[�g���G�X�P�[�v�����ɒu��������
		retStr = Replace(retStr, "\"", "\\\"");
		//�G�X�P�[�v�����l��ԋp����
		return retStr;
	}

	/* �֐����FescapeSQLValue
	* �T�v:�����Ŏ󂯎�����������SQL�p�̃G�X�P�[�v�������s���ԋp����
	* ����:String str:�G�X�P�[�v���镶����
	* �߂�l:String:�G�X�P�[�v����������
	* �쐬��:2016.10.27
	* �쐬��:R.Shibata
	*/
	std::string escapeSQLValue(std::string str) {
		//�ԋp�p������ɁA�����̕�������Z�b�g����
		std::string retStr = str;
		//SQL���s���폜����邽�߁��}�[�N���G�X�P�[�v����
		retStr = Replace(retStr, "\\", "\\\\\\");
		//SQL���s�ł��Ȃ��Ȃ邽�߁A�V���O���N�H�[�g���G�X�P�[�v����
		retStr = Replace(retStr, "'", "\\'");
		return retStr;
	}

	/* �֐����FgetMapValue
	* �T�v:�w�肵��map�̈���̊K�w�̃f�[�^���擾����
	* ����:Map<String, Object> map:�l���擾������map�I�u�W�F�N�g
	*    :String jsonKey:map����擾�������l�̃L�[
	*    :String vauleKey:map��key�Ɋ܂܂��l�������L�[
	* �߂�l:String:�擾����������
	* �쐬��:2016.10.27
	* �쐬��:R.Shibata
	*/
	std::string getJsonValue(picojson::object map, std::string jsonKey, std::string valueKey) {
		//������map���牺�ʂ̃I�u�W�F�N�g���擾����
		picojson::object tempObject = map[jsonKey].get< picojson::object>();
		//���ʃI�u�W�F�N�g���f�[�^���擾���A���̒l��ԋp����
		return tempObject[valueKey].get<std::string>();
	}

	/* �֐����FgetOutHTMLString
	* �T�v:�N���C�A���g�ɏo�͂���p�̕�����ł���outHTMLString��ԋp����
	* ����:����
	* �߂�l:String:outHTMLString
	* �쐬��:2016.10.25
	* �쐬��:R.Shibata
	*/

	//�e�X�g�����B
	std::string getOutHTMLString() {
		//outHTMLString��ԋp����
		return outHTMLString;
	}

	/* �֐����FsetOutHTMLString
	* �T�v:�N���C�A���g�ɏo�͂���p�̕�����ł���outHTMLString��ݒ肷��B
	* ����:String outHTMLString:�ݒ肷�镶����
	* �߂�l:����
	* �쐬��:2016.10.25
	* �쐬��:R.Shibata
	*/

	//�e�X�g�����B
	void setOutHTMLString(std::string setoutHTMLString) {
		//���͂̒l��ݒ肷��
		outHTMLString = setoutHTMLString;
	}

	//�쐬����cookie���擾����B
	std::string getCookieString() {
		//outHTMLString��ԋp����
		return outCookieString;
	}

	//�쐬����cookie��A������N���X�B
	void setOutCookieString(std::string setoutCookieString) {
		//���͂̒l��ݒ肷��
		outCookieString += setoutCookieString;
	}

	//java��replaceAll�ɑ��镶�����u��������֐��B
	std::string Replace(std::string rep, std::string target, std::string change) {
		for (int i = 0; i < rep.length();) {
			//�u�������Ώۂ̕������T���B
			i = rep.find(target, i);
			//�u�������Ώۂ̕���������������B
			if (i <= rep.length()) {
				//�������u��������B
				rep.replace(i, target.length(), change);
				i += change.length();
			}
		}

		//�u��������̕������Ԃ��B
		return rep;
	}

	//JSON���𑖍����p�X���[�h��Ԃ��֐��B
	bool SearchJSON(picojson::object& json, std::string key) {
		bool judge = false;		//���茋�ʂ��i�[����B

								//json�ɂ��čŉ��w�̗v�f�ɂ��ǂ蒅���܂Ń��[�v���ăf�[�^�����o���B
		for (picojson::object::iterator itr = json.begin(); itr != json.end(); ++itr) {
			//���łɃL�[���������ď��������Ă����ꍇ�B
			if (judge) {
				//���[�v�𔲂���B
				break;
			}
			else if (itr->second.to_str() == "object")
			{
				//�ċA����
				judge = SearchJSON(itr->second.get<picojson::object>(), key);
			}
			//�L�[���ɑΉ������l���������Ƃ��B
			else if (itr->first == "src")
			{
				//�����������I��������Ƃ��i�[����B
				if (json["src"].get<std::string>() == key) {
					judge = true;
					//���[�v�𔲂���B
					break;
				}
			}
		}

		//���茋�ʂ�Ԃ��B
		return judge;
	}

	//JSON���𑖍����p�X���[�h��Ԃ��֐��B
	bool ChangeJSON(picojson::object& json, std::string key, std::string change) {
		bool judge = false;		//���茋�ʂ��i�[����B

								//json�ɂ��čŉ��w�̗v�f�ɂ��ǂ蒅���܂Ń��[�v���ăf�[�^�����o���B
		for (picojson::object::iterator itr = json.begin(); itr != json.end(); ++itr) {
			//���łɃL�[���������ď��������Ă����ꍇ�B
			if (judge) {
				//���[�v�𔲂���B
				break;
			}
			//�L�[���ɑΉ������l���������Ƃ��B
			else if (itr->first == key)
			{
				//1�K�w���Ƀp�X���[�h��value������ꍇ����������B
				itr->second.get<picojson::object>()["value"] = (picojson::value)change;
				//�����������I��������Ƃ��i�[����B
				judge = true;
				//���[�v�𔲂���B
				break;
			}
			//�q�����݂��Ă���΁B
			else if (itr->second.to_str() == "object")
			{
				//�ċA����
				judge = ChangeJSON(itr->second.get<picojson::object>(), key, change);
			}
		}

		//���茋�ʂ�Ԃ��B
		return judge;
	}

	//�e�X�g�����B
	//JSON���𑖍����A�L�[���ɑΉ������l��Ԃ��֐��B
	std::string CatchJSON(picojson::object& json, std::string key) {
		std::string result = "roop";	//���o�����l���i�[���镶����B�����l�Ƃ��Ďg��Ȃ��ł��낤���������Ă����B

		//json�ɂ��čŉ��w�̗v�f�ɂ��ǂ蒅���܂Ń��[�v���ăf�[�^�����o���B
		for (picojson::object::iterator itr = json.begin(); itr != json.end(); ++itr) {
			//���łɒl�����o���Ă����ꍇ�B
			if (result != "roop") {
				//���[�v�𔲂���B
				break;
			}
			//�L�[���ɑΉ������l���������Ƃ��B
			else if (itr->first == key)
			{
				//�q�v�f���c���Ă���ꍇ�B
				//���̕����͕ς���\�������B
				if (itr->second.to_str() == "object") {
					//��������p�[�X������Ԃɕϊ�����B
					picojson::value v(itr->second);
					//JSON�̃l�X�g�\���ɐ��^���ĕԂ��B
					result = v.serialize();
					//���[�v�𔲂���B
					break;
				}
				else if (json[key].is<std::string>()) {
					//�l�����o���B
					result = json[key].get<std::string>();
					break;
				}
			}
			//���o���Ă������L�[��"text"�܂���"html"�܂���"src"�ł���΁B
			//���ƂŒ萔�ɒu�������B
			else if (itr->second.to_str() == "object")
			{
				//�ċA����
				result = CatchJSON(itr->second.get<picojson::object>(), key);
			}
		}

		//���o�����l��Ԃ��B
		return result;
	}

	//�e�X�g�����B
	//java��JSON.encode�ɓ�����֐��B
	std::string Encode(picojson::object& json) {
		//JSON���當����ɕϊ����ĕԂ��B
		return picojson::value(json).serialize();
	}

	//�e�X�g�����B
	//java��JSON.decode�ɓ�����֐��B
	picojson::value Decode(std::string json) {
		picojson::value v;
		//��������p�[�X����B
		parse(v, json);
		//�������JSON�ɕϊ��B
		return v;
	}

	//java�̕����񕪊��֐�split�ɓ�������́B����B
	std::vector<std::string> split(const std::string &str, char sep)
	{
		std::vector<std::string> v;
		std::stringstream ss(str);
		std::string buffer;
		while (std::getline(ss, buffer, sep)) {
			v.push_back(buffer);
		}
		return v;
	}

	//�󂯎�����������SHA1 �ɕϊ����ĕԋp����֐��B
	typedef boost::array<boost::uint8_t, 20> hash_data_t;

	std::string changeToSHA1(std::string change) {
		std::stringstream ss;
		std::cout << std::hex;
		std::string tmp;
		const char* changechar = change.c_str();

		hash_data_t hash = get_sha1_hash(changechar, change.length());
		hash_data_t::const_iterator itr = hash.begin();
		const hash_data_t::const_iterator end_itr = hash.end();
		for (; itr != end_itr; ++itr)
		{
			ss << std::hex << ((*itr & 0xf0) >> 4)
				<< (*itr & 0x0f);

			tmp = ss.str();
		}

		return tmp;
	}

	//SHA1�ϊ��̓��������B

	hash_data_t get_sha1_hash(const void *data, const std::size_t byte_count)
	{
		boost::uuids::detail::sha1 sha1;
		sha1.process_bytes(data, byte_count);
		unsigned int digest[5];
		sha1.get_digest(digest);
		const boost::uint8_t *p_digest = reinterpret_cast<const boost::uint8_t *>(digest);
		hash_data_t hash_data;
		for (int i = 0; i < 5; ++i)
		{
			hash_data[i * 4] = p_digest[i * 4 + 3];
			hash_data[i * 4 + 1] = p_digest[i * 4 + 2];
			hash_data[i * 4 + 2] = p_digest[i * 4 + 1];
			hash_data[i * 4 + 3] = p_digest[i * 4];
		}
		return hash_data;
	}

	/*�f�o�b�O�p*/
	void fileout(string str) {
		string filename = "debug";
		ofstream ofs;
		ofs.open(filename, ios::app);
		ofs << str << "\n\n";
		ofs.close();
	}
};
/*
//�e�X�g�p��main
int main() {
	std::ifstream ifs("Text.txt");	//JSON���i�[���ꂽ�t�@�C�����J���A���ۂ̓���̎��̓N���C�A���g���JSON���󂯎��B
	std::string err;				//�p�[�X�����ۂ̃G���[���i�[���镶����B
	std::string json;				//�J�����t�@�C���̒��g���i�[���镶����B�p�[�X�ɗ��p����B
	picojson::value val;			//�p�[�X���JSON���i�[����ϐ��B�B
	picojson::object query;				//key���ɑΉ�����l���i�[���镶����
	JSONDBManager DB;

	while (!ifs.eof()) {
		std::string work = "";
		ifs >> work;
		json += work;
	}
	ifs.close();

	err = parse(val, json);
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	//JSON�z����I�u�W�F�N�g�ɕϊ�����B
	picojson::object& obj = val.get<picojson::object>();
	DB.createJSON(obj, "test", NULL);

	std::cout << ((picojson::value)obj).serialize(true) << '\n';

	return 0;
}
*/