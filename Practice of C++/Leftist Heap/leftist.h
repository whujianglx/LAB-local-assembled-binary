/*

	����ѵ�C�㷨ʵ��   by mark 2015.11.8  

*/

#ifndef _LEFTIST_H
#define _LEFTIST_H
#endif

typedef int ElementType;


typedef struct da_lf
{
	ElementType data;
	int npl;	
	struct da_lf * left;
	struct da_lf * right;
	
}data_struct;


data_struct * Merge(data_struct * One,data_struct * Ano);//�����������Merge
data_struct * NewOne(ElementType a);					//��һ��Ԫ�ر������ṹ
data_struct * Delete(data_struct * One);				//����С���Ǹ�Ԫ��ɾ��
void Destroy(data_struct * One);				//���� �ͷ��ڴ�
