#pragma once
/*
* 引用 http://programing-memo.blogspot.jp/2011/05/ccgi-url_11.html
*
*
*
*/

//クラスに移行。

#include<ctype.h>
#include<string>

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