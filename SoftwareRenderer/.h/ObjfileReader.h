#pragma once
#include"global.h"
using namespace std;
#include<fstream>
#include"Vector3.h"
#include"Mesh.h"

//���ļ�����
class ObjfileReader
{
public:

	ObjfileReader();

	void stringsplit(string s, char splitch,vector<string> &v);  //���ָ����,sΪ���ָ��ַ�����splitchΪ�ָ��,vΪ��ŵ�����

	void ReadObj(string path,Mesh *m);                                              //��ȡģ���ļ���Ϣ

	void Obj_DataPrint(vector<string> vtest);                         //��ȡģ�����ݲ���
	          
	~ObjfileReader();
};