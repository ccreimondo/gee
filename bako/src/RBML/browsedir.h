#include <stdlib.h>
#include <vector>
#include <direct.h>
#include <string.h>
#include <string>
#include <io.h>
#include <stdio.h>
#include <iostream>

class CBrowseDir
{
protected:
	//存放初始目录的绝对路径，以'\'结尾
	char m_szInitDir[_MAX_PATH];

public:
	//缺省构造器
	CBrowseDir();

	//设置初始目录为dir，如果返回false，表示目录不可用
	bool SetInitDir(const char *dir);

	//开始遍历初始目录及其子目录下由filespec指定类型的文件
	//filespec可以使用通配符 * ?，不能包含路径。
	//如果返回false，表示遍历过程被用户中止
	bool BeginBrowse(const char *filespec);
	std::vector<std::string> BeginBrowseFilenames(const char *filespec);

protected:
	//遍历目录dir下由filespec指定的文件
	//对于子目录,采用迭代的方法
	//如果返回false,表示中止遍历文件
	bool BrowseDir(const char *dir, const char *filespec);
	std::vector<std::string> GetDirFilenames(const char *dir, const char *filespec);
	//函数BrowseDir每找到一个文件,就调用ProcessFile
	//并把文件名作为参数传递过去
	//如果返回false,表示中止遍历文件
	//用户可以覆写该函数,加入自己的处理代码
	virtual bool ProcessFile(const char *filename);

	//函数BrowseDir每进入一个目录,就调用ProcessDir
	//并把正在处理的目录名及上一级目录名作为参数传递过去
	//如果正在处理的是初始目录,则parentdir=NULL
	//用户可以覆写该函数,加入自己的处理代码
	//比如用户可以在这里统计子目录的个数
	virtual void ProcessDir(const char *currentdir, const char *parentdir);
};



//从CBrowseDir派生出的子类，用来统计目录中的文件及子目录个数
class CStatDir :public CBrowseDir
{
protected:
	int m_nFileCount;   //保存文件个数
	int m_nSubdirCount; //保存子目录个数

public:
	//缺省构造器
	CStatDir()
	{
		//初始化数据成员m_nFileCount和m_nSubdirCount
		m_nFileCount = m_nSubdirCount = 0;
	}

	//返回文件个数
	int GetFileCount()
	{
		return m_nFileCount;
	}

	//返回子目录个数
	int GetSubdirCount()
	{
		//因为进入初始目录时，也会调用函数ProcessDir，
		//所以减1后才是真正的子目录个数。
		return m_nSubdirCount - 1;
	}

protected:
	//覆写虚函数ProcessFile，每调用一次，文件个数加1
	virtual bool ProcessFile(const char *filename)
	{
		m_nFileCount++;
		return CBrowseDir::ProcessFile(filename);
	}

	//覆写虚函数ProcessDir，每调用一次，子目录个数加1
	virtual void ProcessDir(const char *currentdir, const char *parentdir)
	{
		m_nSubdirCount++;
		CBrowseDir::ProcessDir(currentdir, parentdir);
	}
};


