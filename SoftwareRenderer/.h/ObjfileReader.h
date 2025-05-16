#pragma once
#include"global.h"
using namespace std;
#include<fstream>
#include"Vector3.h"
#include"Mesh.h"

//读文件操作
class ObjfileReader
{
public:

	ObjfileReader();

	void stringsplit(string s, char splitch,vector<string> &v);  //做分割操作,s为待分割字符串，splitch为分割符,v为存放的容器

	void ReadObj(string path,Mesh *m);                                              //读取模型文件信息

	void Obj_DataPrint(vector<string> vtest);                         //读取模型数据测试
	          
	~ObjfileReader();
};