#include"ObjfileReader.h"

ObjfileReader::ObjfileReader()
{

}


void ObjfileReader ::stringsplit(string s,char splitch,vector<string> &v)
{
	if (!v.empty())                   //初始化容器
	{
		v.clear();
	}

	int start = 0;
	//cout << s << endl;
	for (int i = 0; i < s.size(); i++)          //切割字符串
	{
		if (s[i] == splitch && i == 0)         //如果第一个分割符，则往后递推一个字符，之后切割f的时候会有第一个为/的情况
		{
			//cout << "一:" << i <<" ";
			start++;
		}

		else if (s[i] == splitch)
		{
			//cout << "二:" << i <<" ";
			v.push_back(s.substr(start, i - start));      //代数验算即可
			start = i + 1;                                //start就跳过spilt了
		}
		else if (i == s.size() - 1)
		{
			//cout << "三:" << i <<" ";
			v.push_back(s.substr(start, i - start + 1));   //带一下数 v  -34.8998 0.0000 35.3358 验算即可
		}
	}
}
	
void ObjfileReader::ReadObj(string path,unique_ptr<Mesh> &m)
{
	ifstream ifs;

	ifs.open(MODEL, ios::in);

	string obj;         //将模型的数据读入

	if (!ifs.is_open())
	{
		cout << "文件打开失败" << endl;
		return;
	}
	while (getline(ifs, obj))
	{
		if (obj[0] == 'v' && obj[1] == ' ')               //读顶点数据
		{
			vector<string> Obj_vPosition;
			obj.erase(0, 2);                             //把v和空格给剃掉
			stringsplit(obj, ' ', Obj_vPosition);
			Vector3f position;
			position = Vector3f(stof(Obj_vPosition[0].c_str()), stof(Obj_vPosition[1].c_str()), stof(Obj_vPosition[2].c_str()));//存入向量
			m->PositionBuffer.push_back(position);
			
			//this->Obj_DataPrint(Obj_vPosition);
		}
		else if (obj[0] == 'v' && obj[1] == 'n')          //读法线数据
		{
			vector<string> Obj_vNormal;
			obj.erase(0, 2);                              //把vn和空格给剃掉
			stringsplit(obj, ' ', Obj_vNormal);
			//this->Obj_DataPrint(Obj_vNormal);
			Vector3f normal;
			normal = Vector3f(stof(Obj_vNormal[0].c_str()), stof(Obj_vNormal[1].c_str()), stof(Obj_vNormal[2].c_str()));  //存入法向量
			m->NormalBuffer.push_back(normal);
			
		}
		else if (obj[0] == 'v' && obj[1] == 't')          //读取uv数据
		{
			vector<string> Obj_vTexture;
			obj.erase(0, 2);                                //把vt和空格剃掉
			stringsplit(obj, ' ', Obj_vTexture);
			//this->Obj_DataPrint(Obj_vTexture);
			Vector2 texture;
			texture = Vector2(static_cast<float>(stof(Obj_vTexture[0].c_str())), static_cast<float>(stof(Obj_vTexture[1].c_str()))); //存入uv
			m->UVBuffer.push_back(texture);
		}
		else if (obj[0] == 'f' && obj[1] == ' ')         //读取f
		{
			vector<string> num;
			obj.erase(0, 2);                              //把f和空格剃掉
			stringsplit(obj, ' ', num);
			//cout << num[2] << endl;

			for (int i = 0; i < num.size(); i++)
			{
				vector<string> Obj_fIndex;
				stringsplit(num[i], '/', Obj_fIndex);
				Vector3i index;
				//cout << Obj_fIndex[1] << endl;
				index = Vector3i(stoi(Obj_fIndex[0].c_str()), stoi(Obj_fIndex[1].c_str()), stoi(Obj_fIndex[2].c_str())); //存入f
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

	//cout << "容器大小为：" << vtest.size() << endl;
	cout << endl;
}

ObjfileReader::~ObjfileReader()
{

}