#include "procedureSet.h"
#include "Request.h"

/*
* �t�@�C����:jsp/GetJSONString.jsp
* �T�v	:�e�[�u���̃^�O�쐬�p��JSON�z����쐬���ĕԂ��B
* �쐬��:R.Shibata
* �쐬��:2016.10.24
*/

int main() {

	//���N�G�X�g���󂯎�邽�߂̃N���X���C���X�^���X���B
	ServerRequest request;
	//�N���C�A���g�֕ԋp���邽�߂̕������錾����
	std::string outValue = "";
	//�N���C�A���g����擾����json�f�[�^���擾����
	std::string json = request.HTTPRequest();
	//���X�g�`����JSON�����N���X�̃C���X�^���X���쐬����
	procedureSet procedureset;
	//���O�C�����s�G���[���L���b�`���邽�߁Atry�u���b�N�ň͂�
	/*try {*/
		//���������C���X�^���X�̏����֐������s����
		procedureset.run(json/*, request, response*/);
		//�쐬�����������\�����邽�߂̕�����֊i�[����
		outValue = procedureset.getOutHTMLString();
		//���O�C���`�F�b�N�G���[�����������ꍇ
	/*}
	catch (LoginCheckException e) {
		//�G���[���b�Z�[�W���쐬���A�\�����邽�߂̕�����֊i�[����
		outValue = Constants.ERROR_JSON_FRONT + e.checkLoginState() + Constants.ERROR_JSON_BACK;
		//���̑��G���[�����������ꍇ
	}
	catch (Exception e) {
		//�G���[���b�Z�[�W��\������
		e.printStackTrace();
	}
	*///�쐬�����������\�����N���C�A���g�֕ԋp����
	cout << outValue;

	return 0;
}