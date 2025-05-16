#include"MyMath.h"

float Clamp(float x, float min, float max)
{
	if (x > max)
	{
		x = max;
	}
	else if (x < min)
	{
		x = min;
	}
	return x;
}

Vector3f CenterOfGravity(Vector3f v1, Vector3f v2, Vector3f v3, Vector2 p)     //计算重心坐标
{
	if ((-(v1.x - v2.x) * (v3.y - v2.y) + (v1.y - v2.y) * (v3.x - v2.x)) == 0)
	{
		return Vector3f(1, 0, 0);
	}
	if (-(v2.x - v3.x) * (v1.y - v3.y) + (v2.y - v3.y) * (v1.x - v3.x) == 0)
	{
		return Vector3f(1, 0, 0);
	}

	float alpha = (-(p.x - v2.x) * (v3.y - v2.y) + (p.y - v2.y) * (v3.x - v2.x)) / (-(v1.x - v2.x) * (v3.y - v2.y) + (v1.y - v2.y) * (v3.x - v2.x));
	float beta = (-(p.x - v3.x) * (v1.y - v3.y) + (p.y - v3.y) * (v1.x - v3.x)) / (-(v2.x - v3.x) * (v1.y - v3.y) + (v2.y - v3.y) * (v1.x - v3.x));
	float gamma = 1 - alpha - beta;

	/*cout << "alpha: " << alpha << " ";
	cout << "beta: "  << beta << " ";
	cout << "gamma: " << gamma << endl;*/
	//Vector3f(alpha, beta, gamma).Print();
	//cout << "-----------------------------------------" << endl;
	return Vector3f(alpha, beta, gamma);
}