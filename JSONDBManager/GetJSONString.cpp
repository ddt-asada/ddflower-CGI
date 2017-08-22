#include "DbConnect.h"
#include "procedureGet.h"
#include "procedureLogin.h"
#include "Request.h"
#include "JSONtest.h"

/*
* �t�@�C����:jsp/GetJSONString.jsp
* �T�v	:�N���C�A���g����n���ꂽJSON�������DB����擾�����l��ǉ����ĕԂ��B
* 		�܂��A���O�C���p��JSON��n���ꂽ�烍�O�C���p�̃N���X�𐶐����ă��O�C���������s���B
* �쐬��:R.Shibata
* �쐬��:2016.10.24
*/

int main() {
	Constants constants;
	JSONDBManager test;
	
	const char* answer = "Content-Type:text/html;charset=utf-8\n";

	std::cout << answer;

	//HTTP���POST���\�b�h�Ńf�[�^���󂯎��N���X�̃C���X�^���X���B
	ServerRequest request;

	//HTTP���POST���\�b�h�ő���ꂽ�f�[�^���󂯎��B
	std::string json = request.HTTPRequest();
	
	//�N���C�A���g�֕ԋp���邽�߂̕������錾����
	std::string outValue = "";
	//�e�X�g�p�̉��̕�����B
	//std::string json = "{\"db_getQuery\":\"SELECT id AS numberArea, user_name, get_point, authority FROM user_inf WHERE id = 'user_key'; \",\"user_key\":{\"value\":\"3\"},\"authority\":{\"text\":\"10\"},\"memberStatus\":{\"topLogoArea\":{\"topLink\":{\"href\":\"#top.php\",\"topLogoImage\":{\"src\":\"image / top(logo).png\"}}},\"memberName\":{\"user_name\":{\"text\":\"\"},\"nameTitleArea\":{\"text\":\"�l\"}},\"memberPoint\":{\"get_point\":{\"text\":\"\"},\"ptArea\":{\"class\":\"ptArea unitSpace\",\"text\":\"pt\"}},\"logoutArea\":{\"class\":\"logoutArea inlineBlock\",\"logoutLink\":{\"href\":\"\",\"logoutImage\":{\"src\":\"image / icon(logout22 - 50).png\"}}}}}";
	//json�Ƀ��[�U���A�p�X���[�h�AID�̕����񂪊܂܂�Ă����
	if (json.find(constants.USER_NAME) != -1 && json.find(constants.STR_PASSWORD) != -1
		&& json.find(constants.STR_ID) != -1) {
		//���O�C���p�̏������s���N���X�̃C���X�^���X�𐶐�����
		procedureLogin procedurelogin;
		//���O�C�����s�G���[���L���b�`���邽�߁Atry�u���b�N�ň͂�
		/*try {*/
		//���������C���X�^���X�̏����֐������s����
		procedurelogin.run(json);
		//�쐬�����������\�����邽�߂̕�����֊i�[����
		outValue = procedurelogin.getOutHTMLString();

	//���O�C���`�F�b�N�G���[�����������ꍇ
	/*	}
		catch (Exception e) {
		//�G���[���b�Z�[�W��\������
		e.printStackTrace();
		}*/
	}
	else {
		//���O�C���p�̏������s���N���X�̃C���X�^���X�𐶐�����
		procedureGet procedureget;
		//���O�C�����s�G���[���L���b�`���邽�߁Atry�u���b�N�ň͂�
		//try {
		//���������C���X�^���X�̏����֐������s����
		procedureget.run(json);
		//�쐬�����������\�����邽�߂̕�����֊i�[����
		outValue = procedureget.getOutHTMLString();
		//���O�C���`�F�b�N�G���[�����������ꍇ
	//		}
/*		catch (std::exception &e) {
		//�G���[���b�Z�[�W��\������
			outValue = constants.ERROR_JSON_FRONT + "0" + constants.ERROR_JSON_BACK;
		}*/
	}
	//�쐬�����������\�����N���C�A���g�֕ԋp����
	std::cout << '\n' <<outValue;

	return 0;
}