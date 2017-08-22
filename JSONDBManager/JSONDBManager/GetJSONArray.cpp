#include "procedureGetList.h"
#include "Request.h"
#include "JSONtest.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <direct.h>

#include "boost/io/ios_state.hpp"

#include "CgiSaveDataManager.h"
#include "CgiCookieManager.h"
#include "CgiSessionManager.h"


/*
* �t�@�C����:jsp/GetJSONArray.jsp
* �T�v	:�e�[�u���̃^�O�쐬�p��JSON�z����쐬���ĕԂ��B
* �쐬��:R.Shibata
* �쐬��:2016.10.24
*/


int main() {
	Constants constants;
	//�e�X�g�p
	JSONDBManager test;

	//std::string json = testString();

	//std::string json = "{\"class\":\"reservedLessonTable commonTable\",\"db_getQuery\":\"SELECT '' AS lessonDateTime, lesson_name, school_name, '' AS sumCost, '' AS lessonPoint, start_time, end_time, user_classwork_cost, MAX(point_rate) AS point_rate, user_classwork_cost_aj, get_point, flower_cost, flower_cost_aj, extension_cost, lesson_date, today, lesson_key, id, biggest_students, students, order_students, user_work_status, classwork_key, stop_order_date FROM(SELECT  user_classwork_cost, user_classwork_cost_aj, user_classwork.get_point AS get_point, flower_cost, flower_cost_aj, user_classwork.extension_cost AS extension_cost, lesson_name, school_name, start_time, end_time, time_table_day.lesson_date AS lesson_date, lesson_point_rate.point_rate AS point_rate, students, order_students, SUBSTRING(NOW(), 1, 10) AS today, user_classwork.stage_no AS stage_no, user_classwork.level_no AS level_no, classwork.lesson_key AS lesson_key, user_classwork.id as id, biggest_students, user_work_status, classwork.id AS classwork_key, stop_order_date FROM user_classwork INNER JOIN classwork ON classwork.id = user_classwork.classwork_key AND user_classwork.user_key = 'user_key' INNER JOIN  lesson_inf ON lesson_inf.id = classwork.lesson_key INNER JOIN school_inf ON school_inf.id = lesson_inf.school_key INNER JOIN time_table_day ON time_table_day.id = classwork.time_table_day_key INNER JOIN timetable_inf ON timetable_inf.id = time_table_day.timetable_key LEFT JOIN(SELECT MAX(students) AS biggest_students, lesson_key FROM lesson_point_rate GROUP BY lesson_key) AS lsp ON lsp.lesson_key = lesson_inf.id AND  order_students > biggest_students INNER JOIN lesson_point_rate ON lesson_point_rate.lesson_key = lesson_inf.id) AS student_class_rec WHERE user_work_status = 1 AND lesson_date >= 'lesson_date' GROUP BY id ORDER BY lesson_date ASC; \",\"user_key\":{\"value\":\"3\"},\"lesson_date\":{\"value\":\"2017 - 8 - 21\"},\"lesson_name\":{\"value\":\"\"},\"replace_query\":\"SELECT '' AS lessonDateTime, lesson_name, school_name, '' AS sumCost, '' AS lessonPoint, start_time, end_time, user_classwork_cost, MAX(point_rate) AS point_rate, user_classwork_cost_aj, get_point, flower_cost, flower_cost_aj, extension_cost, lesson_date, today, lesson_key, id, biggest_students, students, order_students, user_work_status, classwork_key, stop_order_date FROM(SELECT  user_classwork_cost, user_classwork_cost_aj, user_classwork.get_point AS get_point, flower_cost, flower_cost_aj, user_classwork.extension_cost AS extension_cost, lesson_name, school_name, start_time, end_time, time_table_day.lesson_date AS lesson_date, lesson_point_rate.point_rate AS point_rate, students, order_students, SUBSTRING(NOW(), 1, 10) AS today, user_classwork.stage_no AS stage_no, user_classwork.level_no AS level_no, classwork.lesson_key AS lesson_key, user_classwork.id as id, biggest_students, user_work_status, classwork.id AS classwork_key, stop_order_date FROM user_classwork INNER JOIN classwork ON classwork.id = user_classwork.classwork_key AND user_classwork.user_key = 'user_key' INNER JOIN  lesson_inf ON lesson_inf.id = classwork.lesson_key INNER JOIN school_inf ON school_inf.id = lesson_inf.school_key INNER JOIN time_table_day ON time_table_day.id = classwork.time_table_day_key INNER JOIN timetable_inf ON timetable_inf.id = time_table_day.timetable_key LEFT JOIN(SELECT MAX(students) AS biggest_students, lesson_key FROM lesson_point_rate GROUP BY lesson_key) AS lsp ON lsp.lesson_key = lesson_inf.id AND  order_students > biggest_students INNER JOIN lesson_point_rate ON lesson_point_rate.lesson_key = lesson_inf.id) AS student_class_rec WHERE user_work_status = 1 AND lesson_date >= 'lesson_date' AND  lesson_name IN('lesson_name') GROUP BY id ORDER BY lesson_date ASC; \", \"allSearch_query\":\"SELECT '' AS lessonDateTime, lesson_name, school_name, '' AS sumCost, '' AS lessonPoint, start_time, end_time, user_classwork_cost, MAX(point_rate) AS point_rate, user_classwork_cost_aj, get_point, flower_cost, flower_cost_aj, extension_cost, lesson_date, today, lesson_key, id, biggest_students, students, order_students, user_work_status, classwork_key, stop_order_date FROM(SELECT  user_classwork_cost, user_classwork_cost_aj, user_classwork.get_point AS get_point, flower_cost, flower_cost_aj, user_classwork.extension_cost AS extension_cost, lesson_name, school_name, start_time, end_time, time_table_day.lesson_date AS lesson_date, lesson_point_rate.point_rate AS point_rate, students, order_students, SUBSTRING(NOW(), 1, 10) AS today, user_classwork.stage_no AS stage_no, user_classwork.level_no AS level_no, classwork.lesson_key AS lesson_key, user_classwork.id as id, biggest_students, user_work_status, classwork.id AS classwork_key, stop_order_date FROM user_classwork INNER JOIN classwork ON classwork.id = user_classwork.classwork_key AND user_classwork.user_key = 'user_key' INNER JOIN  lesson_inf ON lesson_inf.id = classwork.lesson_key INNER JOIN school_inf ON school_inf.id = lesson_inf.school_key INNER JOIN time_table_day ON time_table_day.id = classwork.time_table_day_key INNER JOIN timetable_inf ON timetable_inf.id = time_table_day.timetable_key LEFT JOIN(SELECT MAX(students) AS biggest_students, lesson_key FROM lesson_point_rate GROUP BY lesson_key) AS lsp ON lsp.lesson_key = lesson_inf.id AND  order_students > biggest_students INNER JOIN lesson_point_rate ON lesson_point_rate.lesson_key = lesson_inf.id) AS student_class_rec WHERE user_work_status = 1 AND lesson_date >= 'lesson_date' GROUP BY id ORDER BY lesson_date ASC; \",\"config\":{\"columns\":{\"lessonDateTime\":{\"columnName\":\"�J�Ó���\"},\"lesson_name\":{\"columnName\":\"�e�[�}\"},\"school_name\":{\"columnName\":\"�X��\"},\"sumCost\":{\"columnName\":\"����(�~)\"},\"lessonPoint\":{\"columnName\":\"�擾�\��|�C���g(pt)\"},\"start_time\":{\"style\":\"display:none\"},\"end_time\":{\"style\":\"display : none\"},\"user_classwork_cost\":{\"style\":\"display : none\"},\"point_rate\":{\"style\":\"display : none\"},\"user_classwork_cost_aj\":{\"style\":\"display : none; \"},\"get_point\":{\"style\":\"display:none; \"},\"flower_cost\":{\"style\":\"display:none; \"},\"flower_cost_aj\":{\"style\":\"display:none; \"},\"extension_cost\":{\"style\":\"display:none; \"},\"lesson_date\":{\"style\":\"display:none; \"},\"today\":{\"style\":\"display:none; \"},\"lesson_key\":{\"style\":\"display:none; \"},\"id\":{\"style\" : \"display:none; \"},\"biggest_students\":{\"style\":\"display:none; \"},\"students\":{\"style\":\"display:none; \"},\"order_students\":{\"style\":\"display:none; \"},\"user_work_status\":{\"style\":\"display:none; \"},\"classwork_key\":{\"style\":\"display:none; \"},\"stop_order_date\":{\"style\":\"display:none; \"}}},\"noRecordMessage\":\"�\�񒆂̎��Ƃ�������܂���ł����B\"}";

	
	const char* answer = "Content-Type:text/html;charset=utf-8;\n";

	std::cout << answer;
	
	//HTTP���POST���\�b�h�Ńf�[�^���󂯎��N���X�̃C���X�^���X���B
	ServerRequest request;
	
	//HTTP���POST���\�b�h�ő���ꂽ�f�[�^���󂯎��B
	std::string json = request.HTTPRequest();
	//test.fileout(json);
	
	//�N���C�A���g�֕ԋp���邽�߂̕������錾����
	std::string outValue = "";
	//���X�g�`����JSON�����N���X�̃C���X�^���X���쐬����
	procedureGetList listJsonDbGetter;
	
	//���������C���X�^���X�̏����֐������s����
	listJsonDbGetter.run(json);
	//�쐬�����������\�����N���C�A���g�֕ԋp����
	outValue = listJsonDbGetter.getOutHTMLString();

	//�쐬�����������\�����N���C�A���g�֕ԋp����
	std::cout <<'\n' << outValue;
	
	
	return 0;
}