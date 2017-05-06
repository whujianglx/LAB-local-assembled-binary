#include <math.h>
#include <float.h>
#include <windows.h>  
#include <stdlib.h>
#include <stdio.h>

class BPlayer;

double defaultF(double _X);
double defaultdF(double _X,double _Y);




class BPnetwork{

friend class BPlayer;//������Ԫ��

private:
//MAIN
int _depth;
bool zero;
bool trained;
BPlayer ** _fp;//ָ������
double (* F)(double _X);//���庯��ָ��
double (* dF)(double _X,double _Y);//���庯��ָ��

//Tag
int _Tmax;//���ѵ������
double _err;//Ŀ�����

//TEMP
int _width;
int _height;//ʵ������߶�
double * _input;
int _output;


public://��Ա����

  int Depth(void);//�������
//�½�һ��BP
  BPnetwork(int depth=2,int * num_fp=NULL); //����
  ~BPnetwork();//����һ����������
  void BPchoosefunction(double (* bpf)(double _X),double (* bpdf)(double _X,double _Y));//ѡ����
  bool BPpushlayer(int num);//�½����һ��
  bool BPpoplayer(int num);//ɾ�����һ��
  bool BPdelete(int index);//ɾ����Ԫ
  bool BPinsert(int index);//������Ԫ
  int  BPTrain(double * input,int width,int height,double * rate=NULL);//���վ�����ʽ���룬���һ�������
  bool BPBroadCast(double * input,int width);
  void SetBPConfig(int T,double err);

private:
	double CalculateError(int clamp);

  

};

class BPlayer{
	friend class BPnetwork;
private:
	int _num;int _former;
	int _index;//��BPnetwork���ƶ�����
	double * _w , * _nw;
	double * _theta;
	double * _s;//S
	double * _ans;//F(S)
	double * _diedai;//�����ռ�
	bool zero;


    BPnetwork * _f;

private://���� ���ܵ���ʹ��
	BPlayer(int index,int num,BPnetwork * f);
	void Establish(int former);
	void FeedForward(int clamp);
	void BackPropagate(int clamp,double lr);
	void Refresh(void);
	~BPlayer();


};

