#pragma once

#include "boost/io/ios_state.hpp"

#include "CgiSaveData.h"




namespace nana{
	
using namespace std;
using namespace boost;
using namespace boost::io;


namespace{
	
///�����𐔎��ɕϊ����� 
inline const unsigned int toUInt(const char& c)
{
	return (unsigned int)*(unsigned char*)&c;
};


///������̋󔒂���������B 
inline const string deleteSpace(string ret)
{
	string::iterator i = remove(ret.begin(), ret.end(), ' ');
	ret.erase(i, ret.end());
	return ret;
};

///16�i��������𕶎���ɖ߂� 
const string toString(const string& str)
{
	string ret;
	char tmp[3];
	char* endstr;
	istringstream is(str);
	
	//2�������ǂ�ł����Bget()�͍Ō��NULL������B
	while(is.get(tmp, 3)){
		unsigned char c = static_cast<unsigned char>(strtol(tmp, &endstr, 16));
		ret += *(char*)&c;
	}
	 
	return ret;
}

}; //namespace�����I�� 



/**
�o�͂̂��߂ɉ��Z�q��` 
pair�̃L�[���ɂ͉p�����̂ݎg�p�\�B�󔒕�����S�p�����Ȃǂ��g�p���Ȃ����ƁB 
*/
ostream& operator <<(ostream& os, const pair<const string, CgiSaveData>& data)
{
	ios_flags_saver ifs(os);//����̃X�g���[���̏����Ȃǂ�ۑ�
	
	//�L�[���̏o�� 
	os << hex << uppercase << setfill('0');
	os << data.first << "=";
	
	//�G�X�P�[�v���Ȃ���o�͂��� 
	string::const_iterator i;
	for(i=data.second.begin(); i!=data.second.end(); ++i){
		os << setw(2) << *i;
	}
	os << ";";
	
	return os;
};



/**
���͂̂��߂ɉ��Z�q��` �Bpair�P�����o�� 
*/
istream& operator >>(istream& is, pair<string, CgiSaveData>& data)
{
	stringbuf buf;
	
	//�L�[���擾 
	is.get(buf, '=');
	data.first = deleteSpace(buf.str());
	if(is.eof()) return is;
	//=��ǂݔ�΂� 
	is.get();
	
	//�l���擾 
	buf.str("");
	is.get(buf, ';');
	data.second = deleteSpace(buf.str());
	//;��ǂݔ�΂�
	if(!is.eof()) is.get();
	
	return is;
};


typedef pair<string, CgiSaveData> SaveDataPair;


/**
�f�[�^���t�@�C���ɕۑ�����B 
@return ofstream.fail()
*/
bool saveData(const SaveDataMap& smap, ostream& os)
{
	ostream_iterator<SaveDataPair> itrOut(os);
	
	//�C�e���[�^�ŃR�s�[���Ă��� 
	copy(smap.begin(), smap.end(), itrOut);
	
	return !os;
};
 

/**
�f�[�^���t�@�C������ǂݍ��ށB
@return ifstream.fail()
*/
bool loadData(SaveDataMap& smap, istream& is)
{
	typedef istream_iterator<SaveDataPair> SDIterator;
	SDIterator itrIn(is), iend;
	
	//�C�e���[�^�ő}�����Ă��� 
	for(; itrIn !=iend; ++itrIn) smap.insert(*itrIn);
	
	return !is;
};

};//namespace nana�I�� 


