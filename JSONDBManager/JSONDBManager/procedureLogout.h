#pragma once

#include "procedureBase.h"

class procedureLogout : public procedureBase {
public:
	procedureLogout() {

	};
	/* �N���X��:init
	* �T�v:�N���X�̏������֐��B�e�N���X�̏������֐����R�[������B
	* ����:����
	* �߂�l:����
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/
	void init(){
		//�e�N���X��init�֐����R�[������
		procedureBase::init();
	}

	/* �N���X��:job
	* �T�v:�N���X���L�̏������s���֐��B���O�A�E�g�������s���B
	* ����:����
	* �߂�l:����
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/
	void job() {
		//���O�A�E�g�������s��
		logout();
		//DB�Ƃ̐ڑ������
		disconnect();
	}

	/* �N���X��:run
	* �T�v:�N���X��init�Ajob�֐����܂Ƃ߂Ď��s����B
	* ����:����
	* �߂�l:����
	* �쐬��:2016.10.20
	* �쐬��:R.Shibata
	*/
	void run(){
		//�����������ƃN���X�Ǝ��̏������܂Ƃ߂Ď��s����
		init(); //�������֐�
		job(); //�N���X���L�̏������s��
	}
};