#include "stdafx.h"
#include "BPnetwork.h"


double defaultF(double _X)//default Sigmoid Function
{
    return 1/(1+exp(-_X));

}

double defaultdF(double _X,double _Y){
	return (1-_Y)*_Y;

}

int BPnetwork::Depth(void){return _depth;}

void BPnetwork::SetBPConfig(int T,double err){//�������в���
	_Tmax=T;
	_err=err;
}
BPnetwork::BPnetwork(int depth,int *num_fp){//�½�
	
	if (depth<1){
		zero=true;
		_depth=0;
		_fp=NULL;
	}
	else
	{
		int i;
		_depth=depth;//Initialize
		_fp=new BPlayer * [depth];
//
		if (num_fp==NULL){
			for (i=0;i<depth;i++){
				_fp[i]=new BPlayer(i,20,this);//Ĭ�ϲ���
			}
		}
		else{
			for (i=0;i<depth;i++){
				_fp[i]=new BPlayer(i,num_fp[i],this);		
			}	
		}
		zero=false;
	}
	//Initialize
	F=defaultF;
	dF=defaultdF;
	_Tmax=100;
	_err=0.1;
	trained=false;
	return;
}

BPnetwork::~BPnetwork(){//��������
	if (zero==false){
		int i;
		for (i=0;i<this->_depth;i++)
			delete _fp[i];
		delete [] _fp;
	}
}

void BPnetwork::BPchoosefunction(double (* bpf)(double _X),double (* bpdf)(double _X,double _Y)){//����S����
	F=bpf;
	dF=bpdf;

	return;
}
bool BPnetwork::BPdelete(int index){


//Unwritten

	return true;
}
bool BPnetwork::BPinsert(int index){

//Unwritten

	return true;
}
bool BPnetwork::BPpoplayer(int num){
	int i;
	if (_depth==1){
		_depth=0;
		delete _fp[0];
		delete [] _fp;
		_fp=NULL;
		zero=true;
		return true;
	}
	if (zero==false){
		BPlayer ** temp;
		_depth--;
		temp=new BPlayer * [_depth];
		delete _fp[_depth];
		for (i=0;i<_depth;i++)
			temp[i]=_fp[i];
		delete [] _fp;
		_fp=temp;
		return true;
	}

	trained=false;//�ṹ�ı�
	return false;
}
bool BPnetwork::BPpushlayer(int num){
	int i;
	BPlayer ** temp;
	temp=new BPlayer * [_depth+1];
	for(i=0;i<_depth;i++)
		temp[i]=_fp[i];
	delete [] _fp;
	_fp=temp;
	temp[_depth]=new BPlayer(_depth,num,this);
	_depth++;

	trained=false;//�ṹ�ı�
	return true;
}
int BPnetwork::BPTrain(double * input,int width,int height,double * rate){//����ѵ������
	
	_output=_fp[_depth-1]->_num;//�������
	_height=height-_output;//ʵ������

	if (zero==true)//����Ԫ���罨��
		return -1;
	if (width<1 || _height<1 || _output<1)//����
		return -1;

	//Copy
	_input=input;
	_width=width;
	
	double * repl=new double[_depth];
	int i;
	int j;
	int k;
	double TotalError;

	if (rate==NULL){
		rate=repl;
		for (i=0;i<_depth;i++){//����һ��Ĭ��ѧϰ����
			rate[i]=0.001;//		
		}
	}

	_fp[0]->Establish(_height);
	for (i=1;i<_depth;i++)
		_fp[i]->Establish(_fp[i-1]->_num);

		
	TotalError=DBL_MAX;
	for(i=0;i<_Tmax;i++){//ѧϰ����
		if (TotalError<_err)
			break;

		TotalError=0;
		for (k=0;k<width;k++){//clamp each input

			for(j=0;j<_depth;j++){
				_fp[j]->FeedForward(k);//ǰ�򴫵�
			}
			TotalError+=CalculateError(k);//�������

			for(j=_depth-1;j>-1;j--){//��󴫵�
				_fp[j]->BackPropagate(k,rate[j]);//����Ȩֵ			
			}
			for (j=0;j<_depth;j++){
				_fp[j]->Refresh();//����			
			}
		}
	}
	delete [] repl;
	trained=true;
	return i;
}
bool BPnetwork::BPBroadCast(double * input,int width){

	if (trained==false || zero==true)
		return false;
	
	int i;int j;
	_input=input;
	_width=width;

	BPlayer * last=_fp[_depth-1];
	for (i=0;i<width;i++){//i is clamped
		for (j=0;j<_depth;j++){//j ��Ԫ����
			_fp[j]->FeedForward(i);	
		}
		//ȡ������
		for (j=0;j<_output;j++){// ���һ����Ԫ���ȡֵ
			input[(_height+j)*width+i]=last->_ans[j];		
		}
	}
	return true;
}
double BPnetwork::CalculateError(int clamp){//1/2���ƽ�� ��������
	double ANS=0;
	int i;
	for (i=0;i<_output;i++){
		ANS+=0.5*pow(_fp[_depth-1]->_ans[i]-_input[(i+_height)*_width+clamp],2);	
	}
	return ANS;
}
//---------Here are codes for BPlayer---------------

