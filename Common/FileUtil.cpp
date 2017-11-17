#include "FileUtil.h"

CFileUtil::CFileUtil()
{
}

CFileUtil::~CFileUtil()
{

}

void CFileUtil::WriteCSVFile(const string &strFileName, const vector<list<string>> &Datas, const vector<int> &dataLens)
{
	ofstream ofs;
	int nIndex = 0;
	ofs.open(strFileName.c_str(), ios::out);
	if (ofs.is_open())
	{
		for (vector<list<string>>::const_iterator it = Datas.cbegin(); it != Datas.cend(); it++)
		{
			nIndex = 0;
			for (list<string>::const_iterator list_it = it->cbegin(); list_it != it->cend(); list_it++)
			{
				ofs << setiosflags(ios::left) << setw(dataLens.at(nIndex)) << setfill(' ') << *(list_it) << ',';
				nIndex++;
			}
			ofs << endl;
		}
	}

	ofs.close();
}

void CFileUtil::WriteCSVFile(const string &strFileName, const vector<list<string>> &Datas)
{
	ofstream ofs;
	int nIndex = 0;
	ofs.open(strFileName.c_str(), ios::out);
	if (ofs.is_open())
	{
		for (vector<list<string>>::const_iterator it = Datas.cbegin(); it != Datas.cend(); it++)
		{
			nIndex = 0;
			for (list<string>::const_iterator list_it = it->cbegin(); list_it != it->cend(); list_it++)
			{
				ofs << *(list_it) << ',';
				nIndex++;
			}
			ofs << endl;
		}
	}

	ofs.close();
}

void CFileUtil::ReadCSVFile(const string &strFileName, vector<list<string>> &Datas)
{

}
