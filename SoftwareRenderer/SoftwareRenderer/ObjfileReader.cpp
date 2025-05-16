#include"ObjfileReader.h"

ObjfileReader::ObjfileReader()
{

}


void ObjfileReader ::stringsplit(string s,char splitch,vector<string> &v)
{
	if (!v.empty())                   //��ʼ������
	{
		v.clear();
	}

	int start = 0;
	//cout << s << endl;
	for (int i = 0; i < s.size(); i++)          //�и��ַ���
	{
		if (s[i] == splitch && i == 0)         //�����һ���ָ�������������һ���ַ���֮���и�f��ʱ����е�һ��Ϊ/�����
		{
			//cout << "һ:" << i <<" ";
			start++;
		}

		else if (s[i] == splitch)
		{
			//cout << "��:" << i <<" ";
			v.push_back(s.substr(start, i - start));      //�������㼴��
			start = i + 1;                                //start������spilt��
		}
		else if (i == s.size() - 1)
		{
			//cout << "��:" << i <<" ";
			v.push_back(s.substr(start, i - start + 1));   //��һ���� v  -34.8998 0.0000 35.3358 ���㼴��
		}
	}
}
	
void ObjfileReader::ReadObj(string path,unique_ptr<Mesh> &m)
{
	ifstream ifs;

	ifs.open(MODEL, ios::in);

	string obj;         //��ģ�͵����ݶ���

	if (!ifs.is_open())
	{
		cout << "�ļ���ʧ��" << endl;
		return;
	}
	while (getline(ifs, obj))
	{
		if (obj[0] == 'v' && obj[1] == ' ')               //����������
		{
			vector<string> Obj_vPosition;
			obj.erase(0, 2);                             //��v�Ϳո�����
			stringsplit(obj, ' ', Obj_vPosition);
			Vector3f position;
			position = Vector3f(stof(Obj_vPosition[0].c_str()), stof(Obj_vPosition[1].c_str()), stof(Obj_vPosition[2].c_str()));//��������
			m->PositionBuffer.push_back(position);
			
			//this->Obj_DataPrint(Obj_vPosition);
		}
		else if (obj[0] == 'v' && obj[1] == 'n')          //����������
		{
			vector<string> Obj_vNormal;
			obj.erase(0, 2);                              //��vn�Ϳո�����
			stringsplit(obj, ' ', Obj_vNormal);
			//this->Obj_DataPrint(Obj_vNormal);
			Vector3f normal;
			normal = Vector3f(stof(Obj_vNormal[0].c_str()), stof(Obj_vNormal[1].c_str()), stof(Obj_vNormal[2].c_str()));  //���뷨����
			m->NormalBuffer.push_back(normal);
			
		}
		else if (obj[0] == 'v' && obj[1] == 't')          //��ȡuv����
		{
			vector<string> Obj_vTexture;
			obj.erase(0, 2);                                //��vt�Ϳո����
			stringsplit(obj, ' ', Obj_vTexture);
			//this->Obj_DataPrint(Obj_vTexture);
			Vector2 texture;
			texture = Vector2(static_cast<float>(stof(Obj_vTexture[0].c_str())), static_cast<float>(stof(Obj_vTexture[1].c_str()))); //����uv
			m->UVBuffer.push_back(texture);
		}
		else if (obj[0] == 'f' && obj[1] == ' ')         //��ȡf
		{
			vector<string> num;
			obj.erase(0, 2);                              //��f�Ϳո����
			stringsplit(obj, ' ', num);
			//cout << num[2] << endl;

			for (int i = 0; i < num.size(); i++)
			{
				vector<string> Obj_fIndex;
				stringsplit(num[i], '/', Obj_fIndex);
				Vector3i index;
				//cout << Obj_fIndex[1] << endl;
				index = Vector3i(stoi(Obj_fIndex[0].c_str()), stoi(Obj_fIndex[1].c_str()), stoi(Obj_fIndex[2].c_str())); //����f
				m->IndexBuffer.push_back(index);
				//this->Obj_DataPrint(Obj_fIndex);
			}
			//cout << endl;
		}
	}
}

void ObjfileReader::Obj_DataPrint(vector<string> vtest)
{
	for (auto num : vtest)
	{
		cout << num << " ";
	}

	//cout << "������СΪ��" << vtest.size() << endl;
	cout << endl;
}

ObjfileReader::~ObjfileReader()
{

}