BPlayer::BPlayer(int index,int num,BPnetwork * f){//Initialize
	_num=num;
	_index=index;
	_f=f;
	zero=true;
}
void BPlayer::Establish(int former){//�½�
	int i;
	_former=former;
	_w=new double[former*_num];//�����µ�Ȩ�ռ�
	_nw=new double[former * _num];//����
	_theta=new double[_num];//�µ�

	//���������
	LARGE_INTEGER seed;
    QueryPerformanceFrequency(&seed);
    QueryPerformanceCounter(&seed);
    srand(seed.QuadPart);//��ʼ��һ����΢��Ϊ��λ��ʱ������
    //

	for (i=0;i<former*_num;i++)
		_w[i]=0.001*(rand()%1000)-0.5;

	for (i=0;i<_num;i++)
		_theta[i]=0.001*(rand()%1000)-0.5;
	

	_s=new double[_num];//S
	_ans=new double[_num];//����Ĵ洢�ռ�
	_diedai=new double [_num];//�����ռ�
	zero=false;

}
BPlayer::~BPlayer(){//���տռ�
	if (!zero){
		delete [] _w; delete [] _nw;
		delete [] _theta;
		delete [] _ans;
		delete [] _s;
		delete [] _diedai;
	}
}
void BPlayer::BackPropagate(int clamp,double lr){//ѧϰ���� �𼶵���
	
	double * former;
	int i;
	int j;

	former=new double[_former];
	
	//ȡ���ϼ�����
	if (_index==0){
		int width=_f->_width;

		for (i=0;i<_former;i++){
			former[i]=_f->_input[clamp+i*width];		
		}
	}
	else{
		
		BPlayer * temp;
		temp=_f->_fp[_index-1];
		
		for(i=0;i<_former;i++){
			former[i]=temp->_ans[i];
		}  
	}
	//
	//��ʽ��ʼ����


	if (_index==_f->_depth-1){//�㼶����
		
		int height=_f->_height;
		int width=_f->_width;

		for (i=0;i<_num;i++){
			_diedai[i]=(-_ans[i]+_f->_input[(height+i)*width+clamp])*_f->dF(_s[i],_ans[i]);
			//(Oi-Di)*dDi/dS

			//ѧϰ
			for (j=0;j<_former;j++)//wij
			{
				_nw[i*_former+j]=_w[i*_former+j]+lr*_diedai[i]*former[j];
			}
			_theta[i]+=lr*_diedai[i];
		}	
	}
	else{//������
		BPlayer * bpl=_f->_fp[_index+1];
		int latter=bpl->_num;//ȡǰһ��Ԫ����
	

		//��� ǰһ����� dSn+1/dY dY/dSn 
		for (i=0;i<_num;i++){
			_diedai[i]=0;
			for (j=0;j<latter;j++){
				_diedai[i]+=bpl->_diedai[j] * bpl->_w[j*_num+i] * _f->dF(_s[i],_ans[i]);
			}
			
			//ѧϰ
			for (j=0;j<_former;j++)
			{
				_nw[i*_former+j]=_w[i*_former+j]+lr*_diedai[i]*former[j];
			}
			_theta[i]+=lr*_diedai[i];			
		}	
	}

	delete [] former;//�����ڴ�
	
	return;
}
void BPlayer::FeedForward(int clamp){
	double * former;
	int i;
	int j;

	former=new double[_former];
	
	//ȡ���ϼ�����
	if (_index==0){
		int width=_f->_width;

		for (i=0;i<_former;i++){
			former[i]=_f->_input[clamp+i*width];		
		}
	}
	else{
		
		BPlayer * temp;
		temp=_f->_fp[_index-1];
		
		for(i=0;i<_former;i++){
			former[i]=temp->_ans[i];
		}  
	}

	double SUM;
	//��ʽ��ʼ
	for (i=0;i<_num;i++)//��ǰ���ÿ����Ԫ
	{
		SUM=0;
		for (j=0;j<_former;j++)//��ǰ���ÿ����Ԫ
			SUM+=former[j]*_w[i*_former+j];
		SUM+=_theta[i];
		_s[i]=SUM;
		_ans[i]=_f->F(SUM);
	}

	delete [] former;//����

}
void BPlayer::Refresh(void){//����
	int i;
	for (i=0;i<_former*_num;i++){
		_w[i]=_nw[i];	
	}
}


