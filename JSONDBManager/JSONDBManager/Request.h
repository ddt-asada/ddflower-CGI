#pragma once

/* 概要 Ajax通信でリクエスト情報を取得するテスト
* HttpRequestの代用
*
* 作成日 2017年7月24日
*
* 作成者 平澤敬介
*/

#include<iostream>
#include<string>
#include<cstdlib>
#include<ctype.h>

#include"picojson.h"

class ServerRequest {
public :
	ServerRequest(){
		
	};

	char hexToChar(char first, char second)
	{
		int digit;

		digit = (first >= 'A' ? ((first & 0xDF) - 'A') + 10 : (first - '0'));
		digit *= 16;
		digit += (second >= 'A' ? ((second & 0xDF) - 'A') + 10 : (second - '0'));
		return static_cast<char>(digit);
	}


	bool isLF(const std::string & str,
		const std::string::size_type & length,
		std::string::size_type & index,
		char* tmpStr)
	{
		if (tmpStr[0] != '0')
			return false;

		tmpStr[1] = toupper(tmpStr[1]);
		if (tmpStr[1] == 'A')
		{
			// [0A]は改行コード  
			return true;
		}

		if (tmpStr[1] == 'D')
		{
			// [0D]も改行コード  
			if ((index + 3) < length && str[index + 1] == '%')
			{
				// [0D]の後に[%0A]がある時は、[%0A]の後までインデックスを進めます。  
				tmpStr[0] = str[index + 2];
				tmpStr[1] = str[index + 3];
				if (tmpStr[0] == '0' &&
					(tmpStr[1] == 'A' || tmpStr[1] == 'a'))
				{
					index += 3;
				}
			}
			return true;
		}

		return false;
	}

	/** 
	   URLデコードをした文字列を返します。  
	      改行コード(0x0A, 0x0D)は'\n'にします。 
		  **/
	std::string UrlDecorde(const std::string & str)
	{
		std::string retStr;
		char tmpStr[2];
		std::string::size_type length = str.size();
		for (std::string::size_type i = 0; i < length; i++) {
			if (str[i] == '+') {
				// [+]は半角スペース  
				retStr += " ";
			}
			else if (str[i] == '%' && (i + 2) < length) {
				tmpStr[0] = str[i + 1];
				tmpStr[1] = str[i + 2];
				if (isxdigit(tmpStr[0]) && isxdigit(tmpStr[1])) {
					i += 2;

					// 改行コードのチェック  
					if (isLF(str, length, i, tmpStr)) {
						retStr += "<br>";
					}
					else
					{
						retStr += hexToChar(tmpStr[0], tmpStr[1]);
					}
				}
				else
				{
					// no digit  
					retStr += '%';
				}
			}
			else
			{
				retStr += str[i];
			}
		}
		return retStr;
	}

	std::string HTTPRequest() {
		// 環境変数[REQUEST_METHOD]に送信されたメソッド[GET/POST]が入ります。  
		std::string request_method = getenv("REQUEST_METHOD");
		if (request_method != "POST")
		{
			std::cout << "REQUEST_METHOD : " << request_method << std::endl;
			return 0;
		}

		// 送信されたサイズを取得します。  
		const char* lengthStr = getenv("CONTENT_LENGTH");
		int length = atoi(lengthStr);


		// 送信されたデータを保存する文字列のメモリを確保します。  
		char* tmpBuf = new char[length + 1];
		if (!tmpBuf)
		{
			std::cout << "Allocate is failed." << std::endl;
			return 0;
		}

		// 標準入力からPOSTされたデータを取得します。  
		if (fread(tmpBuf, 1, length, stdin) != length)
		{
			std::cout << "fread is failed." << std::endl;
			return 0;
		}
		tmpBuf[length] = '\0';
		std::string result = tmpBuf;
		std::string anset = UrlDecorde(result);

		//json= を削除
		anset.erase(anset.begin(), anset.begin() + 5);

		/*
		//サーバーから送信されてくる連想配列に付属されているkeyの位置を特定する
		int serch_key_place = result.find("&key=");
		//サーバーから送信されてくる連想配列の全体の長さを測る
		int result_length = result.length();
		//keyの値が付属されている場合、&key= を区切り文字に置換する
		if (serch_key_place + 5 < result_length) {
			//実際のパース対象を置き換える
			result.replace(serch_key_place, result_length, ",");
		}
		//keyの値が付属されていない場合、&key= を取り除く
		else if (serch_key_place + 5 == result_length) {
			//実際のパース対象を置き換える
			result.erase(result.begin() + result_length - 5, result.begin() + result_length);
		}*/
/*
		picojson::value pico;
		picojson::parse(pico, anset);
		picojson::object& obj = pico.get<picojson::object>();
		std::string set = obj["db_getQuery"].get<std::string>();
		*/
		//cout << picojson;
		//std::cout << "{" << "\"testdb_getQuery\" : " << set << "}" <<'\n';

		return anset;
	}
};
