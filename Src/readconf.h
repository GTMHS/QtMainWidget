#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

float* read_config(string filename);

string read_mask(string filename);

float* read_config(string filename) {
	ifstream ss(filename);
	//float binary_threshold = 80.0;//��ֵ����ֵ��Ĭ��Ϊ80
	//float max_difference_ratio = 0.5;//��������ֵ��Ĭ��Ϊ0.5
	//float ignore_left_right_ratio = 0.15;//��������������Ĭ��Ϊ0.15
	//float ignore_top_bottom_ratio = 0.2;//��������������Ĭ��Ϊ0.2
	//float min_nonzero_pixel_ratio = 0.5; //��С�������ر�����Ĭ��Ϊ0.5

	float arr[5] = { 80.0, 0.5, 0.15, 0.2, 0.5 };
	int i = 0;
	while (i < 5 && !ss.eof())
	{
		string line;
		ss >> line;
		arr[i] = float(atof(line.c_str()));
		i++;
	}
	ss.close();

	return arr;
}
string read_mask(string filename)
{
	ifstream ss(filename);
	string line;
	ss >> line;
	return line;
}