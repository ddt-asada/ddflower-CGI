#pragma once

#include "procedureBase.h"

/* �N���X��:procedureGetList
* �T�v:JSONDBManager�𗘗p���A�N���C�A���g�����瑗�M���ꂽJSON�̃N�G����
*     ���DB����擾�����f�[�^���e�[�u���ɂ��ăN���C�A���g�ɕԂ������̃N���X�B
* �쐬��:2016.10.20
* �쐬��:R.Shibata
*/

class procedureGetList : public procedureBase {
public:
	procedureGetList() {

	};

	/* �N���X��:init
	* �T�v:�N���X�̏������֐��B
	* ����:����
	* �߂�l:����
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/
	void init(){
		//�e�N���X��init�֐����R�[������B
		procedureBase::init();
	}

	/* �N���X��:job
	* �T�v:�N���X���L�̏������s���֐��B
	* ����:String $jsonString:JSON������
	* �߂�l:����
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/
	void job(std::string jsonString) {
		//�e�N���X��job�����s���A�����o��JSON�̘A�z�z����i�[����
		procedureBase::job(jsonString);

		//�ԋp����JSON�z��̕�������i�[����ϐ���p�ӂ���
		std::string retArrayString = "";

		//SQL�ɂ���O�̑Ώ��̂��߁Atry�u���b�N�ň͂�
		try {
			//���R�[�h��JSON���쐬����
			retArrayString = getListJSONPlusKey(json, constants.STR_TABLE_DATA);
		}
		catch (sql::SQLException &e) {
			// �G���[���b�Z�[�W��\������
			//e.printStackTrace();
			// �v���O�������I������
		//	return;
		}
		//DB�Ƃ̐ڑ������
		disconnect();
		//�쐬����JSON��������o�͗p������֐ݒ肷��
		setOutHTMLString(retArrayString);
	}

	/* �N���X��:run
	* �T�v:�N���X��init�Ajob�֐����܂Ƃ߂Ď��s����B
	* ����:String $jsonString:JSON������
	* �߂�l:����
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/
	void run(std::string jsonString){
		//�����������ƃN���X�Ǝ��̏������܂Ƃ߂Ď��s����
		init(); //�������֐�
		job(jsonString); //�N���X���L�̏������s��
	}
};