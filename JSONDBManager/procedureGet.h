#pragma once

#include "procedureBase.h"

class procedureGet : public procedureBase {
public:
	procedureGet() {

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

		//JSON���擾����
		//SQL�ɂ���O�̑Ώ��̂���try�u���b�N�ň͂�
		try {
			//JSON������̍쐬���s��
			createJSON(json, constants.EMPTY_STRING, NULL);
			//SQL��O��catch�u���b�N
		}
		catch (sql::SQLException &e) {
			//�G���[���b�Z�[�W��\������
		//	e.printStackTrace();
			//�v���O�������I������
			return;
		}

		//DB�Ƃ̐ڑ������
		disconnect();

		//�A�z�z���json�ɕϊ����ĕϐ��ɓ����
		std::string jsonOut = Encode(json);
		//�쐬����JSON��������o�͗p������֐ݒ肷��
		setOutHTMLString(jsonOut);
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