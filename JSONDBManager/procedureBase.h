#pragma once

#include "account.h"

class procedureBase : public account {
public:
	procedureBase() {

	};

	void init(){
		//�e�N���X��init�֐����R�[������
		account::init();
		//���O�C���`�F�b�N���s��
		account::loginCheck();
	}

	void job(std::string jsonString) {
		//JSON�����񂩂�A�z�z����擾���A���g�̃����o�ɕۑ�����
		getJSONMap(jsonString);
	}

	void run(std::string jsonString) {
		//�����������ƃN���X�Ǝ��̏������܂Ƃ߂Ď��s����
		init();//�������֐�
		job(jsonString); //�N���X���L�̏������s��
	}
};