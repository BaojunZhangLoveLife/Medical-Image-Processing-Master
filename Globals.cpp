#include "Globals.h"
#include <io.h>  
#include <fstream>  
#include <string>
#include <vector>
#include <iostream>
#include <io.h>
//获取根路径
void GetRootPath(std::string path, std::vector<std::string>& files)
{
	intptr_t   hFile = 0;
	//文件信息    
	struct _finddata_t fileinfo;//用来存储文件信息的结构体    
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {  //第一次查找  
		do {
			if ((fileinfo.attrib &  _A_SUBDIR)) {  //如果查找到的是文件夹
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)  //进入文件夹查找  
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile); //结束查找  
	}
}

//获取子路径
void GetSubPath(std::string path, std::vector<std::string>& files)
{
	intptr_t   hFile = 0;
	//文件信息    
	struct _finddata_t fileinfo;//用来存储文件信息的结构体    
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {  //第一次查找  
		do {
			if ((fileinfo.attrib &  _A_SUBDIR)) {  //如果查找到的是文件夹
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)  //进入文件夹查找  
				{
					GetSubPath(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else //如果查找到的不是是文件夹   
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile); //结束查找  
	}
}
