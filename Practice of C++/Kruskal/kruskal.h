#pragma once

#include "matrix.h"

struct E{//����
	int u;//smaller one
	int v;//larger one
	double w;
	E * np;

};
//��Ȧ��
double Rosenstiehl(Matrix<double> & st,char * & ans);//����һ���Գƾ��󣨹������󣩣����������Ȩ�͸
//��Ȧ��
double Prim(Matrix<double> & st,char * & ans);//����һ���Գƾ��󣨹������󣩣��ҵ�������
double Kruskal(Matrix<double> & st,char * & ans);//����һ���Գƾ��󣨹�������,�ҵ�������

//Private Functions:
static char * write_char(char * inn,int num);
static int find_root(int ser,int *all);
static void cut_circle(char * Ins,int n);


