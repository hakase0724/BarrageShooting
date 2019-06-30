#include "stdafx.h"
#include "CSVLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>  

using namespace MyDirectX;
using namespace std;

vector<LOAD_FROM_CSV_DATA> CSVLoader::LoadData(const char * filePath)
{
	//�Ԃ��z��
	vector<LOAD_FROM_CSV_DATA> dataTable;

	//�t�@�C���I�[�v��
	ifstream fileStream(filePath);
	//���s���Ă������̔z���Ԃ�
	if (!fileStream) return dataTable;

	//���s�ڂ�
	int i = 0;

	//�t�@�C�����I���܂œǂݍ���
	while (!fileStream.eof())
	{
		i++;
		string bufferString;
		fileStream >> bufferString;
		//1�s�ڂ͗񖼂Ńf�[�^�ł͂Ȃ����߃X�L�b�v
		if (i <= 1) continue;

		//�f�[�^�����ڂ��Ƃɒ��o
		istringstream stringStream(bufferString);
		vector<string> datas;
		string tmp;
		//�J���}��؂�Ńf�[�^��؂蕪��
		while (getline(stringStream, tmp, ','))
		{
			datas.push_back(tmp);
		}
		//�؂蕪�����f�[�^���\���̂Ɋi�[
		LOAD_FROM_CSV_DATA data;
		data.Name = datas[0];
		data.StartPos.x = strtof(datas[1].c_str(), NULL);
		data.StartPos.y = strtof(datas[2].c_str(), NULL);
		data.StartPos.z = strtof(datas[3].c_str(), NULL);
		data.HP = strtod(datas[4].c_str(), NULL);
		auto tag = datas[5];
		if (tag == "Player")
		{
			data.Tag = PlayerTag;
			data.Category = CategoryPlayer;
		}
		if (tag == "Boss")
		{
			data.Tag = EnemyTag;
			data.Category = CategoryBossEnemy;
		}
		if (tag == "Red")
		{
			data.Tag = EnemyTag;
			data.Category = CategoryRedEnemy;
		}
		if (tag == "Blue")
		{
			data.Tag = EnemyTag;
			data.Category = CategoryBlueEnemy;
		}
		if (tag == "Green")
		{
			data.Tag = EnemyTag;
			data.Category = CategoryGreenEnemy;
		}
		data.Wave = strtol(datas[6].c_str(), NULL, 10);
		dataTable.push_back(data);
	}
	//�t�@�C���N���[�Y
	fileStream.close();
	return dataTable;
}