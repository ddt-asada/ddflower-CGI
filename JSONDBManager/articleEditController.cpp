#include "Constants.h"
#include "picojson.h"

/*
* �t�@�C����:jsp/articleEditController.jsp
* �T�v	:�u���O�쐬�p�̃R���g���[���[�@�L���ԍ����Z�b�V�����֕ۑ�����
* �쐬��:R.Shibata
* �쐬��:2016.10.26
*/

void articleEditController() {
	/*
	//�N���C�A���g���瑗�M���ꂽ�L���ԍ����擾����
	//std::string number = request.getParameter(Constants.KEY_NUMBER);
	std::string number;
	//�N���C�A���g��JSON�������ԋp���邽�߂̃I�u�W�F�N�g���쐬����
	picojson::object sendResult;
	//POST���ꂽ�l�����l�Ȃ�1���Z�b�g�A�����łȂ����0���Z�b�g����
	std::string check_digit = Pattern.compile("^-?[0-9]+$").matcher(number).find() ? "1" : "0";
	//�쐬�����I�u�W�F�N�g�ɁA�󂯎�����l�����l�ł��邩�ǂ����������l���Z�b�g����
	sendResult.put(Constants.KEY_SUCCESS, check_digit);
	//�������擾�ł��Ă�����
	if (check_digit.equals("1")) {
		//�Z�b�V�������J�n����
		session = request.getSession(true);
		//�L���ԍ����Z�b�V�����ɓ����
		session.setAttribute(Constants.KEY_NUMBER, number);
	}
	//�쐬�����������\�����N���C�A���g�֕ԋp����
	out.print(JSON.encode(sendResult));
	*/
}