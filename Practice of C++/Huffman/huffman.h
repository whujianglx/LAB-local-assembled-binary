
#ifndef _HUFFMAN_H
#define _HUFFMAN_H
#endif

typedef  double  ElementType;

struct Linked_list{
	ElementType data;
	Linked_list * Left;
	Linked_list * Right;
};

class Heap_H{	

private:
	int Capacity;
	int Pos;
	Linked_list  * * fp;
public:
	Heap_H(int size);
	~Heap_H();
	bool Is_Full(void);
	bool Is_Empty(void);
	bool Insert(Linked_list * a);
	Linked_list * Read_Delete(void);

};

Linked_list * Huffman(ElementType * fp,int num);//�׵�ַָ�룬Ԫ�ظ���
void Print_leaves(Linked_list *list,int num);	//��ӡ��Ҷ
void Delete_Linked_list(Linked_list * list);	//ɾ������
	





