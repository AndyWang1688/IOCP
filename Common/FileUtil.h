#pragma once
#include <fstream>
#include<iomanip>
#include <string>
#include <vector>
#include <list>
using namespace std;

class CFileUtil
{
public:
	CFileUtil();
	~CFileUtil();

	void WriteCSVFile(const string &strFileName, const vector<list<string>> &Datas, const vector<int> &dataLens);
	void WriteCSVFile(const string &strFileName, const vector<list<string>> &Datas);
	void ReadCSVFile(const string &strFileName, vector<list<string>> &Datas);
};