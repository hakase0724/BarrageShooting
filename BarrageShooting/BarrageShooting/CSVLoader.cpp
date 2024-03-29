#include "stdafx.h"
#include "CSVLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>  

using namespace MyDirectX;
using namespace std;

vector<LOAD_FROM_CSV_DATA> CSVLoader::LoadData(const char * filePath)
{
	//返す配列
	vector<LOAD_FROM_CSV_DATA> dataTable;

	//ファイルオープン
	ifstream fileStream(filePath);
	//失敗していたら空の配列を返す
	if (!fileStream) return dataTable;

	//何行目か
	int i = 0;

	//ファイルが終わるまで読み込み
	while (!fileStream.eof())
	{
		i++;
		string bufferString;
		fileStream >> bufferString;
		//1行目は列名でデータではないためスキップ
		if (i <= 1) continue;

		//データを項目ごとに抽出
		istringstream stringStream(bufferString);
		vector<string> datas;
		string tmp;
		//カンマ区切りでデータを切り分け
		while (getline(stringStream, tmp, ','))
		{
			datas.push_back(tmp);
		}
		//切り分けたデータを構造体に格納
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
	//ファイルクローズ
	fileStream.close();
	return dataTable;
}
