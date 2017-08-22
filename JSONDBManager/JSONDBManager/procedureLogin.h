#pragma once

#include "account.h"
#include "procedureBase.h"

procedureBase Base;

class procedureLogin : public account {
public:
	procedureLogin() {

	};

	/* �N���X��:init
	* �T�v:�N���X�̏������֐��B���O�C���p�̃N���X�̏������֐����R�[������B
	* ����:����
	* �߂�l:����
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/
	void init(){
		//ok
		//account�N���X��init�֐����R�[������B
		account::init();
	}

	/* �N���X��:job
	* �T�v:�N���X���L�̏������s���֐��B���O�C���������s���B
	* ����:String $jsonString:JSON������
	* �߂�l:����
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/
	void job(std::string jsonString) {
		//���O�C�������s����
		login(jsonString);
		//DB�Ƃ̐ڑ������
		disconnect();
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