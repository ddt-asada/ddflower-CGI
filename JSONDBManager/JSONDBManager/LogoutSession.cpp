#include "procedureLogout.h"
#include "Request.h"

/*
* �t�@�C����:jsp/LogoutSession.jsp
* �T�v	:���O�A�E�g�����̂��߁A�Z�b�V������j������B
* �쐬��:R.Shibata
* �쐬��:2016.10.24
*/

//�������B
int main() {
	//ServerRequest request;

	const char* answer = "Content-Type:text/html;charset=utf-8\n";

	std::cout << answer;

	//std::string json = request.HTTPRequest();

	JSONDBManager dbug;
	//�N���C�A���g�֕ԋp���邽�߂̕������錾����
	std::string outValue = "";
	//���X�g�`����JSON�����N���X�̃C���X�^���X���쐬����
	procedureLogout logout;
	//���O�C�����s�G���[���L���b�`���邽�߁Atry�u���b�N�ň͂�
	/*try {*/
		//���������C���X�^���X�̏����֐������s����
		logout.run();
		//���O�C���`�F�b�N�G���[�����������ꍇ
/*	}
	catch (Exception e) {
		//�G���[���b�Z�[�W��\������
		e.printStackTrace();
	}*/
	//�쐬�����������\�����N���C�A���g�֕ԋp����
	std::cout << '\n' <<outValue <<'\n';
	return 0;
}