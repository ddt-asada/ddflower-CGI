#pragma once

#include "procedureBase.h"

class procedureSet : public procedureBase {
public:
	procedureSet()
	{

	};
	/* �N���X��:init
	* �T�v:�N���X�̏������֐��B���O�C���p�̃N���X�̏������֐����R�[������B
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
	* �T�v:�N���X���L�̏������s���֐��B���O�C���������s���B
	* ����:String $jsonString:JSON������
	* �߂�l:����
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/
	void job(std::string jsonString) {
		//�e�N���X��job�����s���A�����o��JSON�̘A�z�z����i�[����
		procedureBase::job(jsonString);
		//JSON��DB�ɔ��f������
		//SQL�ɂ���O�̑Ώ��̂��߁Atry�u���b�N�ň͂�
		//INSERT�A�܂���UPDATE���߂����s����
		executeQuery(json, constants.DB_SETQUERY);
		//SQL��O��catch�u���b�N
		//�Ō�ɍs������
		disconnect();
		//�N���C�A���g�֕Ԃ����b�Z�[�W���쐬����
		std::string returnMessage = "{\"message\":\"" + std::to_string(processedRecords) + "\"}";
		//�쐬����Json��������o�͗p������֐ݒ肷��
		setOutHTMLString(returnMessage);
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